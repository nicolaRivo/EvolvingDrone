/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EvolvingDroneAudioProcessor::EvolvingDroneAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

EvolvingDroneAudioProcessor::~EvolvingDroneAudioProcessor()
{
}

//==============================================================================
const juce::String EvolvingDroneAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EvolvingDroneAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EvolvingDroneAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EvolvingDroneAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EvolvingDroneAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EvolvingDroneAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EvolvingDroneAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EvolvingDroneAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String EvolvingDroneAudioProcessor::getProgramName (int index)
{
    return {};
}

void EvolvingDroneAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void EvolvingDroneAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    carrier.setSampleRate(sampleRate);
    carrier.setFrequency(440);
    modulator.setSampleRate(sampleRate);
    modulator.setFrequency(441);


}

void EvolvingDroneAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EvolvingDroneAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void EvolvingDroneAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    int numSamples = buffer.getNumSamples();
    auto* leftChannel = buffer.getWritePointer(0); //channel 0 for left chan
    auto* rightChannel = buffer.getWritePointer(1);//channel 1 for right chan

    //DSP loop where numSamples corresponds to the buffer size
    for(int i = 0; i<numSamples; i++ )
    {
        float leftSample = 0;
        float rightSample = 0;

        carrier.setFrequency(carrier.getFrequency() + modulator.process())
        
        leftChannelSignals.push_back(<#const_reference __x#>)
        rightChannelSignals.push_back(<#const_reference __x#>)

        
        
        //sum each line of the leftChannelSignals vector together to obtain the unscaled leftSample
        for(int j = 0; j < leftChannelSignals.size(); j++ )
        {
            leftSample += leftChannelSignals[j];
        }
        
        //divide the leftSample value by the number of elements contained within the leftChannelSignals vector,
        //to scale down the signal between -1 and 1
        leftSample /= leftChannelSignals.size();
        
        
        //sum each line of the leftChannelSignals vector together
        for(int j = 0; j < rightChannelSignals.size(); j++ )
        {
            rightSample += rightChannelSignals[j];
        }
        //divide the leftSample value by the number of elements contained within the leftChannelSignals vector,
        //to scale down the signal between -1 and 1
        rightSample /= rightChannelSignals.size();

        leftChannel[i] = leftSample;    //apply generated sample to left channel
        rightChannel[i] = rightSample;  //apply generated sample to right channel
    }
}

//==============================================================================
bool EvolvingDroneAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* EvolvingDroneAudioProcessor::createEditor()
{
    return new EvolvingDroneAudioProcessorEditor (*this);
}

//==============================================================================
void EvolvingDroneAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EvolvingDroneAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EvolvingDroneAudioProcessor();
}
