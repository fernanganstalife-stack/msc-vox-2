
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <cmath>

class AutoTuneModule
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
    }

    void process(juce::AudioBuffer<float>& buffer, bool enabled,
                 int scale, int note, float retuneSpeed,
                 float amount, int mode)
    {
        if (!enabled)
            return;

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* data = buffer.getWritePointer(channel);

            for (int i = 0; i < buffer.getNumSamples(); ++i)
                data[i] *= (1.0f - amount * 0.1f);
        }
    }

private:
    double sampleRate = 44100.0;
};

class CompressorModule
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        compressor.prepare(spec);
    }

    void setParameters(float thresh, float ratio, float release,
                       float makeup, bool enabled)
    {
        compressor.setThreshold(thresh);
        compressor.setRatio(ratio);
        compressor.setRelease(release);

        makeupGain = makeup;
        isActive = enabled;
    }

    void process(juce::AudioBuffer<float>& buffer, float& grOut)
    {
        grOut = 0.0f;

        if (!isActive)
            return;

        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);

        compressor.process(context);

        buffer.applyGain(juce::Decibels::decibelsToGain(makeupGain));
    }

private:
    juce::dsp::Compressor<float> compressor;
    float makeupGain = 0.0f;
    bool isActive = true;
};

class DeEsserModule
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        filter.prepare(spec);
    }

    void process(juce::AudioBuffer<float>& buffer, float thresh,
                 float freq, float amount, bool enabled, float& grOut)
    {
        grOut = 0.0f;

        if (!enabled)
            return;

        grOut = 0.5f;
    }

private:
    juce::dsp::ProcessorDuplicator<
        juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Coefficients<float>> filter;
};

class EQModule
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        chain.prepare(spec);
    }

    void process(juce::AudioBuffer<float>& buffer, float low,
                 float mid, float high, bool enabled)
    {
        if (!enabled)
            return;
    }

private:
    juce::dsp::ProcessorChain<
        juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Filter<float>> chain;
};

class SaturationModule
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        waveShaper.prepare(spec);
    }

    void process(juce::AudioBuffer<float>& buffer, float drive,
                 float mix, bool enabled)
    {
        if (!enabled)
            return;

        waveShaper.functionToUse = [](float x)
        {
            return std::tanh(x * 1.5f);
        };

        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        waveShaper.process(context);
    }

private:
    juce::dsp::WaveShaper<float> waveShaper;
};

class ChorusModule
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        chorus.prepare(spec);
    }

    void process(juce::AudioBuffer<float>& buffer, float rate,
                 float depth, float mix, bool enabled)
    {
        if (!enabled)
            return;

        chorus.setRate(rate);
        chorus.setDepth(depth);

        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        chorus.process(context);
    }

private:
    juce::dsp::Chorus<float> chorus;
};

class StereoWidenerModule
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec) {}

    void process(juce::AudioBuffer<float>& buffer, float width,
                 float mix, bool enabled)
    {
        if (!enabled || buffer.getNumChannels() < 2)
            return;

        auto* left = buffer.getWritePointer(0);
        auto* right = buffer.getWritePointer(1);

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float m = 0.5f * (left[i] + right[i]);
            float s = 0.5f * (left[i] - right[i]) * width;

            left[i] = m + s;
            right[i] = m - s;
        }
    }
};

class ReverbModule
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        reverb.prepare(spec);
    }

    void process(juce::AudioBuffer<float>& buffer, float size,
                 float decay, float damping, float mix, bool enabled)
    {
        if (!enabled)
            return;

        juce::dsp::Reverb::Parameters params;
        params.roomSize = size;
        params.damping = damping;

        reverb.setParameters(params);

        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        reverb.process(context);
    }

private:
    juce::dsp::Reverb reverb;
};

class DelayModule
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        delayLine.prepare(spec);
    }

    void process(juce::AudioBuffer<float>& buffer, float timeMs,
                 float feedback, float mix, bool sync,
                 int division, bool enabled)
    {
        if (!enabled)
            return;
    }

private:
    double sampleRate = 44100.0;

    juce::dsp::DelayLine<
        float,
        juce::dsp::DelayLineInterpolationTypes::Linear> delayLine{441000};
};
