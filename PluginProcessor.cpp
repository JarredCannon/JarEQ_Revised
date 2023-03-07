#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JarEqAudioProcessorEditor::JarEqAudioProcessorEditor(JarEqAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Initialize member variables here
    for (int i = 0; i < 10; ++i) {
        mBandTypeComboAttachments[i] = nullptr;
    }

    setSize(800, 600);

    int xPos = 20;
    int yPos = 20;
    int yGap = 30;
    int controlWidth = 100;
    int controlHeight = 20;

    addAndMakeVisible(bandCountLabel);
    bandCountLabel.setText("Band Count:", NotificationType::dontSendNotification);
    bandCountLabel.attachToComponent(&bandCountSlider, false);
    bandCountLabel.setJustificationType(Justification::centredLeft);
    bandCountLabel.setFont(Font(15.0f, Font::plain));

    addAndMakeVisible(bandCountSlider);
    bandCountSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    bandCountSlider.setRange(1, 10, 1);
    bandCountSlider.setValue(5);
    bandCountSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    bandCountSlider.addListener(this);

    xPos = 20;
    yPos = 80;

    for (int i = 0; i < 10; ++i) {
        addAndMakeVisible(bandTypeLabels[i]);
        bandTypeLabels[i].setText("Band " + String(i) + " Type:", NotificationType::dontSendNotification);
        bandTypeLabels[i].attachToComponent(&bandTypeCombos[i], false);
        bandTypeLabels[i].setJustificationType(Justification::centredLeft);
        bandTypeLabels[i].setFont(Font(15.0f, Font::plain));

        addAndMakeVisible(bandTypeCombos[i]);
        bandTypeCombos[i].addItem("Bell", 1);
        bandTypeCombos[i].addItem("HighPass", 2);
        bandTypeCombos[i].addItem("LowPass", 3);
        bandTypeCombos[i].addItem("HighShelf", 4);
        bandTypeCombos[i].addItem("LowShelf", 5);
        bandTypeCombos[i].setSelectedId(1);
        bandTypeCombos[i].addListener(this);
        bandTypeCombos[i].setEditableText(false);
        bandTypeCombos[i].setJustificationType(Justification::centredLeft);

        addAndMakeVisible(bandFreqLabels[i]);
        bandFreqLabels[i].setText("Band " + String(i) + " Freq:", NotificationType::dontSendNotification);
        bandFreqLabels[i].attachToComponent(&bandFreqSliders[i], false);
        bandFreqLabels[i].setJustificationType(Justification::centredLeft);
        bandFreqLabels[i].setFont(Font(15.0f, Font::plain));

        addAndMakeVisible(bandFreqSliders[i]);
        bandFreqSliders[i].setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
        bandFreqSliders[i].setRange(20.0, 20000.0, 1.0);
        bandFreqSliders[i].setValue(1000.0);
        bandFreqSliders[i].setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        bandFreqSliders[i].addListener(this);

        addAndMakeVisible(bandGainLabels[i]);
        bandGainLabels[i].setText("Band " + String(i) + " Gain:", NotificationType::dontSendNotification);
        bandGainLabels[i].attachToComponent(&bandGainSliders[i], false);
        bandGainLabels[i].setJustificationType(Justification::centredLeft);
        bandGainLabels[i].setFont(Font(15.0f, Font::plain));

        addAndMakeVisible(bandGainSliders[i]);
        bandGainSliders[i].setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
        bandGainSliders[i].setRange(-24.0, 24.0, 0.1);
        bandGainSliders[i].setValue(0.0);
        bandGainSliders[i].setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        bandGainSliders[i].addListener(this);

        addAndMakeVisible(bandQLabels[i]);
        bandQLabels[i].setText("Band " + String(i) + " Q:", NotificationType::dontSendNotification);
        bandQLabels[i].attachToComponent(&bandQSliders[i], false);
        bandQLabels[i].setJustificationType(Justification::centredLeft);
        bandQLabels[i].setFont(Font(15.0f, Font::plain));

        addAndMakeVisible(bandQSliders[i]);
        bandQSliders[i].setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
        bandQSliders[i].setRange(0.1, 10.0, 0.1);
        bandQSliders[i].setValue(1.0);
        bandQSliders[i].setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        bandQSliders[i].addListener(this);

        xPos += controlWidth + 30;
        if ((i + 1) % 5 == 0) {
            xPos = 20;
            yPos += yGap * 4;
        }
    }

    addAndMakeVisible(globalGainLabel);
    globalGainLabel.setText("Global Gain:", NotificationType::dontSendNotification);
    globalGainLabel.attachToComponent(&globalGainSlider, false);
    globalGainLabel.setJustificationType(Justification::centredLeft);
    globalGainLabel.setFont(Font(15.0f, Font::plain));

    addAndMakeVisible(globalGainSlider);
    globalGainSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    globalGainSlider.setRange(-24.0, 24.0, 0.1);
    globalGainSlider.setValue(0.0);
    globalGainSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    globalGainSlider.addListener(this);

    addAndMakeVisible(mixLabel);
    mixLabel.setText("Mix:", NotificationType::dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, false);
    mixLabel.setJustificationType(Justification::centredLeft);
    mixLabel.setFont(Font(15.0f, Font::plain));

    addAndMakeVisible(mixSlider);
    mixSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    mixSlider.setRange(0.0, 1.0, 0.01);
    mixSlider.setValue(1.0);
    mixSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mixSlider.addListener(this);

    addAndMakeVisible(hpfLabel);
    hpfLabel.setText("High Pass Freq:", NotificationType::dontSendNotification);
    hpfLabel.attachToComponent(&hpfSlider, false);
    hpfLabel.setJustificationType(Justification::centredLeft);
    hpfLabel.setFont(Font(15.0f, Font::plain));

    addAndMakeVisible(hpfSlider);
    hpfSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    hpfSlider.setRange(20.0, 20000.0, 1.0);
    hpfSlider.setValue(20.0);
    hpfSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    hpfSlider.addListener(this);

    addAndMakeVisible(lpfLabel);
    lpfLabel.setText("Low Pass Freq:", NotificationType::dontSendNotification);
    lpfLabel.attachToComponent(&lpfSlider, false);
    lpfLabel.setJustificationType(Justification::centredLeft);
    lpfLabel.setFont(Font(15.0f, Font::plain));

    addAndMakeVisible(lpfSlider);
    lpfSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    lpfSlider.setRange(20.0, 20000.0, 1.0);
    lpfSlider.setValue(20000.0);
    lpfSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    lpfSlider.addListener(this);

    addAndMakeVisible(resetButton);
    resetButton.setButtonText("Reset");
    resetButton.onClick = [this]() { this->audioProcessor.reset(); };
    resetButton.setBounds(350, 20, 100, 30);

    addAndMakeVisible(bypassButton);
    bypassButton.setButtonText("Bypass");
    bypassButton.onClick = [this]() { this->audioProcessor.setBypassed(!this->audioProcessor.isBypassed()); };
    bypassButton.setBounds(500, 20, 100, 30);

    addAndMakeVisible(waveformDisplay);
    waveformDisplay.setBufferSize(512);
    waveformDisplay.setBounds(50, 200, 700, 250);

    startTimerHz(30);
}

