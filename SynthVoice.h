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
        
        //osc.on();
        frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber, tuningOfA440);
        level = velocity;
        
        osc.setSampleRate(sampleRate);
        osc.setFreqeuncy (frequency);
        
        std::cout << midiNoteNumber << std::endl;
        
        
    }

    void stopNote(float velocity, bool allowTailOff)
    {
        clearCurrentNote();
        velocity = 0;
        level = 0;
        frequency = 0;
        //osc.off();
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
            
            float wave = osc.sinewave() * level;
            
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
    
    
private:
    
    double frequency;
    double tuningOfA440 = 440.0;
    double sampleRate;
    float level;
    
    SynOscillator osc;
    
};
