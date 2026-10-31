#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class AnimationEngine : public juce::Timer
{
public:
    AnimationEngine() { startTimerHz(60); }
    ~AnimationEngine() override { stopTimer(); }

    void triggerPresetChangeAnimation()
    {
        animationPhase = 1.0f; // Triggers fire, sparks, and energetic character bounce
    }

    float getAnimationPhase() const { return animationPhase; }

    void timerCallback() override
    {
        if (animationPhase > 0.0f)
        {
            animationPhase -= 0.02f; // Smooth decay back to idle
            if (animationPhase < 0.0f) animationPhase = 0.0f;
        }
        idleCounter += 0.05f;
    }

    float getIdleOffset() const { return std::sin(idleCounter) * 3.0f; }

private:
    float animationPhase = 0.0f;
    float idleCounter = 0.0f;
};
