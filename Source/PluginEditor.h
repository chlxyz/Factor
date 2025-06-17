/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveformDisplay.h"
#include "LEDIndicator.h"

//==============================================================================
/**
*/
class FactorAudioProcessorEditor  : public juce::AudioProcessorEditor,
    private juce::Timer
{
public:
    FactorAudioProcessorEditor (FactorAudioProcessor&);
    ~FactorAudioProcessorEditor() override;


    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FactorAudioProcessor& processor;
    juce::Slider depthSlider;
    WaveformDisplay waveform;
	  LEDIndicator ledIndicator;

    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FactorAudioProcessorEditor)
};
