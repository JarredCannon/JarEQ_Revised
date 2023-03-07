#pragma once

#include <JuceHeader.h>
#include "BandFilter.h"
#include "WaveformDisplay.h"

class JarEqAudioProcessor : public juce::AudioProcessor
{
public:
    enum class FilterType
    {
        Bell,
        HighPass,
        LowPass,
        HighShelf,
        LowShelf
    };

    JarEqAudioProcessor();
    ~JarEqAudioProcessor() override;

    void updateFilters();
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    int getBandCount() const;
    void setBandCount(int bandCount);
    FilterType getBandType(int bandIndex) const;
    void setBandType(int bandIndex, FilterType type);
    float getBandFreq(int bandIndex) const;
    void setBandFreq(int bandIndex, float freq);
    float getBandGain(int bandIndex) const;
    void setBandGain(int bandIndex, float gain);
    float getBandQ(int bandIndex) const;
    void setBandQ(int bandIndex, float q);
    float getGlobalGain() const;
    void setGlobalGain(float gain);
    float getMix() const;
    void setMix(float mix);
    float getHpfFreq() const;
    void setHpfFreq(float freq);
    float getLpfFreq() const;
    void setLpfFreq(float freq);
    void reset();
    float* getWaveformDisplay();
    int getWaveformDisplaySize() const;
    bool isBypassed() const;
    void setBypassed(bool bypassed);

private:
    juce::AudioProcessorValueTreeState mParams;
    std::vector<BandFilter> mBandFilters;
    juce::dsp::HighPassFilter<float> mHighPassFilter;
    juce::dsp::LowPassFilter<float> mLowPassFilter;
    juce::dsp::Gain<float> mInputGain;
    juce::dsp::Gain<float> mOutputGain;
    WaveformDisplay<float> mWaveformDisplay;
    bool mBypassed;

    int mBandCount;
    std::array<FilterType, 10> mBandTypes;
    std::array<float, 10> mBandFreqs;
    std::array<float, 10> mBandGains;
    std::array<float, 10> mBandQs;
    float mGlobalGain;
    float mMixed;
    float mHpfFreq;
    float mLpfFreq;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR