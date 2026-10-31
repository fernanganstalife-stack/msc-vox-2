
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

#include <atomic>
#include <memory>
#include <vector>

#include "DSPModules.h"
#include "PresetManager.h"

class MSCVoxityV2AudioProcessor : public juce::AudioProcessor
{
public:
    MSCVoxityV2AudioProcessor();
    ~MSCVoxityV2AudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(
        juce::AudioBuffer<float>& buffer,
        juce::MidiBuffer& midiMessages) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;

    const juce::String getProgramName(int index) override;
    void changeProgramName(
        int index,
        const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;

    void setStateInformation(
        const void* data,
        int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getAPVTS()
    {
        return apvts;
    }

    PresetManager& getPresetManager()
    {
        return presetManager;
    }

    // Real-time visualizer data getters

    float getInputMeterLevel() const
    {
        return inputMeterLevel.load();
    }

    float getOutputMeterLevel() const
    {
        return outputMeterLevel.load();
    }

    float getCompressorGR() const
    {
        return compGR.load();
    }

    float getDeEsserGR() const
    {
        return deEsserGR.load();
    }

    float getCurrentDetectedPitch() const
    {
        return detectedPitch.load();
    }

private:
    juce::AudioProcessorValueTreeState apvts;

    juce::AudioProcessorValueTreeState::ParameterLayout
        createParameterLayout();

    PresetManager presetManager;

    // DSP Chains & Modules

    AutoTuneModule autoTuneModule;
    CompressorModule compressorModule;
    DeEsserModule deEsserModule;
    EQModule eqModule;
    SaturationModule saturationModule;
    ChorusModule chorusModule;
    StereoWidenerModule stereoWidenerModule;
    ReverbModule reverbModule;
    DelayModule delayModule;

    // Metering data

    std::atomic<float> inputMeterLevel{ 0.0f };
    std::atomic<float> outputMeterLevel{ 0.0f };
    std::atomic<float> compGR{ 0.0f };
    std::atomic<float> deEsserGR{ 0.0f };
    std::atomic<float> detectedPitch{ 440.0f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        MSCVoxityV2AudioProcessor)
};
