#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JarEqAudioProcessorEditor::JarEqAudioProcessorEditor(JarEqAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(600, 400);

    // Add frequency label and slider
    addAndMakeVisible(freqLabel);
    freqLabel.setText("Frequency", juce::NotificationType::dontSendNotification);
    freqLabel.attachToComponent(&freqSlider, true);
    addAndMakeVisible(freqSlider);
    freqSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    freqSlider.setRange(20.0, 20000.0, 1.0);
    freqSlider.setValue(1000.0);
    freqSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

    // Add gain label and slider
    addAndMakeVisible(gainLabel);
    gainLabel.setText("Gain", juce::NotificationType::dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, true);
    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setRange(-24.0, 24.0, 0.1);
    gainSlider.setValue(0.0);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

    // Add Q label and slider
    addAndMakeVisible(qLabel);
    qLabel.setText("Q", juce::NotificationType::dontSendNotification);
    qLabel.attachToComponent(&qSlider, true);
    addAndMakeVisible(qSlider);
    qSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    qSlider.setRange(0.1, 10.0, 0.1);
    qSlider.setValue(1.0);
    qSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

    // Add filter type combo box
    addAndMakeVisible(filterTypeComboBox);
    filterTypeComboBox.addItem("Bell", 1);
    filterTypeComboBox.addItem("High Pass", 2);
    filterTypeComboBox.addItem("Low Pass", 3);
    filterTypeComboBox.addItem("High Shelf", 4);
    filterTypeComboBox.addItem("Low Shelf", 5);
    filterTypeComboBox.setSelectedId(1);

    // Add global gain label and slider
    addAndMakeVisible(globalGainLabel);
    globalGainLabel.setText("Global Gain", juce::NotificationType::dontSendNotification);
    globalGainLabel.attachToComponent(&globalGainSlider, true);
    addAndMakeVisible(globalGainSlider);
    globalGainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    globalGainSlider.setRange(-24.0, 24.0, 0.1);
    globalGainSlider.setValue(0.0);
    globalGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

    // Add mix label and slider
    addAndMakeVisible(mixLabel);
    mixLabel.setText("Mix", juce::NotificationType::dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, true);
    addAndMakeVisible(mixSlider);
    mixSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mixSlider.setRange(0.0, 1.0, 0.01);

    // Add bypass button
    addAndMakeVisible(bypassButton);
    bypassButton.setButtonText("Bypass");
    bypassButton.onClick = [this] { audioProcessor.setBypassed(!audioProcessor.isBypassed()); };
    bypassButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    bypassButton.setToggleState(!audioProcessor.isBypassed(), juce::NotificationType::dontSendNotification);

    // Add reset button
    addAndMakeVisible(resetButton);
    resetButton.setButtonText("Reset");
    resetButton.onClick = [this] { audioProcessor.reset(); };

    // Add waveform display
    addAndMakeVisible(waveformDisplay);
    waveformDisplay.setBufferSize(2048);
    waveformDisplay.setNumChannels(1);
    waveformDisplay.setSamplesPerBlock(512);
    waveformDisplay.setLookAndFeel(&lnf);
    startTimerHz(30);

}

JarEqAudioProcessorEditor::~JarEqAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void JarEqAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
}
void JarEqAudioProcessorEditor::resized()
{
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;

    grid.templateRows = { Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr), Track(4_fr) };
    grid.templateColumns = { Track(1_fr), Track(1_fr) };

    grid.items = {
        juce::GridItem(freqLabel).withSize(100, 30).withArea(1, 1),
        juce::GridItem(freqSlider).withSize(100, 100).withArea(2, 1),
        juce::GridItem(gainLabel).withSize(100, 30).withArea(1, 2),
        juce::GridItem(gainSlider).withSize(100, 100).withArea(2, 2),
        juce::GridItem(qLabel).withSize(100, 30).withArea(1, 3),
        juce::GridItem(qSlider).withSize(100, 100).withArea(2, 3),
        juce::GridItem(filterTypeComboBox).withSize(100, 30).withArea(1, 4),
        juce::GridItem(globalGainLabel).withSize(100, 30).withArea(1, 5),
        juce::GridItem(globalGainSlider).withSize(100, 200).withArea(2, 5),
        juce::GridItem(mixLabel).withSize(100, 30).withArea(1, 6),
        juce::GridItem(mixSlider).withSize(100, 200).withArea(2, 6),
        juce::GridItem(bypassButton).withSize(100, 30).withArea(1, 7),
        juce::GridItem(resetButton).withSize(100, 30).withArea(2, 7),
        juce::GridItem(waveformDisplay).withSize(500, 200).withArea(3, 1, 5, 2),
    };

    grid.performLayout(getLocalBounds());

}

void JarEqAudioProcessorEditor::timerCallback()
{
    auto waveform = audioProcessor.getWaveformDisplay();
    if (waveform.getNumSamples() > 0)
    {
        waveformDisplay.setSamples(waveform.getReadPointer(0), waveform.getNumSamples());
    }
    else
    {
        waveformDisplay.clear();
    }
}

