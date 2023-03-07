#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JarEqAudioProcessorEditor::JarEqAudioProcessorEditor(JarEqAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
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
        bandTypeCombos[i].addItem("High Pass", 2);
        bandTypeCombos[i].addItem("Low Pass", 3);
        bandTypeCombos[i].addItem("High Shelf", 4);
        bandTypeCombos[i].addItem("Low Shelf", 5);
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