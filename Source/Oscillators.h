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
    
    
    float getFrequency()
    {
        return frequency;
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
        Phasor::setFrequency(freq);
    }
    
    float output (float phase) override
    {
        float outputValue = sin(phase * 2 * 3.141593);
        
        
        
        //scale the output value between the desired min and max
        outputValue *= (maxValue - minValue) / 2;  //   set the size of the fluctuation
        outputValue += (minValue + maxValue) / 2;   //    shift the fluctuation to the correct numeric region

        //multiply output value by the scaleIndex
        outputValue *= scaleIndex;
        
        assert((minValue < maxValue)); //catch potential troublesome condition fo min being larger than max
        
        
        //truncate the output if overtakes set min/max bounduaries
        if (outputValue < minValue)
            outputValue=minValue;
        
        if (outputValue > maxValue)
            outputValue=maxValue;
        
        return outputValue;
    }
    /**
     this function sets the mapping parameters
     @param max float, maximum value reached by the LFO
     @param min float, minimum value reached by the LFO
     @param scaleIndex float, multiplies the lfo values, resulting in a magnified and clipped LFO sinewave if multiplied by a number larger than 1 and a shrunk  LFO sinewave if multiplied by a number between 0 and 1
     */
    
    void mapLFO(float max, float min = 0.00001f, float scale = 1.0f)
    {
        maxValue = max;
        minValue = min;
        scaleIndex = scale;
    };
    
    
private:
    
    float maxValue = 1.0f;
    float minValue = 0.00001f;
    float scaleIndex = 1.0f;
    
    
};

#endif /* Oscillators_h */
