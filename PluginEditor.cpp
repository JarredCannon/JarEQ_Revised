#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JarEqAudioProcessorEditor::JarEqAudioProcessorEditor(JarEqAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(600, 400);

    auto& lookAndFeel = getLookAndFeel();
    lookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::black);

    addAndMakeVisible(freqSlider);
    freqSlider.setRange(20.0f, 20000.0f, 1.0f);
    freqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20);
    freqSlider.addListener(this);

    addAndMakeVisible(freqLabel);
    freqLabel.setText("Frequency", juce::dontSendNotification);
    freqLabel.attachToComponent(&freqSlider, false);

    addAndMakeVisible(gainSlider);
    gainSlider.setRange(-24.0f, 24.0f, 0.1f);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20);
    gainSlider.addListener(this);

    addAndMakeVisible(gainLabel);
    gainLabel.setText("Gain (dB)", juce::dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, false);

    addAndMakeVisible(qSlider);
    qSlider.setRange(0.1f, 10.0f, 0.1f);
    qSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20);
    qSlider.addListener(this);

    addAndMakeVisible(qLabel);
    qLabel.setText("Q", juce::dontSendNotification);
    qLabel.attachToComponent(&qSlider, false);

    addAndMakeVisible(filterTypeBox);
    filterTypeBox.addItem("Bell", 1);
    filterTypeBox.addItem("High Pass", 2);
    filterTypeBox.addItem("Low Pass", 3);
    filterTypeBox.addItem("High Shelf", 4);
    filterTypeBox.addItem("Low Shelf", 5);
    filterTypeBox.setSelectedId(1, juce::dontSendNotification);
    filterTypeBox.addListener(this);

    addAndMakeVisible(filterTypeLabel);
    filterTypeLabel.setText("Filter Type", juce::dontSendNotification);
    filterTypeLabel.attachToComponent(&filterTypeBox, false);

    addAndMakeVisible(globalGainSlider);
    globalGainSlider.setRange(-24.0f, 24.0f, 0.1f);
    globalGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20);
    globalGainSlider.addListener(this);

    addAndMakeVisible(globalGainLabel);
    globalGainLabel.setText("Global Gain (dB)", juce::dontSendNotification);
    globalGainLabel.attachToComponent(&globalGainSlider, false);

    addAndMakeVisible(mixSlider);
    mixSlider.setRange(0.0f, 1.0f, 0.01f);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20);
    mixSlider.addListener(this);

    addAndMakeVisible(mixLabel);
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, false);

    mCoefficients = processor.getCoefficients();
    mGlobalGain = processor.getGlobalGain();
    mMix = processor.getMix();

    updateSliderValues();
}

JarEqAudioProcessorEditor::~JarEqAudioProcessorEditor()
{
}

