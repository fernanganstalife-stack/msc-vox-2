#include "PluginProcessor.h"
#include "PluginEditor.h"

MSCVoxityV2AudioProcessor::MSCVoxityV2AudioProcessor()
     : AudioProcessor (BusesProperties().withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
       apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

MSCVoxityV2AudioProcessor::~MSCVoxityV2AudioProcessor() {}

const juce::String MSCVoxityV2AudioProcessor::getName() const { return JucePlugin_Name; }
bool MSCVoxityV2AudioProcessor::acceptsMidi() const { return false; }
bool MSCVoxityV2AudioProcessor::producesMidi() const { return false; }
double MSCVoxityV2AudioProcessor::getTailLengthSeconds() const { return 0.0; }
int MSCVoxityV2AudioProcessor::getNumPrograms() { return 1; }
int MSCVoxityV2AudioProcessor::getCurrentProgram() { return 0; }
void MSCVoxityV2AudioProcessor::setCurrentProgram (int) {}
const juce::String MSCVoxityV2AudioProcessor::getProgramName (int) { return {}; }
void MSCVoxityV2AudioProcessor::changeProgramName (int, const juce::String&) {}

juce::AudioProcessorValueTreeState::ParameterLayout MSCVoxityV2AudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back(std::make_unique<juce::AudioParameterBool>("autotune_on", "Auto-Tune On", true));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("autotune_amount", "Auto-Tune Amount", 0.0f, 100.0f, 100.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("comp_thresh", "Compressor Thresh", -60.0f, 0.0f, -12.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("comp_ratio", "Compressor Ratio", 1.0f, 20.0f, 4.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("master_mix", "Master Mix", 0.0f, 100.0f, 100.0f));
    return { params.begin(), params.end() };
}

void MSCVoxityV2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec{ sampleRate, (uint32_t)samplesPerBlock, 2 };
    autoTuneModule.prepare(spec);
    compressorModule.prepare(spec);
    deEsserModule.prepare(spec);
    eqModule.prepare(spec);
    saturationModule.prepare(spec);
    chorusModule.prepare(spec);
    stereoWidenerModule.prepare(spec);
    reverbModule.prepare(spec);
    delayModule.prepare(spec);
}

void MSCVoxityV2AudioProcessor::releaseResources() {}

bool MSCVoxityV2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return layouts.getMainOutputChannelSet() == layouts.getMainInputChannelSet();
}

void MSCVoxityV2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    inputMeterLevel.store(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));

    float gr = 0.0f;
    autoTuneModule.process(buffer, true, 0, 0, 20.0f, 1.0f, 0);
    compressorModule.process(buffer, gr);
    compGR.store(gr);

    outputMeterLevel.store(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
}

bool MSCVoxityV2AudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* MSCVoxityV2AudioProcessor::createEditor() { return new MSCVoxityV2AudioProcessorEditor (*this); }

void MSCVoxityV2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void MSCVoxityV2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr && xmlState->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessorEditor* createEditor() { return nullptr; }
