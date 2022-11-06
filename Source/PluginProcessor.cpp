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

    //initialize carrier sinewave
    carrier.setSampleRate(sampleRate);
    carrier.setFrequency(440);
    carrierCentralFrequency = carrier.getFrequency();
    
    //initialize modulator sinewave
    modulator.setSampleRate(sampleRate);
    modulator.setFrequency(431.0f);
    modulatorCentralFrequency = modulator.getFrequency();

    
    //initialize an LFO that will modulate the modulator
    modulatorLfo.setSampleRate(sampleRate);
    modulatorLfo.setFrequency(0.001f);
    modulatorLfo.mapLFO(10.0f, -10.0f);
    
    envelope.setSampleRate(sampleRate);
    
    sr = sampleRate;
    
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
    
    
    //activate debug flag
    bool debug = 0;
    
    int debugResolution = 200.0f;
    int debugResolutionCount = 1;

    bool production = 1; //activate production enviroment
    bool testing = 0 ;   //activate testing enviroment
    
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
        //debug handler
        bool displayDebug = 0;
        
        if (debugResolution % debugResolutionCount == 1)
            displayDebug = 1;
        
        if (testing){
            float envelopeProcess = envelope.process();
            
            if (debug && displayDebug) std::cout << "envelope.getTimeFlow()-->" << envelope.getTimeFlow() << " || envelopeProcess-->" << envelopeProcess << " || envelope.getNodeName() -->"<< envelope.getNodeName() <<"\n\n";
            
        }

        
        if (production){


            float envelopeProcess = envelope.process();
            
            //initialize variables
            float leftSample = 0.0f;
            float rightSample = 0.0f;
            float fmWave = 0.0f;
            float carrierFrequency = 0.0f;
            float modulationIntensity = 5.0f;
            
            //set safety gain
            float safetyGain = 0.4f;
            
            //shall the modulator wave frequency get modulated by the lfo?
            bool modulateTheModulator = 1;
            
            //if modulateTheModulator, the modulatro wave frequency will get modulated as well
            if (modulateTheModulator)
            {
                //process the LFO that moduates the frequency of the modulator and store it in processedModulatorLfo
                float processedModulatorLfo = modulatorLfo.process();
                
                //store the modulator frequency, modulated by adding the lfo value
                float modulatorFrequency = modulatorCentralFrequency + processedModulatorLfo;
                
                //update the modulator frequency
                modulator.setFrequency(modulatorFrequency);
                if(debug && displayDebug) std::cout << "modulatorFrequency " << modulatorFrequency << "\n\n";
                if(debug && displayDebug) std::cout << "LFO freq" << modulatorLfo.getFrequency() << "\n\n";
                if(debug && displayDebug) std::cout << "LFO " << processedModulatorLfo << "\n\n";


            }
            

            
            //process the modulator and store it in processedModulator
            float processedModulator = modulator.process();
            float intensifiedModulator = processedModulator * modulationIntensity;
            
            //store the carrier modulated frequency
            carrierFrequency = carrierCentralFrequency * intensifiedModulator;

            //update the carrier frequency
            carrier.setFrequency(carrierFrequency);
            
            if(debug && displayDebug) std::cout << "carrierFrequency " << carrierFrequency << "\n\n";
            if(debug && displayDebug) std::cout << "modulatorOutput " << processedModulator << "\n\n";

            
            //process the carrier wave
            fmWave = carrier.process();

            leftChannelSignals.push_back(fmWave);
            rightChannelSignals.push_back(fmWave);

            

            //sum each line of the leftChannelSignals vector together to obtain the unscaled leftSample
            for(int j = 0; j < leftChannelSignals.size(); j++ )
            {
                leftSample += leftChannelSignals[j];
            }

            //divide the leftSample value by the number of elements contained within the leftChannelSignals vector,
            //to scale down the signal between -1 and 1
            leftSample /= leftChannelSignals.size();

            //randomly retrigger envelope
            if (random.nextFloat() * 100.0f < 0.0001f)
            {
                envelope.trigger();
            }
            
            
            //apply safety gain
            leftSample *= safetyGain;

            //sum each line of the leftChannelSignals vector together
            for(int j = 0; j < rightChannelSignals.size(); j++ )
            {
                rightSample += rightChannelSignals[j];
            }
            //divide the leftSample value by the number of elements contained within the leftChannelSignals vector,
            //to scale down the signal between -1 and 1
            rightSample /= rightChannelSignals.size();
            //apply safety gain
            rightSample *= safetyGain;

            leftChannel[i] = leftSample * envelopeProcess;    //apply generated sample to left channel
            rightChannel[i] = rightSample * envelopeProcess;  //apply generated sample to right channel
            
            //reset the channel vectors
            rightChannelSignals.clear();
            leftChannelSignals.clear();
            
            //incrementy debug resolution tool
        }
        debugResolutionCount++;
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
