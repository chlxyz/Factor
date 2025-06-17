#include "LEDIndicator.h"

LEDIndicator::LEDIndicator()
{
    startTimerHz(30); // refresh every ~33ms
}

void LEDIndicator::setAudioBuffer(const juce::AudioBuffer<float>& bufferToDraw)
{
    buffer.makeCopyOf(bufferToDraw);

    float level = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    isAudioDetected = (level > 0.01f);
}

void LEDIndicator::timerCallback()
{
    isAudioDetected = false;

    if (buffer.getNumSamples() > 0)
    {
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* samples = buffer.getReadPointer(channel);
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                if (std::abs(samples[i]) > 0.01f) // adjust threshold as needed
                {
                    isAudioDetected = true;
                    break;
                }
            }
            if (isAudioDetected)
                break; // Exit loop once audio is detected
        }
    }
    // Update hue if audio is detected
    if (isAudioDetected)
    {
        hue += 0.01f;
        if (hue > 1.0f)
            hue -= 1.0f;
    }

    repaint();
}


void LEDIndicator::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(4);

    juce::Colour glowColour = isAudioDetected
        ? juce::Colour::fromHSV(hue, 1.0f, 1.0f, 1.0f)
        : juce::Colours::darkgrey;

    // Outer glow
    g.setColour(glowColour.withAlpha(0.4f));
    g.setGradientFill(juce::ColourGradient{
        glowColour.withAlpha(0.4f), bounds.getCentreX(), bounds.getCentreY(),
        juce::Colours::transparentBlack, bounds.getRight(), bounds.getBottom(),
        false
        });
    g.fillRoundedRectangle(bounds.expanded(10), 10.0f); // soft outer glow

    // Main LED
    g.setColour(glowColour);
    g.fillRoundedRectangle(bounds, 8.0f);
}
