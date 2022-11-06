/*
  ==============================================================================

    Envelope.h
    Created: 5 Nov 2022 9:41:23pm
    Author:  Nicola Rivosecchi

  ==============================================================================
*/

#pragma once

class Envelope {
    
public:
    
    void setSampleRate (float sr)
    {

        seconds = 1.0f / float(sr);  //calculates how many seconds each sample lasts
        
        if(debug) std::cout << "\nClass Envelope Debug == seconds " << seconds;

        sampleRate = sr;
    }
    
    float getTimeFlow()
    {
        return timeFlow;
    }

    
    float gainRamp (float startGain = 0.0f, float endGain = 1.0f, float nodeTime = 1.0f)
    {
        assert (1.0f >= endGain  && endGain >= 0.0f && 1.0f >= startGain && startGain >= 0.0f);
        
        float gainDifference = (endGain - startGain);
        
        float rampStep = gainDifference / (nodeTime * sampleRate); //size of each gain increase/decrease for process cycle
        
        if(debug) std::cout << "\nClass Envelope Debug == rampStep is " << rampStep;
        
        
        if (startGain == endGain)
            return gain;
        
        gain += rampStep;
        return gain;
    }
    
    void setNodeName (std::string newNode)
    {
        nodeName = newNode;
    };
    
    std::string getNodeName()
    {
        return nodeName;
    };
    
    float process ()
    {
        if (gain > 1.1f)
            std::cout << "gain is " << gain << "\n";
        assert(gain < 1.1f);
   
        timeFlow += seconds;
        
        float envNode = 0.0f; //keeps track of the node of the envelope we are on adding the duration of each segment together

            
        envNode = attack;
        if(debug) std::cout << "\n\nClass Envelope Debug == nattack " << attack;
        if(debug) std::cout << "\nClass Envelope Debug == timeFlow " << timeFlow;
        if(debug) std::cout << "\nClass Envelope Debug == envNode " << envNode;


        if (timeFlow < envNode)
        {
            setNodeName("attack");
            if(debug) std::cout <<"\nClass Envelope Debug: stage-->" << nodeName << " || timeFlow-->" << timeFlow;
            return gainRamp(0.0f, 1.0f, attack);
        }
        
        envNode +=decay;
        
        if (timeFlow < envNode)
        {
            setNodeName("decay");
            if(debug) std::cout <<"\nClass Envelope Debug: stage-->" << nodeName << " || timeFlow-->" << timeFlow;
            return gainRamp(1.0f, sustain, decay);
        }

        
        envNode +=hold;
        
        if (timeFlow < envNode)
        {
            setNodeName("hold");
            if(debug) std::cout <<"\nClass Envelope Debug: stage-->" << nodeName << " || timeFlow-->" << timeFlow;
            return gainRamp(sustain, sustain, hold);
        }
        
        envNode +=release;
        
        if (timeFlow < envNode)
        {
            setNodeName("release");
            if(debug) std::cout <<"\nClass Envelope Debug: stage-->" << nodeName << " || timeFlow-->" << timeFlow;
            return gainRamp(sustain, 0.0f, release);
        }

            setNodeName("ended");
            if(debug) std::cout <<"\nClass Envelope Debug: stage-->" << nodeName << " || timeFlow-->" << timeFlow;
            gain = 0.0f;
            return gain;
    }
    
    void trigger(){
        if (gain == 0.0f)
        {
            timeFlow = 0.0f; //resets the count of seconds, retriggering the envelope
            gain = 0.0f;
        }
    }
    
private:
    bool debug = 0;
    
    //Envelope engine
    float gain = 0.0f;
    float sampleRate;
    double seconds; // lapse of time each sample is worth in seconds----(s)
    float timeFlow = 0.0f;          //seconds passed since triggered--------------------(s)
    std::string nodeName = "uninitialized";
    
    //Envelope Parameters
    float attack = 1.0f;            //attack time in seconds----------------------------(s)
    float decay = 1.0f;             //decay time in seconds-----------------------------(s)
    float sustain = 0.5f;           //gain applied to the sustain-----------------------(%)
    float hold = 1.0f;              //time the sustain is hold in seconds---------------(s)
    float release = 2.5f;           //attack time in seconds----------------------------(s)
};


