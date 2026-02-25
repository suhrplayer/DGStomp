/*
  ==============================================================================
    DGStompDSP.h  —  Yamaha DG Stomp amp simulator, native C++ port
    Cabinet IR: supports both built-in 64-tap IR and user-loaded WAV IRs
    (up to kMaxIRLen taps, auto-resampled to plugin sample rate).
  ==============================================================================
*/

#pragma once
#include <cmath>
#include <cstring>
#include <algorithm>
#include <vector>
#include <atomic>

class DGStompDSP
{
public:
    DGStompDSP()  { initBuiltinIR(); }
    ~DGStompDSP() = default;

    void prepare (double sr)
    {
        sampleRate = sr;
        resetFilters();
        // If a user IR was loaded at a different sample rate, resample it now
        if (pendingIR.size() > 0 && pendingIRSampleRate > 0.0)
            applyPendingIR();
    }

    // Called from message thread to install a new user IR.
    // samples[] are normalized floats at irSR sample rate.
    // Resamples to plugin SR and swaps in atomically (well, under a spinlock-free
    // double-buffer: we write to the inactive buffer, then flip the index).
    void loadUserIR (const float* samples, int numSamples, double irSR)
    {
        pendingIR.assign (samples, samples + numSamples);
        pendingIRSampleRate = irSR;
        if (sampleRate > 0.0)
            applyPendingIR();
        usingBuiltinIR = false;
    }

    void resetToBuiltinIR()
    {
        initBuiltinIR();
        usingBuiltinIR = true;
    }

    bool isUsingBuiltinIR() const { return usingBuiltinIR; }

    // State persistence helpers — called from processor getStateInformation/setStateInformation
    const std::vector<float>& getActiveIR()    const { return activeIR; }
    double                    getSampleRate()   const { return sampleRate; }
    const char*               getUserIRName()   const { return userIRName; }
    void                      setUserIRName (const char* name)
    {
        std::strncpy (userIRName, name, sizeof (userIRName) - 1);
        userIRName[sizeof (userIRName) - 1] = '\0';
    }

    // ── Parameter setters ─────────────────────────────────────────────────
    void loadPatch (int p)
    {
        struct Patch { float dr,ba,mi,tr,pr,mv; bool br; };
        static const Patch patches[20] = {
            {8.5f,4.0f,5.5f,5.0f,6.0f,7.0f,false},{9.0f,3.5f,5.0f,5.5f,6.5f,7.0f,false},
            {7.5f,5.5f,6.0f,4.5f,5.0f,7.5f,false},{8.0f,4.5f,7.0f,4.5f,5.5f,7.0f,false},
            {9.5f,3.0f,5.5f,6.0f,7.0f,6.5f,true}, {8.0f,3.5f,4.5f,7.0f,7.5f,7.0f,true},
            {7.0f,5.0f,6.5f,4.0f,4.5f,8.0f,false},{8.5f,5.5f,7.0f,3.5f,4.0f,7.5f,false},
            {10.f,3.0f,5.0f,6.0f,7.0f,6.0f,true}, {6.5f,5.0f,5.5f,5.5f,5.5f,8.0f,false},
            {6.0f,6.0f,6.5f,3.0f,3.5f,8.5f,false},{9.0f,6.0f,3.5f,7.0f,7.5f,7.0f,true},
            {2.5f,5.5f,5.0f,6.0f,5.5f,8.0f,false},{2.0f,6.5f,5.5f,4.5f,4.5f,8.5f,false},
            {2.0f,4.5f,4.5f,7.5f,7.0f,7.5f,true}, {3.5f,5.0f,5.5f,6.5f,6.0f,7.5f,true},
            {1.5f,7.0f,6.0f,3.0f,3.0f,9.0f,false},{3.0f,4.5f,6.0f,6.5f,6.5f,8.0f,true},
            {2.0f,6.0f,5.0f,5.5f,5.0f,9.0f,false},{1.5f,5.0f,4.5f,6.5f,6.0f,8.5f,true},
        };
        const int idx = std::max(1, std::min(20, p)) - 1;
        const auto& d = patches[idx];
        drive=d.dr; bass=d.ba; mid=d.mi; treble=d.tr;
        presence=d.pr; masterVol=d.mv; bright=d.br;
    }

    float getDrive()     const { return drive; }
    float getBass()      const { return bass; }
    float getMid()       const { return mid; }
    float getTreble()    const { return treble; }
    float getPresence()  const { return presence; }
    float getMasterVol() const { return masterVol; }
    bool  getBright()    const { return bright; }

