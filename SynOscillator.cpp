//
//  SynOscillator.cpp
//  SynloveSynth
//
//  Created by Derek Nadeau on 9/14/23.
//  Copyright © 2023 Synlove DSP. All rights reserved.
//

#include "SynOscillator.h"
#include <math.h>


SynOscillator::SynOscillator(float sampleRate, float  freq)
{
    SR = sampleRate;
    f  = freq;
}

SynOscillator::SynOscillator(float sampleRate, double freq){
    
    SR = sampleRate;
    f  = static_cast<float>(freq);
    
}

SynOscillator::~SynOscillator() {}

double SynOscillator::sinewave()
{
    
    // 2pi * freq * timestep / samplerate
    float radians = w * f * t / SR;
    
    // move time forward (max val of int t means we can hold note down for 93 mins)
    t++;
    
    // return correct value
    return sin(radians);
    
}

