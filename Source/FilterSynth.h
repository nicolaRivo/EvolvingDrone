/*
  ==============================================================================

    FilterSynth.h
    Created: 25 Oct 2022 2:20:58pm
    Author:  Nicola Rivosecchi

  ==============================================================================
*/
#pragma once
#include "Oscillators.h"


class FilterSynth
{
public:
    
    ///set the sample rate to the lfo and to the squareWave oscillator
    void setSampleRate(float sr)
    {
        sampleRate = sr;
        squareOsc.setSampleRate(sr);
        squareOsc.setFrequency(oscFrequency); //not sure wether this is necessary?
        lfo.setSampleRate(sr);
        lfo.setFrequency(lfoFrequency);  //not sure wether this is necessary?
    }
    
    ///set the lfo frequency
    void setLfoFrequency(float f)
    {
        lfoFrequency = f;
        lfo.setFrequency(lfoFrequency);
    }
    
    ///set the square wave oscillator frequency
    void setOscFrequency(float f)
    {
        oscFrequency = f;
        squareOsc.setFrequency(oscFrequency);
    }
    
    ///set the resonance for the filter
    void setResonance(float r)
    {
        resonance = r;
    }
    
    ///set the minimum cutoff point for the oscillating filter
    void setMinCutoff(float mco)
    {
        minCutoff = mco;
    }

    ///set the maximum cutoff point for the oscillating filter
    void setMaxCutoff(float mco)
    {
        maxCutoff = mco;
    }
    
    ///set the gain for the filter
    void setGain(float g)
    {
        gain = g;
    }
    
    ///randomize all the parameters
    void randomizeParams()
    {
        setOscFrequency(random.nextFloat() * 2000.0f);
        setLfoFrequency(random.nextFloat() * 2.0f);
        setResonance(random.nextFloat() * 10.0f);
        setMaxCutoff (random.nextFloat() * 20000.0f);
        setMinCutoff (random.nextFloat() * maxCutoff);
        setGain (random.nextFloat() * 0.9f);
    }
    
    ///generate a filtered square wave sample
    float process ()
    {
        float processedLfo =lfo.process(); //lfo oscillating between ±1
        float positiveLfo =(processedLfo/2) + 0.5f) //shift the lfo range between 0 and 1 (negative cutoff values would break the filter)
        float cutoff = positiveLfo * (maxCutoff - minCutoff) + minCutoff;//map the oscillating cutoff to the desired range
        
        filter.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, cutoff, resonance)); //set the filter
        
        float rawSquareWave = squareOsc.process() * gain ;//generate the required square wave sample
        
        float filteredSquare = filter.processSingleSampleRaw(rawSquareWave); //apply the filter to the square wave
        
        return filteredSquare;
    }
    
private:
     //Filtered Square Wave//
    //====================//
    SquareOsc squareOsc;
    SineOsc lfo;
    juce::IIRFilter filter;
    
    float sampleRate;
    float oscFrequency = 500.0f;
    float lfoFrequency = 0.4f;
    float resonance = 20.0f;
    float maxCutoff = 1900.0f;
    float minCutoff = 0.1f;
    float gain = 0.5f;
    
    juce::Random random;
};

