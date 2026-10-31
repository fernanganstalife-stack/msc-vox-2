#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"
#include "VisualizerComponents.h"
#include "AnimationEngine.h"

class MSCVoxityV2AudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    MSCVoxityV2AudioProcessorEditor (MSCVoxityV2AudioProcessor&);
    ~MSCVoxityV2AudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    MSCVoxityV2AudioProcessor& audioProcessor;
    AnimationEngine animationEngine;
    VisualizerComponent mainVisualizer;

    juce::ComboBox presetBox;
    juce::TextButton saveButton{"SAVE"};
    juce::TextButton loadButton{"LOAD"};
    juce::ComboBox themeBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MSCVoxityV2AudioProcessorEditor)
};
