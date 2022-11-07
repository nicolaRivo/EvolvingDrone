/*
  ==============================================================================

    FmSineOsc.h
    Created: 5 Nov 2022 9:07:56pm
    Author:  Nicola Rivosecchi

  ==============================================================================
*/

class FmSineOsc
{
public:
  
    
    void setSampleRate (float sampleRate)
    {
        carrier.setSampleRate(sampleRate);
        modulator.setSampleRate(sampleRate);
        modulatorLfo.setSampleRate(sampleRate);
    }
    
    /**
     set frequences for all the oscillators,
     @param carrierFreq float frequency to set the carrier to (Hz)
     @param modulatorFreq float frequency to set the carrier to (Hz); default value is carrierFreq + 1
     @param lfoFreq float frequency to set the lfo modulating the modulator's frequency to (Hz); as for any lfo, the max allowed is 20.0f.
     */
    void setAllFrequencies (float carrierFreq, float modulatorFreq = 0.0f, float lfoFreq = 0.1f)
    {
        //if the modulatorFreq has not been set or equals to 0, set it as carrierFreq + 1.0f
        if (modulatorFreq == 0.0f)
            modulatorFreq = carrierFreq + 1.0f;
                    
        carrierCentralFrequency = carrierFreq;
        
        modulatorCentralFrequency = modulatorFreq;
        
        modulatorLfo.setFrequency(lfoFreq);
    }
    
    ///set exclusively the carrier's frequency (Hz)
    void setCarrierFrequency (float f)
    {
        carrier.setFrequency(f);
    }
    
    void setCarrierCentralFrequency (float f)
    {
        modulatorCentralFrequency = f;
    }
    
    ///set exclusively the carrier's frequency
    void setModulatorFrequency (float f)
    {
        modulator.setFrequency(f);
    }
    
    ///set exclusively the modulator's Lfo frequency
    void setModulatorLfoFrequency(float f)
    {
        modulatorLfo.setFrequency(f);
    }
    
    void setModulatorLfoIntensity(float i)
    {
        lfoModulationIntensity = i;
    }
    
    void setFmIntensity(float i)
    {
        fmIntensity = i;
    }
    
    void toggleModulatorLfo(bool toggle)
    {
        modulateTheModulator = toggle;
    }
    
    void mapModulatorLfo(float max, float min, float scale = 1.0f)
    {
        modulatorLfo.mapLFO(max, min, scale);
    }

    
    
    float process()
    {
        //if modulateTheModulator, the modulatro wave frequency will get modulated as well
        if (modulateTheModulator)
        {
            //process the LFO that moduates the frequency of the modulator and store it in processedModulatorLfo
            float processedModulatorLfo = modulatorLfo.process();
            
            //store the modulator frequency, modulated by adding the lfo value
            float modulatorFrequency = modulatorCentralFrequency + processedModulatorLfo * lfoModulationIntensity;
            
            //update the modulator frequency
            modulator.setFrequency(modulatorFrequency);

        }
        
        //process the modulator and store it in processedModulator
        float processedModulator = modulator.process();
        float intensifiedModulator = processedModulator * fmIntensity;
        
        //store the carrier modulated frequency
        float carrierFrequency = carrierCentralFrequency * intensifiedModulator;

        //update the carrier frequency
        carrier.setFrequency(carrierFrequency);
        
        return carrier.process();
    }
    
private:
    
    bool modulateTheModulator = 1;
    float modulatorCentralFrequency;
    float carrierCentralFrequency;
    float fmIntensity = 1.0f;
    float lfoModulationIntensity = 1.0f ;
    SineOsc carrier;
    SineOsc modulator;
    LFO modulatorLfo;

//
//    //initialize carrier sinewave
//    carrier.setSampleRate(sampleRate);
//    carrier.setFrequency(440);
//    carrierCentralFrequency = carrier.getFrequency();
//
//    //initialize modulator sinewave
//    modulator.setSampleRate(sampleRate);
//    modulator.setFrequency(431.0f);
//    modulatorCentralFrequency = modulator.getFrequency();
//
//
//    //initialize an LFO that will modulate the modulator
//    modulatorLfo.setSampleRate(sampleRate);
//    modulatorLfo.setFrequency(0.001f);
//    modulatorLfo.mapLFO(10.0f, -10.0f);
};


#pragma once
