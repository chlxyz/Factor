 
#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay()
{
    startTimerHz(30); // refresh at 30 Hz
}

void WaveformDisplay::setAudioBuffer(const juce::AudioBuffer<float>& bufferToDraw)
{
    buffer.makeCopyOf(bufferToDraw);
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);

    auto width = getWidth();
    auto height = getHeight();
    auto midY = height / 2;

    if (buffer.getNumSamples() == 0)
        return;

    auto* samples = buffer.getReadPointer(0);
    int numSamples = buffer.getNumSamples();

    float scaleX = static_cast<float>(width - 1) / (numSamples - 1);

    juce::Path path;
    path.startNewSubPath(0, midY);

    for (int i = 0; i < numSamples; ++i)
    {
        float x = i * scaleX;
        float y = midY - (samples[i] * midY * 10.0f); // temporarily amplify visually
        path.lineTo(x, y);
    }

    g.strokePath(path, juce::PathStrokeType(1.0f));
}

void WaveformDisplay::timerCallback()
{
    repaint();
}
