/*
  ==============================================================================

    DelayLine.h
    Created: 5 Nov 2022 5:03:23pm
    Author:  Nicola Rivosecchi

  ==============================================================================
*/

#pragma once

class DelayLine
{
public:
    ///set the maximum size of the delay line
    void setSizeInSamples (int newSize)
    {
        size = newSize; //store the new size
        
        //free up existing data
        if (data != nullptr) 
            delete[] data;
        
        data = new float[size]; //initialize our array
        
        
        //make sure that our new data array is empty
        for (int i = 0; i < size; i++)
        {
            data[i] = 0.0f;
        }
    }
    
    ///set the delay time
    void setDelayTimeInSamples (int newDelayTime)
    {
    }
    
    ///store a sample
    void writeDataToDelayLine (float inputSample)
    {
        
    }
    
    ///advance play and record head, wraping if necessary
    void updateReadAndWriteHead ()
    {
        
    }
    
    ///call every sample to
    ///-- store the new sample
    ///-- advance play and write index
    ///-- return the value at play index
    float process(float inputSample)
    {
        return inputSample;
    }


    
    
private:
    int readIndex = 0;  //read position as an index
    
    int writeIndex = 0;  //write position as an index
    
    int size;           //maximum possible delay time
    
    float* data = nullptr;        //store input audio data
    
    int delayTime;      //delay time in samples

};
