/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FilterSynth.h"
#include "Oscillators.h"
#include "DelayLine.h"
#include  "Envelope.h"

//==============================================================================
/**
*/
class EvolvingDroneAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    EvolvingDroneAudioProcessor();
    ~EvolvingDroneAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    bool debug = 1;
    int debugResolution = 200.0f;

    
    
    std::vector<Phasor> oscillators;
    
    std::vector<float> leftChannelSignals; // vector to store a sample for all the audio sources to be resolved in the left channel
    std::vector<float> rightChannelSignals;// vector to store a sample for all the audio sources to be resolved in the right channel

    Envelope envelope;
    SineOsc carrier;
    SineOsc modulator;
    DelayLine delay;
    LFO modulatorLfo;
    
    float carrierCentralFrequency;
    float modulatorCentralFrequency;
    float sr;

    juce::Random random;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EvolvingDroneAudioProcessor)
};
