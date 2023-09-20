//
//  SynOscillator.h
//  SynloveSynth
//
//  Created by Derek Nadeau on 9/14/23.
//  Copyright © 2023 Synlove DSP. All rights reserved.
//

#include <stdio.h>
#include <math.h>

class SynOscillator
{
    
public:
    
    SynOscillator();
    SynOscillator(float sampleRate, float  freq);
    SynOscillator(float sampleRate, double freq);
    ~SynOscillator();
    
    void on();
    void off();
    
    float getFrequency()              { return f; }
    void  setFreqeuncy(float newFreq) { f = newFreq; }
    
    void  setSampleRate(float newSampleRate) { SR = newSampleRate; }
    
    float sinewave();
    
    float sawtoothwave();
    float sawtoothwave(int numHarmonics);
    
    float squarewave();
    float squarewave(int numHarmonics);
    
    
    
private:
    
    float f;            // frequency
    float SR = 48000;   // sample rate
    float s  = 0;       // sample number
    float w  = 2.0 * M_PI;
    bool active = true;
    
    
};

