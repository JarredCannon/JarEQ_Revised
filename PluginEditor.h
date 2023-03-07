#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveformDisplay.h"
#include "CustomLnF.h"

//==============================================================================
/**
*/
class JarEqAudioProcessorEditor : public juce::AudioProcessorEditor,
    private juce::Timer
{
public:
    JarEqAudioProcessorEditor(JarEqAudioProcessor&);
    ~JarEqAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JarEqAudioProcessor& audioProcessor;

    juce::Slider freqSlider, gainSlider, qSlider, globalGainSlider, mixSlider;
    juce::ComboBox filterTypeComboBox;
    juce::Label freqLabel, gainLabel, qLabel, globalGainLabel, mixLabel;
    juce::TextButton bypassButton, resetButton;

    WaveformDisplay waveformDisplay;
    CustomLnF lnf;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JarEqAudioProcessorEditor)
};
