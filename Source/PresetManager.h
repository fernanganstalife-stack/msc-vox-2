#pragma once
#include <juce_core/juce_core.h>

class PresetManager
{
public:
    PresetManager()
    {
        presets = {
            "01 - MySpace Dream",
            "02 - Destroyed Dream",
            "03 - Chorus Hell",
            "04 - Vampire Love",
            "05 - Angel Voice",
            "06 - Everything Broken",
            "07 - Raw Underground",
            "08 - MySpaceCore"
        };
    }

    const juce::StringArray& getPresets() const { return presets; }
    juce::String getCurrentPresetName() const { return currentPresetName; }
    void setCurrentPreset(const juce::String& name) { currentPresetName = name; }

private:
    juce::StringArray presets;
    juce::String currentPresetName = "08 - MySpaceCore";
};
