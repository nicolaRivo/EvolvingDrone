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
    void setSampleRate(float sr)
    {
        sampleRate = sr;
        squareOsc.setSampleRate(sr);
        squareOsc.setFrequency(oscFrequency);
        lfo.setSampleRate(sr);
        lfo.setFrequency(lfoFrequency);
    }
    
    void setLfoFrequency(float f)
    {
        lfoFrequency = f;
        lfo.setFrequency(lfoFrequency);
    }
    
    void setOscFrequency(float f)
    {
        oscFrequency = f;
        squareOsc.setFrequency(oscFrequency);
    }
    
    void setResonance(float r)
    {
        resonance = r;
    }
    
    void setMinCutoff(float mco)
    {
        minCutoff = mco;
    }

    void setMaxCutoff(float mco)
    {
        maxCutoff = mco;
    }
    
    void setGain(float g)
    {
        gain = g;
    }
    
    void randomizeParams()
    {
        setOscFrequency(random.nextFloat() * 2000.0f);
        setLfoFrequency(random.nextFloat() * 2.0f);
        setResonance(random.nextFloat() * 10.0f);
        setMaxCutoff (random.nextFloat() * 20000.0f);
        setMinCutoff (random.nextFloat() * maxCutoff);
        setGain (random.nextFloat() * 0.9f);
    }
    
    float process ()
    {
        float cutoff = ((lfo.process()/2) + 0.5f) * (maxCutoff - minCutoff) + minCutoff;
        filter.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, cutoff, resonance));
        float rawSquareWave = squareOsc.process() * gain ;//generate the required sample
        float filteredSquare = filter.processSingleSampleRaw(rawSquareWave);
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

