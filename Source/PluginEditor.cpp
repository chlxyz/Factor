/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FactorAudioProcessorEditor::FactorAudioProcessorEditor (FactorAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
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

    setSize(200, 200);
}

FactorAudioProcessorEditor::~FactorAudioProcessorEditor()
{
}

//==============================================================================
void FactorAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}


void FactorAudioProcessorEditor::resized()
{
    depthSlider.setBounds(40, 40, 120, 120); // x, y, width, height
}

