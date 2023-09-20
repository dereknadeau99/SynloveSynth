/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "SynthVoice.h"

//==============================================================================
/**
*/
class SynloveSynthAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    
    float delayTimeVal;
    float delayFeedbackVal;
    float drywetVal;
    float panVal {0.5};
    float bpmVal;
    
    static float maxDelayTime; // in seconds
    
    //juce::AudioProcessorValueTreeState tree;
    
    //==============================================================================
    SynloveSynthAudioProcessor();
    ~SynloveSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //===================================PROCC FUNCS======================================
    
    
    
    //===================================DELAY FUNCS======================================
    void effectDelayProcessing(const int channel, juce::AudioBuffer<float>& audioBuffer, const float* audioBufferWritePointer, const float* audioBufferReadPointer, const float* delayBufferReadPointer, const int audioBufferLength, const int delayBufferLength);
    
    void effectDelayProcessingPost(const int audioBufferLength, const int delayBufferLength);
    
    void fillDelayBuffer(int channel, const int BufferLength, const int delayBufferLength, const float* audioBufferReadPointer, const float* delayBufferReadPointer);

    void copyFromDelayBuffer(int channel, juce::AudioBuffer<float>& audioBuffer, const int audioBufferLength, const int delayBufferLength, const float* audioBufferReadPointer, const float* delayBufferReadPointer);
    
    void feedbackDelay(int channel, const int audioBufferLength, const int delayBufferLength, const float* audioBufferWritePointer);
    
    //===================================SYNTH FUNCS=======================================
    
    void setEnvelope(float attack, float decay, float sustain, float release);
    void setWaveform(int waveID);
        

private:

    // DELAY VARS
    juce::AudioBuffer<float> delayBuffer;
    juce::AudioBuffer<float> dryAudioBufferCopy;
    int delayWritePosition { 0 };
    int mSampleRate { 44100 };
    int maxVoices = 5;
    
    // SYNTH VARS
    juce::Synthesiser mySynth;
    double lastSampleRate;
        

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynloveSynthAudioProcessor)
};
