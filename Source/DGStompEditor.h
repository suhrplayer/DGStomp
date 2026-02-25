#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "DGStompProcessor.h"


class DGStompEditor : public juce::AudioProcessorEditor,
                      private juce::AudioProcessorValueTreeState::Listener,
                      private juce::AsyncUpdater
{
public:
    explicit DGStompEditor (DGStompProcessor& p)
        : AudioProcessorEditor (p), processor (p), webView (makeWebView())
    {
        setSize (900, 640);
        setResizable (true, true);
        setResizeLimits (720, 520, 1400, 1050);
        addAndMakeVisible (webView);

        for (auto* id : kParamIDs)
            processor.apvts.addParameterListener (id, this);

        webView.goToURL (juce::WebBrowserComponent::getResourceProviderRoot());
    }

    ~DGStompEditor() override
    {
        cancelPendingUpdate();
        for (auto* id : kParamIDs)
            processor.apvts.removeParameterListener (id, this);
    }

    void resized() override { webView.setBounds (getLocalBounds()); }
    void paint (juce::Graphics& g) override { g.fillAll (juce::Colour (0x1c1e22)); }

private:
    static constexpr const char* kParamIDs[] = {
        "param1","param2","param3","param4","param5",
        "param6","param7","param8","param9","param10"
    };

    // ── Lock-free queue: audio/message thread → async updater ─────────────
    struct ParamChange { char id[16]; float value; };
    static constexpr int kQueueSize = 128;
    ParamChange queue[kQueueSize];
    std::atomic<int> qHead { 0 };
    std::atomic<int> qTail { 0 };

    // Suppress echoing changes that originated from the JS UI itself.
    // When JS calls sendParam, we set this flag so parameterChanged
    // won't re-emit the same value back to JS (infinite loop).
    std::atomic<bool> suppressNextEmit { false };

    void parameterChanged (const juce::String& paramID, float newValue) override
    {
        // If this change was initiated by the JS UI via sendParam, skip it.
        // The JS already knows the new value — no need to echo it back.
        if (suppressNextEmit.exchange (false))
            return;

        // Otherwise it came from the host (automation, preset load) — push to JS.
        const int head = qHead.load (std::memory_order_relaxed);
        const int next = (head + 1) % kQueueSize;
        if (next != qTail.load (std::memory_order_acquire))
        {
            auto& slot = queue[head];
            paramID.copyToUTF8 (slot.id, sizeof (slot.id));
            slot.value = newValue;
            qHead.store (next, std::memory_order_release);
        }
        triggerAsyncUpdate();
    }

    void handleAsyncUpdate() override
    {
        int tail = qTail.load (std::memory_order_acquire);
        const int head = qHead.load (std::memory_order_acquire);
        while (tail != head)
        {
            const auto& slot = queue[tail];
            juce::DynamicObject::Ptr obj = new juce::DynamicObject();
            obj->setProperty ("id",    juce::String (slot.id));
            obj->setProperty ("value", slot.value);
            webView.emitEventIfBrowserIsVisible ("paramChanged", juce::var (obj.get()));
            tail = (tail + 1) % kQueueSize;
        }
        qTail.store (tail, std::memory_order_release);
    }

    // ── WebView ───────────────────────────────────────────────────────────
    juce::WebBrowserComponent makeWebView()
    {
        using WBC = juce::WebBrowserComponent;
        return WBC (WBC::Options{}
            .withNativeIntegrationEnabled()
            .withNativeFunction ("sendParam",
                [this] (const juce::Array<juce::var>& args, WBC::NativeFunctionCompletion done)
                {
                    if (args.size() >= 2)
                    {
                        const auto id   = args[0].toString();
                        const float val = (float) args[1];
                        // JS sends raw parameter values (same range as Cmajor receives).
                        // JUCE needs them normalized 0-1 via convertTo0to1.
                        suppressNextEmit.store (true);
                        if (auto* param = processor.apvts.getParameter (id))
                            param->setValueNotifyingHost (param->convertTo0to1 (val));
                    }
                    done ({});
                })
            .withNativeFunction ("getParam",
                [this] (const juce::Array<juce::var>& args, WBC::NativeFunctionCompletion done)
                {
                    float val = 0.f;
                    if (args.size() >= 1)
                        if (auto* v = processor.apvts.getRawParameterValue (args[0].toString()))
                            val = v->load();   // raw value — JS fromVal() scales it to 0-1
                    done (val);
                })
            // loadCabIR(base64wav) — JS sends base64-encoded WAV bytes,
            // decoded and loaded directly into the DSP here in the editor.
            .withNativeFunction ("loadCabIR",
                [this] (const juce::Array<juce::var>& args, WBC::NativeFunctionCompletion done)
                {
                    if (args.size() < 1) { done (juce::var("No data")); return; }

                    const auto b64 = args[0].toString();
                    juce::MemoryOutputStream mos;
                    if (!juce::Base64::convertFromBase64 (mos, b64))
                    {
                        done (juce::var("Base64 decode failed"));
                        return;
                    }

                    // Wrap decoded bytes and try to create a WAV/AIFF reader
                    auto* mis = new juce::MemoryInputStream (
                        mos.getData(), mos.getDataSize(), false);
                    std::unique_ptr<juce::AudioFormatReader> reader (
                        processor.formatManager.createReaderFor (
                            std::unique_ptr<juce::InputStream> (mis)));
                    if (reader == nullptr)
                    {
                        done (juce::var("Unsupported format or corrupt file"));
                        return;
                    }

                    const int numSamples = (int) reader->lengthInSamples;
                    if (numSamples <= 0 || numSamples > 1'000'000)
                    {
                        done (juce::var("IR too long (max ~20s) or empty"));
                        return;
                    }

                    // Decode to float (mono-mix stereo if needed)
                    juce::AudioBuffer<float> buf (1, numSamples);
                    if (reader->numChannels >= 2)
                    {
                        juce::AudioBuffer<float> stereo (2, numSamples);
                        reader->read (&stereo, 0, numSamples, 0, true, true);
                        buf.clear();
                        for (int i = 0; i < numSamples; ++i)
                            buf.setSample (0, i,
                                (stereo.getSample(0,i) + stereo.getSample(1,i)) * 0.5f);
                    }
                    else
                    {
                        reader->read (&buf, 0, numSamples, 0, true, false);
                    }
                    processor.dsp.loadUserIR (
                        buf.getReadPointer(0), numSamples, reader->sampleRate);
                    // Store the filename for state persistence (passed as second arg)
                    if (args.size() >= 2)
                        processor.dsp.setUserIRName (args[1].toString().toRawUTF8());
                    done (juce::var("ok"));
                })
            // resetCabIR() — revert to built-in Celestion V30 IR
            .withNativeFunction ("resetCabIR",
                [this] (const juce::Array<juce::var>&, WBC::NativeFunctionCompletion done)
                {
                    processor.dsp.resetToBuiltinIR();
                    processor.dsp.setUserIRName ("");
                    done (juce::var("ok"));
                })
            // setCabHP(hz) — cabinet high-pass cutoff in Hz
            .withNativeFunction ("setCabHP",
                [this] (const juce::Array<juce::var>& args, WBC::NativeFunctionCompletion done)
                {
                    if (args.size() >= 1)
                        processor.dsp.setCabHP (std::max(20.f, std::min(2000.f, (float)args[0])));
                    done ({});
                })
            // setCabLP(hz) — cabinet low-pass cutoff in Hz
            .withNativeFunction ("setCabLP",
                [this] (const juce::Array<juce::var>& args, WBC::NativeFunctionCompletion done)
                {
                    if (args.size() >= 1)
                        processor.dsp.setCabLP (std::max(1000.f, std::min(20000.f, (float)args[0])));
                    done ({});
                })
            // getIRInfo() — returns {name, builtin} so JS can restore IR display on editor open
            .withNativeFunction ("getIRInfo",
                [this] (const juce::Array<juce::var>&, WBC::NativeFunctionCompletion done)
                {
                    juce::DynamicObject::Ptr obj = new juce::DynamicObject();
                    obj->setProperty ("name",    juce::String (processor.dsp.getUserIRName()));
                    obj->setProperty ("builtin", processor.dsp.isUsingBuiltinIR());
                    done (juce::var (obj.get()));
                })
            .withResourceProvider (
                [this] (const juce::String& path) -> std::optional<WBC::Resource>
                { return serveResource (path); })
        );
    }

    std::optional<juce::WebBrowserComponent::Resource>
    serveResource (const juce::String& path)
    {
        if (path == "/" || path == "/index.html")
        {
            auto html = buildHTML();
            return makeResource (html.toRawUTF8(), (size_t) html.getNumBytesAsUTF8(), "text/html");
        }
        if (path == "/DGStompUI.js")
        {
            auto f = getJSFile();
            if (f.existsAsFile())
            {
                juce::MemoryBlock mb;
                f.loadFileAsData (mb);
                return makeResource ((const char*) mb.getData(), mb.getSize(), "application/javascript");
            }
        }
        return {};
    }

    static juce::WebBrowserComponent::Resource
    makeResource (const char* data, size_t size, const char* mime)
    {
        std::vector<std::byte> bytes (size);
        std::memcpy (bytes.data(), data, size);
        return { std::move (bytes), mime };
    }

    static juce::File getJSFile()
    {
        auto app = juce::File::getSpecialLocation (juce::File::currentApplicationFile);
#if JUCE_MAC
        auto res = app.getChildFile ("Contents/Resources/DGStompUI.js");
        if (res.existsAsFile()) return res;
#endif
        return app.getParentDirectory().getChildFile ("DGStompUI.js");
    }

    juce::String buildHTML()
    {
        juce::String init = "{";
        for (int i = 0; i < 10; ++i)
        {
            float val = 0.f;
            if (auto* v = processor.apvts.getRawParameterValue (kParamIDs[i]))
                val = v->load();
            init += juce::String ("\"") + kParamIDs[i] + "\":"
                  + juce::String (val, 4);
            if (i < 9) init += ",";
        }
        init += "}";

        return R"(<!DOCTYPE html>
<html>
<head><meta charset="UTF-8">
<style>
  *{margin:0;padding:0;box-sizing:border-box;}
  html,body{width:100%;height:100%;overflow:hidden;background:#1c1e22;}
  #err{display:none;position:fixed;inset:0;background:#1a0000;color:#ff6666;
       font:13px/1.6 monospace;padding:20px;white-space:pre-wrap;overflow:auto;z-index:9999;}
</style>
</head>
<body>
<div id="err"></div>
<script>
window.onerror=function(m,s,l){var d=document.getElementById('err');d.style.display='block';d.textContent+='ERROR: '+m+'\n  at '+s+':'+l+'\n\n';};
window.addEventListener('unhandledrejection',function(e){var d=document.getElementById('err');d.style.display='block';d.textContent+='REJECTED: '+(e.reason&&(e.reason.stack||e.reason))+'\n\n';});
</script>
<script>
var _init=)" + init + R"(;
var _lpid=0,_pmap=new Map();
window.__JUCE__.backend.addEventListener('__juce__complete',function(d){if(_pmap.has(d.promiseId)){_pmap.get(d.promiseId)(d.result);_pmap.delete(d.promiseId);}});
function getNativeFunction(name){return function(){var id=_lpid++;var p=new Promise(function(r){_pmap.set(id,r);});window.__JUCE__.backend.emitEvent('__juce__invoke',{name:name,params:Array.from(arguments),resultId:id});return p;};}
var _sendParam=getNativeFunction('sendParam');
var _getParam=getNativeFunction('getParam');
window._loadCabIR=getNativeFunction('loadCabIR');
window._resetCabIR=getNativeFunction('resetCabIR');
window._setCabHP=getNativeFunction('setCabHP');
window._setCabLP=getNativeFunction('setCabLP');
window._getIRInfo=getNativeFunction('getIRInfo');
var patchConnection={
  _listeners:[],
  sendEventOrValue:function(id,v){_sendParam(id,v);},
  requestParameterValue:function(id){_getParam(id).then(function(v){patchConnection._dispatch(id,v!=null?v:(_init[id]||0));}).catch(function(){patchConnection._dispatch(id,_init[id]||0);});},
  addAllParameterListener:function(cb){this._listeners.push(cb);},
  removeAllParameterListener:function(cb){this._listeners=this._listeners.filter(function(l){return l!==cb;});},
  _dispatch:function(id,value){this._listeners.forEach(function(l){l({endpointID:id,value:value});});}
};
window.__JUCE__.backend.addEventListener('paramChanged',function(e){if(e&&e.id!==undefined)patchConnection._dispatch(e.id,e.value);});
var jsUrl='juce://juce.backend/DGStompUI.js';
import(jsUrl).then(function(mod){document.body.appendChild(mod.default(patchConnection));}).catch(function(e){var d=document.getElementById('err');d.style.display='block';d.textContent+='IMPORT ERROR: '+(e.stack||e)+'\n\nURL: '+jsUrl+'\n\n';});
</script>
</body>
</html>
)";
    }

    DGStompProcessor&          processor;
    juce::WebBrowserComponent  webView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DGStompEditor)
};
