#include "PluginProcessor.h"
#include "PluginEditor.h"

MSCVoxityV2AudioProcessorEditor::MSCVoxityV2AudioProcessorEditor (MSCVoxityV2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (1024, 720);
    startTimerHz(60);

    addAndMakeVisible(mainVisualizer);
    
    presetBox.addItem("01 - MySpace Dream", 1);
    presetBox.addItem("02 - Destroyed Dream", 2);
    presetBox.addItem("03 - Chorus Hell", 3);
    presetBox.addItem("04 - Vampire Love", 4);
    presetBox.addItem("05 - Angel Voice", 5);
    presetBox.addItem("06 - Everything Broken", 6);
    presetBox.addItem("07 - Raw Underground", 7);
    presetBox.addItem("08 - MySpaceCore", 8);
    presetBox.setSelectedId(8);
    addAndMakeVisible(presetBox);

    addAndMakeVisible(saveButton);
    addAndMakeVisible(loadButton);

    themeBox.addItem("Neon Purple", 1);
    themeBox.addItem("Cyber Pink", 2);
    themeBox.addItem("Underground Green", 3);
    themeBox.setSelectedId(1);
    addAndMakeVisible(themeBox);

    presetBox.onChange = [this] {
        animationEngine.triggerPresetChangeAnimation();
    };
}

MSCVoxityV2AudioProcessorEditor::~MSCVoxityV2AudioProcessorEditor()
{
    stopTimer();
}

void MSCVoxityV2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // Deep dark MySpace core background styling matching the reference image
    g.fillAll (juce::Colour(0xff0d0a14));

    // Top header bar
    g.setColour(juce::Colour(0xff181226));
    g.fillRect(0, 0, getWidth(), 60);

    // Title branding
    g.setColour(juce::Colour(0xffb026ff));
    g.setFont(juce::Font("Impact", 28.0f, juce::Font::bold));
    g.drawText("MSC VOXITY V2", 20, 15, 200, 30, juce::Justification::left);

    // Draw animated accent glow based on animation engine
    float glowAlpha = 0.2f + (animationEngine.getAnimationPhase() * 0.8f);
    g.setColour(juce::Colour(0xffff007f).withAlpha(glowAlpha));
    g.drawRect(getLocalBounds(), 2);
}

void MSCVoxityV2AudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    auto topBar = area.removeFromTop(60);

    presetBox.setBounds(topBar.removeFromLeft(220).reduced(15, 15));
    saveButton.setBounds(topBar.removeFromLeft(70).reduced(10, 15));
    loadButton.setBounds(topBar.removeFromLeft(70).reduced(10, 15));

    themeBox.setBounds(topBar.removeFromRight(150).reduced(10, 15));
    
    mainVisualizer.setBounds(area.removeFromTop(120).reduced(20));
}

void MSCVoxityV2AudioProcessorEditor::timerCallback()
{
    repaint();
}
