#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_graphics/juce_gui_graphics.h>

class VisualizerComponent : public juce::Component, public juce::Timer
{
public:
    VisualizerComponent() { startTimerHz(30); }
    ~VisualizerComponent() override { stopTimer(); }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black.withAlpha(0.6f));
        g.setColour(juce::Colour(0xffb026ff)); // Neon purple accent
        
        juciePath.clear();
        float width = (float)getWidth();
        float height = (float)getHeight();
        
        juciePath.startNewSubPath(0, height * 0.5f);
        for (int i = 0; i < width; i += 4)
        {
            float y = height * 0.5f + std::sin(i * 0.05f + phase) * (height * 0.3f);
            juciePath.lineTo((float)i, y);
        }
        g.strokePath(juciePath, juce::PathStrokeType(1.5f));
    }

    void timerCallback() override
    {
        phase += 0.2f;
        repaint();
    }

private:
    juce::Path juciePath;
    float phase = 0.0f;
};