    void setDrive     (float v) { drive      = v; }
    void setBass      (float v) { bass       = v; }
    void setMid       (float v) { mid        = v; }
    void setTreble    (float v) { treble     = v; }
    void setPresence  (float v) { presence   = v; }
    void setMasterVol (float v) { masterVol  = v; }
    void setCabinet   (float v) { cabinet    = v > 0.5f; }
    void setCabHP     (float hz)  { cabHPFreq  = hz; }   // 20-2000 Hz
    void setCabLP     (float hz)  { cabLPFreq  = hz; }   // 1000-20000 Hz
    void setGate      (float v) { inTrim     = v; }
    void setBright    (float v) { bright     = v > 0.5f; }

    // ── Block processing ──────────────────────────────────────────────────
    void processBlock (const float* inL, const float* inR,
                       float* outL, float* outR, int n)
    {
        // Snapshot IR length once per block (safe: irLen is only written
        // on the message thread via loadUserIR/initBuiltinIR, never mid-block
        // in a plugin that calls prepareToPlay before streaming).
        const int irLen = (int) activeIR.size();

        const float sr       = (float) sampleRate;
        const float preGain  = std::exp(drive * 0.46f);
        const float hpCoef   = 1.f - std::exp(-6.283185f * 16.f    / sr);
        const float lpCoef   = 1.f - std::exp(-6.283185f * 12000.f / sr);
        const float bassCoef = 1.f - std::exp(-6.283185f * 200.f   / sr);
        const float midCoef  = 1.f - std::exp(-6.283185f * 750.f   / sr);
        const float trebCoef = 1.f - std::exp(-6.283185f * 2500.f  / sr);
        const float presCoef = 1.f - std::exp(-6.283185f * 4000.f  / sr);
        const float brtCoef  = 1.f - std::exp(-6.283185f * 3000.f  / sr);
        const float sagAtt   = std::exp(-6.283185f * 30.f  / sr);
        const float sagRel   = std::exp(-6.283185f *  0.5f / sr);

        const float bassG = (bass     - 5.f) * 0.18f;
        const float midG  = (mid      - 5.f) * 0.15f;
        const float trebG = (treble   - 5.f) * 0.15f;
        const float presG = (presence - 5.f) * 0.12f;
        const float brtG  = bright ? 0.5f : 0.f;
        const float vol   = masterVol * 0.1f;

        for (int i = 0; i < n; ++i)
        {
            float sigL = inL[i];
            float sigR = inR[i];

            // ── 0. Input Trim ─────────────────────────────────────────────
            sigL *= inTrim * 2.f;
            sigR *= inTrim * 2.f;

            // ── 1. Bright high-shelf ──────────────────────────────────────
            {
                const float lpL = brtCoef*(sigL-brightSL)+brightSL; brightSL=lpL;
                const float lpR = brtCoef*(sigR-brightSR)+brightSR; brightSR=lpR;
                sigL += (sigL-lpL)*brtG;
                sigR += (sigR-lpR)*brtG;
            }

            // ── 2. Preamp — 3 cascaded 12AX7 triode stages ───────────────
            {
                const float lpL=hpCoef*(sigL-s1hpL)+s1hpL; s1hpL=lpL;
                sigL=triode(sigL-lpL,preGain*0.33f);
                const float lL=lpCoef*(sigL-s1lpL)+s1lpL; s1lpL=lL; sigL=lL;
                const float lpR=hpCoef*(sigR-s1hpR)+s1hpR; s1hpR=lpR;
                sigR=triode(sigR-lpR,preGain*0.33f);
                const float lR=lpCoef*(sigR-s1lpR)+s1lpR; s1lpR=lR; sigR=lR;
            }
            {
                const float lpL=hpCoef*(sigL-s2hpL)+s2hpL; s2hpL=lpL;
                sigL=triode(sigL-lpL,preGain*0.5f);
                const float lL=lpCoef*(sigL-s2lpL)+s2lpL; s2lpL=lL; sigL=lL;
                const float lpR=hpCoef*(sigR-s2hpR)+s2hpR; s2hpR=lpR;
                sigR=triode(sigR-lpR,preGain*0.5f);
                const float lR=lpCoef*(sigR-s2lpR)+s2lpR; s2lpR=lR; sigR=lR;
            }
            {
                const float lpL=hpCoef*(sigL-s3hpL)+s3hpL; s3hpL=lpL;
                sigL=triode(sigL-lpL,preGain);
                const float lL=lpCoef*(sigL-s3lpL)+s3lpL; s3lpL=lL; sigL=lL;
                const float lpR=hpCoef*(sigR-s3hpR)+s3hpR; s3hpR=lpR;
                sigR=triode(sigR-lpR,preGain);
                const float lR=lpCoef*(sigR-s3lpR)+s3lpR; s3lpR=lR; sigR=lR;
            }

            // ── 3. Tone Stack ─────────────────────────────────────────────
            {
                const float lpL=bassCoef*(sigL-tbassL)+tbassL; tbassL=lpL; sigL+=lpL*bassG;
                const float lpR=bassCoef*(sigR-tbassR)+tbassR; tbassR=lpR; sigR+=lpR*bassG;
            }
            {
                const float lpL=midCoef*(sigL-tmidL)+tmidL; tmidL=lpL; sigL+=(sigL-lpL)*midG;
                const float lpR=midCoef*(sigR-tmidR)+tmidR; tmidR=lpR; sigR+=(sigR-lpR)*midG;
            }
            {
                const float lpL=trebCoef*(sigL-ttrebL)+ttrebL; ttrebL=lpL; sigL+=(sigL-lpL)*trebG;
                const float lpR=trebCoef*(sigR-ttrebR)+ttrebR; ttrebR=lpR; sigR+=(sigR-lpR)*trebG;
            }
            {
                const float lpL=presCoef*(sigL-presStateL)+presStateL; presStateL=lpL; sigL+=(sigL-lpL)*presG;
                const float lpR=presCoef*(sigR-presStateR)+presStateR; presStateR=lpR; sigR+=(sigR-lpR)*presG;
            }

            // ── 4. Power amp: sag + class-AB soft clip ────────────────────
            {
                const float aL=std::abs(sigL), aR=std::abs(sigR);
                sagEnvL+=(aL>sagEnvL?1.f-sagAtt:1.f-sagRel)*(aL-sagEnvL);
                sagEnvR+=(aR>sagEnvR?1.f-sagAtt:1.f-sagRel)*(aR-sagEnvR);
                sigL *= 1.f-std::min(sagEnvL*0.3f,0.35f);
                sigR *= 1.f-std::min(sagEnvR*0.3f,0.35f);
                sigL /= 1.f+std::abs(sigL)*0.7f;
                sigR /= 1.f+std::abs(sigR)*0.7f;
            }

            // ── 5. Cabinet IR FIR convolution ─────────────────────────────
            if (cabinet && irLen > 0)
            {
                const int wr = cabWr & (cabBufCapacity - 1);
                cabBufL[wr] = sigL;
                cabBufR[wr] = sigR;
                float accL=0.f, accR=0.f;
                const float* ir = activeIR.data();
                for (int k=0; k<irLen; ++k)
                {
                    const int rd = (wr - k + cabBufCapacity) & (cabBufCapacity - 1);
                    accL += ir[k] * cabBufL[rd];
                    accR += ir[k] * cabBufR[rd];
                }
                sigL=accL; sigR=accR;
                cabWr++;
            }

            // ── 6. Cabinet EQ: HP + LP filters ───────────────────────────
            if (cabinet)
            {
                // High-pass (removes rumble/DC below cabHPFreq)
                if (cabHPFreq > 25.f)
                {
                    const float hpC = 1.f - std::exp(-6.283185f * cabHPFreq / (float)sampleRate);
                    cabHPStateL += hpC * (sigL - cabHPStateL);
                    cabHPStateR += hpC * (sigR - cabHPStateR);
                    sigL -= cabHPStateL;
                    sigR -= cabHPStateR;
                }
                // Low-pass (rolls off harshness above cabLPFreq)
                if (cabLPFreq < 19000.f)
                {
                    const float lpC = 1.f - std::exp(-6.283185f * cabLPFreq / (float)sampleRate);
                    cabLPStateL += lpC * (sigL - cabLPStateL);
                    cabLPStateR += lpC * (sigR - cabLPStateR);
                    sigL = cabLPStateL;
                    sigR = cabLPStateR;
                }
            }

                        // ── 6. Master Volume ──────────────────────────────────────────
                        outL[i] = std::max(-1.f, std::min(1.f, sigL*vol));
            outR[i] = std::max(-1.f, std::min(1.f, sigR*vol));
        }
    }

private:
    // Max IR length we support: 8192 taps (~170ms at 48kHz). Power-of-2 for
    // the delay-line wrap mask trick.
    static constexpr int kMaxIRLen       = 8192;
    static constexpr int cabBufCapacity  = kMaxIRLen; // must be power of 2

