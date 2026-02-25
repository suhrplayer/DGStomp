#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include "DGStompDSP.h"

class DGStompProcessor : public juce::AudioProcessor,
                         private juce::AudioProcessorValueTreeState::Listener
{
public:
    juce::AudioProcessorValueTreeState apvts;

    DGStompProcessor();
    ~DGStompProcessor() override;

    void prepareToPlay (double sr, int blockSize) override;
    void releaseResources() override {}
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    // formatManager is public so the editor can create readers for user IR files
    juce::AudioFormatManager formatManager;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
        const auto& out = layouts.getMainOutputChannelSet();
        if (out != juce::AudioChannelSet::mono() &&
            out != juce::AudioChannelSet::stereo())
            return false;
        const auto& in = layouts.getMainInputChannelSet();
        if (!in.isDisabled() &&
            in != juce::AudioChannelSet::mono() &&
            in != juce::AudioChannelSet::stereo())
            return false;
        return true;
    }

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "DG Stomp"; }
    bool acceptsMidi()  const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int  getNumPrograms()    override { return 20; }
    int  getCurrentProgram() override { return currentProgram; }
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& d) override;
    void setStateInformation (const void* d, int size) override;

    static juce::AudioProcessorValueTreeState::ParameterLayout createLayout();

    DGStompDSP dsp;

private:
    void parameterChanged (const juce::String& paramID, float newValue) override;

    int currentProgram = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DGStompProcessor)
};
