
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <cmath>

class VisualizerComponent : public juce::Component,
                            public juce::Timer
{
public:
    VisualizerComponent()
    {
        startTimerHz(30);
    }

    ~VisualizerComponent() override
    {
        stopTimer();
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black.withAlpha(0.6f));

        g.setColour(juce::Colour(0xffb026ff));

        wavePath.clear();

        const float width = static_cast<float>(getWidth());
        const float height = static_cast<float>(getHeight());

        wavePath.startNewSubPath(0.0f, height * 0.5f);

        for (int i = 0; i < getWidth(); i += 4)
        {
            const float x = static_cast<float>(i);

            const float y = height * 0.5f
                + std::sin(x * 0.05f + phase) * (height * 0.3f);

            wavePath.lineTo(x, y);
        }

        g.strokePath(
            wavePath,
            juce::PathStrokeType(1.5f)
        );
    }

    void timerCallback() override
    {
        phase += 0.2f;
        repaint();
    }

private:
    juce::Path wavePath;
    float phase = 0.0f;
};
