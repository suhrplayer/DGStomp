#include "DGStompProcessor.h"
#include "DGStompEditor.h"

static const char* kProgramNames[20] = {
    "Lead 1 - Main Lead",    "Lead 2 - Sustain Lead",   "Lead 3 - Warm Lead",
    "Lead 4 - Singing Lead", "Lead 5 - Compressed Lead","Lead 6 - Bright Lead",
    "Lead 7 - Mellow Lead",  "Lead 8 - Creamy Lead",    "Lead 9 - Solo Lead",
    "Lead 10 - Natural Lead","Lead 11 - Jazz Lead",     "Lead 12 - Modern Lead",
    "Clean 1 - Main Clean",  "Clean 2 - Warm Clean",    "Clean 3 - Bright Clean",
    "Clean 4 - Comp Clean",  "Clean 5 - Jazz Clean",    "Clean 6 - Chimey Clean",
    "Clean 7 - Full Clean",  "Clean 8 - Glassy Clean"
};

juce::AudioProcessorValueTreeState::ParameterLayout DGStompProcessor::createLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add (std::make_unique<juce::AudioParameterFloat> ("param1",  "Patch",       juce::NormalisableRange<float>(1,20,1),  1));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("param2",  "Drive",       juce::NormalisableRange<float>(0,10),    5));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("param3",  "Bass",        juce::NormalisableRange<float>(0,10),    5));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("param4",  "Middle",      juce::NormalisableRange<float>(0,10),    5));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("param5",  "Treble",      juce::NormalisableRange<float>(0,10),    5));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("param6",  "Presence",    juce::NormalisableRange<float>(0,10),    5));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("param7",  "Master Vol",  juce::NormalisableRange<float>(0,10),    7));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("param8",  "Cabinet Sim", juce::NormalisableRange<float>(0,1,1),   1));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("param9",  "In Trim",     juce::NormalisableRange<float>(0,1),     0.5f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("param10", "Bright",      juce::NormalisableRange<float>(0,1,1),   0));
    return layout;
}

DGStompProcessor::DGStompProcessor()
    : AudioProcessor (BusesProperties()
                        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "DGStomp", createLayout())
{
    formatManager.registerBasicFormats();
    formatManager.registerBasicFormats();
    apvts.addParameterListener ("param1", this);
    dsp.loadPatch (1);
}

DGStompProcessor::~DGStompProcessor()
{
    apvts.removeParameterListener ("param1", this);
}

void DGStompProcessor::parameterChanged (const juce::String& paramID, float newValue)
{
    if (paramID == "param1")
    {
        const int patchNum = juce::jlimit (1, 20, (int) newValue);
        dsp.loadPatch (patchNum);
        struct KV { const char* id; float val; };
        const KV updates[] = {
            {"param2",  dsp.getDrive()              },
            {"param3",  dsp.getBass()               },
            {"param4",  dsp.getMid()                },
            {"param5",  dsp.getTreble()             },
            {"param6",  dsp.getPresence()           },
            {"param7",  dsp.getMasterVol()          },
            {"param10", dsp.getBright() ? 1.f : 0.f},
        };
        for (auto& kv : updates)
            if (auto* p = apvts.getParameter (kv.id))
                p->setValueNotifyingHost (p->convertTo0to1 (kv.val));
    }
}

void DGStompProcessor::prepareToPlay (double sr, int)
{
    dsp.prepare (sr);
}

void DGStompProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    const int numSamples  = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();
    if (numSamples == 0 || numChannels == 0) return;

    dsp.setDrive     (apvts.getRawParameterValue ("param2") ->load());
    dsp.setBass      (apvts.getRawParameterValue ("param3") ->load());
    dsp.setMid       (apvts.getRawParameterValue ("param4") ->load());
    dsp.setTreble    (apvts.getRawParameterValue ("param5") ->load());
    dsp.setPresence  (apvts.getRawParameterValue ("param6") ->load());
    dsp.setMasterVol (apvts.getRawParameterValue ("param7") ->load());
    dsp.setCabinet   (apvts.getRawParameterValue ("param8") ->load());
    dsp.setGate      (apvts.getRawParameterValue ("param9") ->load());
    dsp.setBright    (apvts.getRawParameterValue ("param10")->load());
    float* ch0 = buffer.getWritePointer (0);
    float* ch1 = numChannels > 1 ? buffer.getWritePointer (1) : ch0;
    dsp.processBlock (ch0, ch1, ch0, ch1, numSamples);
}

juce::AudioProcessorEditor* DGStompProcessor::createEditor()
{
    return new DGStompEditor (*this);
}

void DGStompProcessor::setCurrentProgram (int index)
{
    currentProgram = juce::jlimit (0, 19, index);
    if (auto* p = apvts.getParameter ("param1"))
        p->setValueNotifyingHost (p->convertTo0to1 ((float)(currentProgram + 1)));
}

const juce::String DGStompProcessor::getProgramName (int index)
{
    return index >= 0 && index < 20 ? kProgramNames[index] : "Unknown";
}

void DGStompProcessor::getStateInformation (juce::MemoryBlock& d)
{
    auto state = apvts.copyState();

    // Persist the loaded cabinet IR (if user has loaded one).
    // We store the raw float samples as base64 under a child element so the
    // APVTS XML structure is unchanged and backward-compatible.
    if (!dsp.isUsingBuiltinIR())
    {
        const auto& ir = dsp.getActiveIR();
        if (!ir.empty())
        {
            // Pack floats → raw bytes → base64
            juce::MemoryBlock irBytes (ir.data(), ir.size() * sizeof (float));
            auto irB64 = juce::Base64::toBase64 (irBytes.getData(), irBytes.getSize());

            auto irElem = state.getOrCreateChildWithName ("CabIR", nullptr);
            irElem.setProperty ("samples", irB64, nullptr);
            irElem.setProperty ("sampleRate", dsp.getSampleRate(), nullptr);
            irElem.setProperty ("name", juce::String (dsp.getUserIRName()), nullptr);
        }
    }

    if (auto xml = state.createXml())
        copyXmlToBinary (*xml, d);
}

void DGStompProcessor::setStateInformation (const void* d, int size)
{
    if (auto xml = getXmlFromBinary (d, size))
    {
        auto state = juce::ValueTree::fromXml (*xml);
        apvts.replaceState (state);

        // Restore cabinet IR if one was saved
        auto irElem = state.getChildWithName ("CabIR");
        if (irElem.isValid())
        {
            const auto b64 = irElem["samples"].toString();
            const double irSR = (double) irElem["sampleRate"];
            const auto name = irElem["name"].toString();

            juce::MemoryOutputStream mos;
            if (juce::Base64::convertFromBase64 (mos, b64) && mos.getDataSize() > 0)
            {
                const int numSamples = (int) (mos.getDataSize() / sizeof (float));
                const float* samples = reinterpret_cast<const float*> (mos.getData());
                dsp.loadUserIR (samples, numSamples, irSR > 0.0 ? irSR : 48000.0);
                dsp.setUserIRName (name.toRawUTF8());
            }
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new DGStompProcessor(); }
