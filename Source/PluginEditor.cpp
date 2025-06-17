/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FactorAudioProcessorEditor::FactorAudioProcessorEditor(FactorAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    depthSlider.setSliderStyle(juce::Slider::Rotary);
    depthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    depthSlider.setRange(0.0, 1.0, 0.01);
    depthSlider.setValue(*processor.depthParam);
    depthSlider.setNumDecimalPlacesToDisplay(2);
    depthSlider.setTextValueSuffix(" %");
    depthSlider.onValueChange = [this]() {
        *processor.depthParam = (float)depthSlider.getValue();
        };
    addAndMakeVisible(depthSlider);

    addAndMakeVisible(waveform);
    startTimerHz(30); // update waveform

	addAndMakeVisible(ledIndicator);

    setSize(400, 250); // expanded size for display
}

FactorAudioProcessorEditor::~FactorAudioProcessorEditor()
{
}

//==============================================================================
void FactorAudioProcessorEditor::timerCallback()
{
    waveform.setAudioBuffer(processor.getVisualBuffer());
    ledIndicator.setAudioBuffer(processor.getVisualBuffer());
}

void FactorAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::white);
}


void FactorAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20); // padding of 20px on all sides

    // Reserve top row height (e.g., for slider + waveform)
    auto topRow = area.removeFromTop(140);

    // Depth slider on the left of top row
    depthSlider.setBounds(topRow.removeFromLeft(120));

    // Space between slider and waveform
    topRow.removeFromLeft(20);

    // Waveform uses the rest of the top row
    waveform.setBounds(topRow);

    // Remaining area for LED
    area.removeFromTop(20); // spacing between waveform and LED
    ledIndicator.setBounds(area.removeFromTop(40));
}

