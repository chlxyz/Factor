#pragma once

#include <JuceHeader.h>

class LEDIndicator : public juce::Component, private juce::Timer
{
public:
    LEDIndicator();

    void setAudioBuffer(const juce::AudioBuffer<float>& bufferToDraw);

    void paint(juce::Graphics& g) override;
    void resized() override {}

private:
    void timerCallback() override;

    juce::AudioBuffer<float> buffer;
    bool isAudioDetected = false;

    float hue = 0.0f; // For RGB cycling (0.0 to 1.0)

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LEDIndicator)
};