void JarEqAudioProcessorEditor::resized()
{
    const int margin = 10;
    const int eqMargin = 20;
    const int labelHeight = 20;
    const int controlHeight = 60;
    const int sliderWidth = 200;
    const int comboBoxWidth = 100;
    const int filterControlsWidth = sliderWidth + comboBoxWidth + eqMargin;
    const int globalControlsWidth = sliderWidth + eqMargin;
    const int filterSectionHeight = labelHeight + controlHeight * mProcessor.getNumBands() + eqMargin * (mProcessor.getNumBands() - 1);
    const int totalWidth = margin * 2 + filterControlsWidth * 2 + globalControlsWidth + eqMargin;
    const int totalHeight = margin * 2 + filterSectionHeight + eqMargin * 3 + labelHeight * 2 + controlHeight;
    setResizable(true, false);
    setResizeLimits(totalWidth, totalHeight, totalWidth, totalHeight);

    int x = margin;
    int y = margin + labelHeight + controlHeight;

    for (int i = 0; i < mProcessor.getNumBands(); ++i)
    {
        auto& freqSlider = mFreqSliders[i];
        freqSlider.setBounds(x, y, sliderWidth, controlHeight);

        auto& freqLabel = mFreqLabels[i];
        freqLabel.setBounds(x, y - labelHeight, sliderWidth, labelHeight);

        auto& gainSlider = mGainSliders[i];
        gainSlider.setBounds(x + sliderWidth + eqMargin, y, sliderWidth, controlHeight);

        auto& gainLabel = mGainLabels[i];
        gainLabel.setBounds(x + sliderWidth + eqMargin, y - labelHeight, sliderWidth, labelHeight);

        auto& qSlider = mQSliders[i];
        qSlider.setBounds(x + sliderWidth * 2 + eqMargin * 2 + comboBoxWidth, y, sliderWidth, controlHeight);

        auto& qLabel = mQLabels[i];
        qLabel.setBounds(x + sliderWidth * 2 + eqMargin * 2 + comboBoxWidth, y - labelHeight, sliderWidth, labelHeight);

        auto& filterTypeBox = mFilterTypeBoxes[i];
        filterTypeBox.setBounds(x + sliderWidth + eqMargin, y - labelHeight - eqMargin, comboBoxWidth, labelHeight);

        y += controlHeight + eqMargin;
    }

    y = margin;
    x = margin * 2 + filterControlsWidth;

    auto& globalGainSlider = mGlobalGainSlider;
    globalGainSlider.setBounds(x, y, sliderWidth, controlHeight);

    auto& globalGainLabel = mGlobalGainLabel;
    globalGainLabel.setBounds(x, y - labelHeight, sliderWidth, labelHeight);

    y += controlHeight + eqMargin;

    auto& mixSlider = mMixSlider;
    mixSlider.setBounds(x, y, sliderWidth, controlHeight);

    auto& mixLabel = mMixLabel;
    mixLabel.setBounds(x, y - labelHeight, sliderWidth, labelHeight);

    x += sliderWidth + eqMargin;

    auto& bypassButton = mBypassButton;
    bypassButton.setBounds(x, y, comboBoxWidth, controlHeight);

    auto& bypassLabel = mBypassLabel;
    bypassLabel.setBounds(x, y - labelHeight, comboBoxWidth, labelHeight);

    y += controlHeight + eqMargin;

    auto& analyzerButton = mAnalyzerButton;
    analyzerButton.setBounds(x, y, comboBoxWidth, controlHeight);

    auto& analyzerLabel = mAnalyzerLabel;
    analyzerLabel.setText("Analyzer Type", juce::dontSendNotification);

    analyzerLabel.attachToComponent(&analyzerButton, false);

    addAndMakeVisible(analyzerLabel);
    addAndMakeVisible(analyzerButton);

    analyzerButton.setSelectedId(1, juce::dontSendNotification);
    analyzerButton.addListener(this);

    x += comboBoxWidth + spacing;

    addAndMakeVisible(mAnalyzerDisplay);
    mAnalyzerDisplay.setBounds(x, y, displayWidth, displayHeight);
    mAnalyzerDisplay.setBufferSize(2048);

    mAnalyzerDisplay.setUpdateInterval(50);
    mAnalyzerDisplay.setRange(0.0f, 1.0f);

    x = padding;
    y += displayHeight + spacing;

    for (int i = 0; i < numBands; ++i)
    {
        addAndMakeVisible(mBandSliders[i]);
        addAndMakeVisible(mBandLabels[i]);

        mBandSliders[i].setRange(-24.0f, 24.0f, 0.1f);
        mBandSliders[i].setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20);
        mBandSliders[i].addListener(this);

        mBandLabels[i].setText(juce::String(i + 1), juce::dontSendNotification);
        mBandLabels[i].attachToComponent(&mBandSliders[i], true);

        x += controlWidth + spacing;

        if (x + controlWidth + padding > getWidth())
        {
            x = padding;
            y += controlHeight + spacing;
        }
    }
}