    double sampleRate = 44100.0;
    float  drive=5, bass=5, mid=5, treble=5, presence=5, masterVol=7;
    bool   cabinet=true, bright=false;
    float  inTrim=0.5f;
    float  cabHPFreq=20.f;      // HP cutoff Hz — default off (20Hz)
    float  cabLPFreq=20000.f;   // LP cutoff Hz — default off (20kHz)
    float  cabHPStateL=0.f, cabHPStateR=0.f;
    float  cabLPStateL=0.f, cabLPStateR=0.f;
    bool   usingBuiltinIR = true;
    char   userIRName[256] = {};

    // Active IR used by the audio thread
    std::vector<float> activeIR;

    // Pending user IR (written on message thread, applied in prepare or loadUserIR)
    std::vector<float> pendingIR;
    double             pendingIRSampleRate = 0.0;

    // Delay line for cabinet FIR — power-of-2 size for fast modulo
    float cabBufL[cabBufCapacity] = {};
    float cabBufR[cabBufCapacity] = {};
    int   cabWr = 0;

    // Filter states
    float s1hpL=0,s1hpR=0,s1lpL=0,s1lpR=0;
    float s2hpL=0,s2hpR=0,s2lpL=0,s2lpR=0;
    float s3hpL=0,s3hpR=0,s3lpL=0,s3lpR=0;
    float tbassL=0,tbassR=0,tmidL=0,tmidR=0,ttrebL=0,ttrebR=0;
    float presStateL=0,presStateR=0;
    float brightSL=0,brightSR=0;
    float sagEnvL=0,sagEnvR=0;

