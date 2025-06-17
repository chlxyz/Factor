#pragma once

#include <JuceHeader.h>

class WaveformDisplay : public juce::Component,
    private juce::Timer
{
public:
    WaveformDisplay();

    void setAudioBuffer(const juce::AudioBuffer<float>& bufferToDraw);

    void paint(juce::Graphics& g) override;
    void resized() override {}

private:
    void timerCallback() override;

    juce::AudioBuffer<float> buffer;
    juce::Image waveformImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