JarEqAudioProcessor::JarEqAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
#endif
    mBandCount(5),
    mBandTypes{ FilterType::Bell, FilterType::Bell, FilterType::Bell, FilterType::Bell, FilterType::Bell,
                FilterType::Bell, FilterType::Bell, FilterType::Bell, FilterType::Bell, FilterType::Bell },
    mBandFreqs{ 100.0f, 300.0f, 800.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f },
    mBandGains{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    mBandQs{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    mGlobalGain(1.0f),
    mMixed(1.0f),
    mHpfFreq(20.0f),
    mLpfFreq(20000.0f)
{
    updateFilters();
}


void JarEqAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    less
        Copy code
        g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);
}

void JarEqAudioProcessorEditor::resized()
{
}

void JarEqAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &bandCountSlider) {
        audioProcessor.setBandCount(static_cast<int>(bandCountSlider.getValue()));
    }
    else {
        for (int i = 0; i < 10; ++i) {
            if (slider == &bandTypeCombos[i]) {
                audioProcessor.setBandType(i, static_castJarEqAudioProcessor::FilterType(bandTypeCombos[i].getSelectedId()));
            }
            else if (slider == &bandFreqSliders[i]) {
                audioProcessor.setBandFreq(i, bandFreqSliders[i].getValue());
            }
            else if (slider == &bandGainSliders[i]) {
                audioProcessor.setBandGain(i, bandGainSliders[i].getValue());
            }
            else if (slider == &bandQSliders[i]) {
                audioProcessor.setBandQ(i, bandQSliders[i].getValue());
            }
        }
    }
    if (slider == &globalGainSlider) {
        audioProcessor.setGlobalGain(globalGainSlider.getValue());
    }
    else if (slider == &mixSlider) {
        audioProcessor.setMix(mixSlider.getValue());
    }
    else if (slider == &hpfSlider) {
        audioProcessor.setHpfFreq(hpfSlider.getValue());
    }
    else if (slider == &lpfSlider) {
        audioProcessor.setLpfFreq(lpfSlider.getValue());
    }
}

void JarEqAudioProcessorEditor::timerCallback()
{
    waveformDisplay.setSamples(audioProcessor.getWaveformDisplay(), audioProcessor.getWaveformDisplaySize());
}

void JarEqAudioProcessor::setBandCount(int bandCount)
{
    mBandCount = bandCount;
    updateFilters();
}

void JarEqAudioProcessor::setBandType(int bandIndex, FilterType type)
{
    mBandTypes[bandIndex] = type;
    updateFilters();
}

void JarEqAudioProcessor::setBandFreq(int bandIndex, float freq)
{
    mBandFreqs[bandIndex] = freq;
    updateFilters();
}

void JarEqAudioProcessor::setBandGain(int bandIndex, float gain)
{
    mBandGains[bandIndex] = gain;
    updateFilters();
}

void JarEqAudioProcessor::setBandQ(int bandIndex, float q)
{
    mBandQs[bandIndex] = q;
    updateFilters();
}

void JarEqAudioProcessor::setGlobalGain(float gain)
{
    mGlobalGain = Decibels::decibelsToGain(gain);
    updateFilters();
}

void JarEqAudioProcessor::setMix(float mix)
{
    mMixed = mix;
    updateFilters();
}

void JarEqAudioProcessor::setHpfFreq(float freq)
{
    mHpfFreq = freq;
    updateFilters();
}

void JarEqAudioProcessor::setLpfFreq(float freq)
{
    mLpfFreq = freq;
    updateFilters();
}

void JarEqAudioProcessor::reset()
{
    mBandCount = 5;
    for (int i = 0; i < 10; ++i) {
        mBandTypes[i] = FilterType::Bell;
        mBandFreqs[i] = 1000.0f;
        mBandGains[i] = 0.0f;
        mBandQs[i] = 1.0f;
    }
    mGlobalGain = 1.0f;
    mMixed = 1.0f;
    mHpfFreq = 20.0f;
    mLpfFreq = 20000.0f;

    updateFilters();
}
