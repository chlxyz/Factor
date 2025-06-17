/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FactorAudioProcessor::FactorAudioProcessor()
//#ifndef JucePlugin_PreferredChannelConfigurations
//     : AudioProcessor (BusesProperties()
//                     #if ! JucePlugin_IsMidiEffect
//                      #if ! JucePlugin_IsSynth
//                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
//                      #endif
//                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
//                     #endif
//                       )
//#endif
{
    addParameter(depthParam = new juce::AudioParameterFloat("depth", "Depth", 0.0f, 1.0f, 0.5f));
}

FactorAudioProcessor::~FactorAudioProcessor()
{
}

//==============================================================================
const juce::String FactorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FactorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FactorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FactorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FactorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FactorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FactorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FactorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FactorAudioProcessor::getProgramName (int index)
{
    return {};
}

void FactorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FactorAudioProcessor::prepareToPlay (double newSampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    sampleRate = newSampleRate;
    delayBuffer.setSize(getTotalNumInputChannels(), (int)(2.0 * sampleRate));
    delayBuffer.clear();
    delayBufferWritePosition = 0;
    lfoPhase = 0.0f;
}

void FactorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FactorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FactorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    //auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    int numSamples = buffer.getNumSamples();
    int numChannels = totalNumOutputChannels;

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    //for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    //    buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < numSamples; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        auto* delayData = delayBuffer.getWritePointer(channel);

        for (int i = 0; i < numSamples; ++i)
        {
            delayData[delayBufferWritePosition] = channelData[i];

            float mod = std::sin(lfoPhase) * (*depthParam) * 0.01f; // up to 10ms
            float delayTime = mod + 0.005f;
            int delaySamples = static_cast<int>(delayTime * sampleRate);

            int readPosition = delayBufferWritePosition - delaySamples;
            if (readPosition < 0)
                readPosition += delayBuffer.getNumSamples();

            float delayedSample = delayData[readPosition];
            channelData[i] = delayedSample;

            lfoPhase += 2.0f * juce::MathConstants<float>::pi * lfoFrequency / (float)sampleRate;
            if (lfoPhase >= 2.0f * juce::MathConstants<float>::pi)
                lfoPhase -= 2.0f * juce::MathConstants<float>::pi;

            delayBufferWritePosition = (delayBufferWritePosition + 1) % delayBuffer.getNumSamples();
        }
    }

    visualBuffer.setSize(1, buffer.getNumSamples(), false, false, true);
    visualBuffer.copyFrom(0, 0, buffer.getReadPointer(0), buffer.getNumSamples());  
}

//==============================================================================
bool FactorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FactorAudioProcessor::createEditor()
{
    return new FactorAudioProcessorEditor (*this);
}

//==============================================================================
void FactorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FactorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FactorAudioProcessor();
}
