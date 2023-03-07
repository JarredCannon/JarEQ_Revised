#pragma once

#include <JuceHeader.h>

class JarEqAudioProcessor : public juce::AudioProcessor
{
public:
    JarEqAudioProcessor();
    ~JarEqAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    int getNumParameters() override;
    float getParameter(int index) override;
    void setParameter(int index, float newValue) override;
    const juce::String getParameterName(int index) override;
    const juce::String getParameterText(int index) override;

    bool isParameterAutomatable(int index) const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

private:
    juce::AudioParameterFloat* freqParam;
    juce::AudioParameterFloat* gainParam;
    juce::AudioParameterFloat* qParam;
    juce::AudioParameterChoice* filterTypeParam;
    juce::AudioParameterFloat* globalGainParam;
    juce::AudioParameterFloat* mixParam;

    dsp::IIR::Filter<float> filter;
    dsp::ProcessorChain<dsp::IIR::Filter<float>, dsp::Gain<float>> processorChain;

    dsp::IIR::Coefficients<float>::Ptr state;

    void updateCoefficients();
};
