/*
  ==============================================================================

    SynthVoice.h
    Created: 14 Sep 2023 5:43:12pm
    Author:  Derek Nadeau

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "SynOscillator.h"
#include <math.h>


class SynthVoice : public juce::SynthesiserVoice
{
  
public:
    
    SynthVoice();

    bool canPlaySound(juce::SynthesiserSound* sound)
    {
        // can play sound if input matches our subclass, and is not null
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }
    
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
    {
        
        CURR_ENV = ATTACK;
        
        frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber, tuningOfA440);
        initVelocity = velocity; // / 127.0;
        
        osc.setSampleRate(sampleRate);
        osc.setFreqeuncy (frequency);
        
    }

    void stopNote(float velocity, bool allowTailOff)
    {
        CURR_ENV = RELEASE;
    }
    
    bool isVoiceActive ()
    {
        return true; //TODO
    }
    
    void pitchWheelMoved(int newPitchWheelValue)
    {
        
    }
    
    void controllerMoved(int controllerNumber, int newControllerValue)
    {
        
    }

    void aftertouchChanged (int newAftertouchValue)
    {
        
    }
    
    void channelPressureChanged (int newChannelPressureValue)
    {
        
    }
    
    void renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
    {
        
        for (int sample = 0; sample < numSamples; ++sample)
        {
            
            // get value from oscillator & apply linear attack envelope and initial level
            float wave = osc.sawtoothwave() * envelope() * initVelocity;
            
            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
               outputBuffer.addSample(channel, startSample, wave);
            }
            
            ++startSample;
            
        }
        
        
    }
    
    void renderNextBlock (juce::AudioBuffer<double> &outputBuffer, int startSample, int numSamples)
    {
        
        
    }
    
    void setCurrentPlaybackSampleRate (double newRate)
    {

        sampleRate = newRate;
        osc.setSampleRate(sampleRate);

    }
    
    bool isPlayingChannel (int midiChannel)
    {
        return true; // TODO
    }
    
    void setEnvelope(float attack, float decay, float sustain, float release)
    {
        ADSR[ATTACK]  = attack;
        ADSR[DECAY]   = decay;
        ADSR[SUSTAIN] = sustain;
        ADSR[RELEASE] = release;
    }
    
    // returns appropriate gain percentage based on envelope position
    float envelope() {
        
        if (CURR_ENV == NOTE_OFF) { return 0; }
        
        float gain = 0;
        
        // check envelope in reverse order (RSDA) so that no advancement messes with processing
        
        if (CURR_ENV == RELEASE)
        {
            
            float   samplesToRelease = (ADSR[RELEASE] * sampleRate) - envelopeSampleCounter;
            gain = (samplesToRelease / (ADSR[RELEASE] * sampleRate)) * ADSR[SUSTAIN];
            
            if (gain <= 0.0)
            {
                gain = 0;
                CURR_ENV = NOTE_OFF;
                envelopeSampleCounter = 0;
                clearCurrentNote(); // IMPORTANT
                return gain;
            }
        }
        
        if (CURR_ENV == SUSTAIN)
        {
            // no need to count samples in sustain
            return ADSR[SUSTAIN];
        }
        
        if (CURR_ENV == DECAY) {
            
            // num of samples it will take to decay fully
            float   decayLength = (ADSR[DECAY] * sampleRate);
            
            // num of samples left in decay envelope
            float   remainingSamplesToDecay = decayLength - envelopeSampleCounter;
            
            // multiply gain change by time elapsed
            gain = (1 - ADSR[SUSTAIN]) * (remainingSamplesToDecay / decayLength);
            
            if (gain < ADSR[SUSTAIN])
            {
                gain = ADSR[SUSTAIN];
                CURR_ENV = SUSTAIN;
                envelopeSampleCounter = 0;
                return gain;
                
            }
        }
        
        if (CURR_ENV == ATTACK) {
            
            // attack in s divided sample rate should be == 1 at end of attack
            gain = envelopeSampleCounter / (ADSR[ATTACK] * sampleRate);
            
            if (gain >= 1)
            {
                gain = 1.0;
                CURR_ENV = DECAY;
                envelopeSampleCounter = 0;
                return gain;
                
            }
        }
        
        envelopeSampleCounter += 1.0;
        return gain;
        
    }
    
private:
    
    double frequency;
    double tuningOfA440 = 440.0;
    double sampleRate;
    float  initVelocity;
    float  envelopeSampleCounter = 0.0;
    
    // seconds, seconds, dB, seconds
    float ADSR[4] = {0.5, 0.5, 0.5, 2.5};
    int ATTACK   = 0;
    int DECAY    = 1;
    int SUSTAIN  = 2;
    int RELEASE  = 3;
    int CURR_ENV = 0;
    int NOTE_OFF =-1;
    
    SynOscillator osc;
    
};
