//
//  SynOscillator.hpp
//  SynloveSynth
//
//  Created by Derek Nadeau on 9/14/23.
//  Copyright © 2023 S*ynlove DSP. All rights reserved.
//

#ifndef SynOscillator_hpp
#define SynOscillator_hpp

#include <stdio.h>
#include <math.h>

#endif /* SynOscillator_hpp */

class SynOscillator
{
    
public:
    
    SynOscillator(float sampleRate, float  freq);
    SynOscillator(float sampleRate, double freq);
    ~SynOscillator();
    
    float getFrequency() { return f; }
    void  setFreqeuncy(float newFreq) { f = newFreq; }
    
    void  setSampleRate(float newSampleRate) { SR = newSampleRate; }
    
    double sinewave();
    
    
    
private:
    
    float f = 440;  // frequency
    float SR;       // sample rate
    float t = 0;    // time
    float w = 2.0 * M_PI;
    
    
};

