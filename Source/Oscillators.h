//
//  Oscillators.h
//  Challange 4
//
//  Created by Nicola Rivosecchi on 24/10/2022.
//

#ifndef Oscillators_h
#define Oscillators_h


class Phasor
{
#include <cmath>                // for the sin() and tan() function
    
public:
    
    float process()
    {
        float processedPhase;
        phase += phaseDelta;
        
        if(phase > 1.0f)
            phase =-1;
        
        processedPhase = output(phase);
        return processedPhase;
    };
    
    float modulatePhase ( float modPhase, float LFO = 1, float AMT = 1)
    {
        float processed = process();
        processed += modPhase * LFO * AMT;
        return processed;
    };
    
    virtual float output (float outputValue){
        return outputValue;
    }
    /**
     modulates the phasor phase by multiplying it by an input value
     */

    void setSampleRate(float SR)
    {
        sampleRate = SR;
    }
    
    virtual void setFrequency(float freq)
    {
        frequency = freq;
        phaseDelta = frequency  / float(sampleRate);
    }
    
    void setOscKind(std::string Name)
    {
        oscKind = Name;
    }
    
    int type = 3 ;
    
    
    float getPhase()
    {
        return phase;
    };
    
private:
    std::string oscKind = "" ;
    int sampleRate = 44100;
    float frequency = 440;              // the frequency of the tone to generate
    float phase = 0.0f;                 // keep track of the phase of our oscillator
    float phaseDelta = frequency / sampleRate;                   // phaseDelta - how much increment the phase by for each sample

};

 
// CHILD Class
class SineOsc : public Phasor
{
public:
    //Builder Function assigns the correct OscKind to the instance
    SineOsc()
    {
        setOscKind("Sine Wave");
    }
    
    float output (float phase) override
    {
        float outputValue = sin(phase * 2 * 3.141593);
        return outputValue;
    }
    
    
};

// CHILD Class
class TriOsc : public Phasor
{
public:
    //Builder Function assigns the correct OscKind to the instance
    TriOsc()
    {
        setOscKind("Triangle Wave");
    }
    
    float output (float phase) override
    {
        return fabsf(phase - 0.5f) -0.5f;
    }
};

// CHILD Class
class SquareOsc : public Phasor
{
public:
    //Builder Function assigns the correct OscKind to the instance
    SquareOsc()
    {
        setOscKind("Square Wave");
    }
    
    float output (float phase) override
    {
        float outputValue = 0.5f;
        if (phase < pulseWidth)
            outputValue = -0.5f;
        return outputValue;
    }
    
    void setPulseWidth(float pw)
    {
         pulseWidth = pw;
    }
private:
    float pulseWidth = 0.5f;
};


// CHILD Class
class SmoothSquareOsc : public Phasor
{
public:
    //Builder Function assigns the correct OscKind to the instance
    SmoothSquareOsc()
    {
        setOscKind("Smooth Square Wave");
    }
    
    float output (float phase) override
    {
        float outputValue = 0.5f;
        if (phase < pulseWidth)
            outputValue = -0.5f;
        
        
        return tan(outputValue);
    }
    
    void setPulseWidth(float pw)
    {
         pulseWidth = pw;
    }
private:
    float pulseWidth = 0.5f;
};



class LFO : public Phasor
{
public:
    void setFrequency(float freq) override
    {
        assert((freq<20.0f)); // frequency i
    }

};

#endif /* Oscillators_h */