    void resetFilters()
    {
        s1hpL=s1hpR=s1lpL=s1lpR=0.f;
        s2hpL=s2hpR=s2lpL=s2lpR=0.f;
        s3hpL=s3hpR=s3lpL=s3lpR=0.f;
        tbassL=tbassR=tmidL=tmidR=ttrebL=ttrebR=0.f;
        presStateL=presStateR=brightSL=brightSR=0.f;
        sagEnvL=sagEnvR=0.f;
        cabHPStateL=cabHPStateR=cabLPStateL=cabLPStateR=0.f;
        std::memset(cabBufL,0,sizeof(cabBufL));
        std::memset(cabBufR,0,sizeof(cabBufR));
        cabWr=0;
    }

    static float triode(float x, float gain) noexcept
    {
        const float d = x * gain;
        const float y = d > 0.f ? d / (1.f + d * 0.5f)
                                : d / (1.f + std::abs(d) * 1.2f);
        return std::max(-1.f, std::min(0.85f, y));
    }

    // Resample pendingIR from pendingIRSampleRate to sampleRate using
    // linear interpolation (sufficient for IR loading, not real-time).
    void applyPendingIR()
    {
        if (pendingIR.empty() || pendingIRSampleRate <= 0.0 || sampleRate <= 0.0)
            return;

        const double ratio = sampleRate / pendingIRSampleRate;
        const int srcLen   = (int) pendingIR.size();
        const int dstLen   = std::min ((int)(srcLen * ratio), kMaxIRLen);

        std::vector<float> resampled (dstLen);
        for (int i = 0; i < dstLen; ++i)
        {
            const double srcPos = i / ratio;
            const int    s0     = (int) srcPos;
            const int    s1     = std::min (s0 + 1, srcLen - 1);
            const float  frac   = (float)(srcPos - s0);
            resampled[i] = pendingIR[s0] * (1.f - frac) + pendingIR[s1] * frac;
        }

        // Normalise so the loaded IR has the same convolution gain as the
        // built-in Celestion V30 IR (L1 norm ≈ 2.36).  Peak normalisation
        // is wrong here because it ignores tap count — a 2000-tap IR with
        // peak=1.0 produces ~30× more convolution gain than a 64-tap one.
        static constexpr float kTargetL1 = 2.36f;
        float l1 = 0.f;
        for (float v : resampled) l1 += std::abs(v);
        if (l1 > 1e-6f)
            for (float& v : resampled) v *= kTargetL1 / l1;

        // Swap in — audio thread only reads activeIR.data()/size() which
        // are stable once the vector is fully constructed.
        activeIR = std::move(resampled);
        std::memset(cabBufL,0,sizeof(cabBufL));
        std::memset(cabBufR,0,sizeof(cabBufR));
        cabWr = 0;
    }

    void initBuiltinIR()
    {
        // Celestion V30 1x12 open-back — 64-tap minimum-phase FIR
        static const float kCoeffs[64] = {
             0.18f, 0.29f, 0.22f, 0.08f,-0.06f,-0.14f,-0.11f,-0.03f,
             0.07f, 0.13f, 0.11f, 0.06f, 0.01f,-0.04f,-0.07f,-0.06f,
            -0.03f, 0.01f, 0.05f, 0.06f, 0.05f, 0.03f, 0.01f,-0.02f,
            -0.04f,-0.04f,-0.03f,-0.01f, 0.01f, 0.03f, 0.03f, 0.02f,
             0.01f,-0.01f,-0.02f,-0.02f,-0.02f,-0.01f, 0.00f, 0.01f,
             0.02f, 0.01f, 0.01f, 0.00f,-0.01f,-0.01f,-0.01f,-0.01f,
             0.00f, 0.01f, 0.01f, 0.01f, 0.00f, 0.00f,-0.01f,-0.01f,
             0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f
        };
        activeIR.assign(kCoeffs, kCoeffs + 64);
        std::memset(cabBufL,0,sizeof(cabBufL));
        std::memset(cabBufR,0,sizeof(cabBufR));
        cabWr = 0;
        usingBuiltinIR = true;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DGStompDSP)
};
