//
//  SynOscillator.cpp
//  SynloveSynth
//
//  Created by Derek Nadeau on 9/14/23.
//  Copyright © 2023 Synlove DSP. All rights reserved.
//

#include "SynOscillator.h"
#include <math.h>
#include <iostream>

SynOscillator::SynOscillator() {}

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

void SynOscillator::on()  { active = true;  }

void SynOscillator::off() { active = false; }

float SynOscillator::sinewave()
{
    
    //if (not active) { return 0; }
    
    // 2pi * freq * timestep / samplerate
    double radians = w * f * (s / SR);
    
    // move time forward (max val of int t means we can hold note down for 93 mins)
    s++;
    
    // return correct value
    return static_cast<float>(cos(radians));
    
}

float SynOscillator::sawtoothwave() { return sawtoothwave(10); }

float SynOscillator::sawtoothwave(int numHarmonics)
{
    
    if (not active) { return 0; }
    
    // modulo approach for sawtooth wave
    // https://thewolfsound.com/sine-saw-square-triangle-pulse-basic-waveforms-in-synthesis/#sawtooth-saw
    float t = fmod((s / SR), (1 / f));
    //std::cout << t << std::endl;
    float sample = (2 * t * f) - 1;
    
    // move time forward (max val of int t means we can hold note down for 93 mins)
    s++;
    
    // return correct value
    return sample;
    
}

float SynOscillator::squarewave() { return sawtoothwave(10); }

float SynOscillator::squarewave(int numHarmonics)
{
    
    if (not active) { return 0; }
    
    // simple approach for now, TODO
    // https://thewolfsound.com/sine-saw-square-triangle-pulse-basic-waveforms-in-synthesis/
    float sample  = (sin(w * f * (s/SR)) >= 0 ? 1 : -1);
    
    // move time forward (max val of int t means we can hold note down for 93 mins)
    s++;
    
    // return correct value
    return sample;
    
}

