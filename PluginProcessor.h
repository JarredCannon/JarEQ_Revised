#pragma once

#include <JuceHeader.h>

class JarEqAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    JarEqAudioProcessor();
    ~JarEqAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void updateCoefficients();
    void processBlockBypassed(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void reset() override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    int getNumParameters() override;
    float getParameter(int index) override;
    void setParameter(int index, float newValue) override;
    juce::String getParameterName(int index) override;
    juce::String getParameterText(int index) override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getState();

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState mState;

    juce::AudioParameterFloat* freqParam;
    juce::AudioParameterFloat* gainParam;
    juce::AudioParameterFloat* qParam;
    juce::AudioParameterChoice* filterTypeParam;
    juce::AudioParameterFloat* globalGainParam;
    juce::AudioParameterFloat* mixParam;

    dsp::ProcessorChain<dsp::IIR::Filter<float>, dsp::Gain<float>> processorChain;
    dsp::IIR::Coefficients<float>::Ptr state;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JarEqAudioProcessor)
};
