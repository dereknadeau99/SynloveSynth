/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SynloveSynthAudioProcessorEditor: public juce::AudioProcessorEditor,
                                        private juce::Slider::Listener
{
public:
    SynloveSynthAudioProcessorEditor (SynloveSynthAudioProcessor&);
    ~SynloveSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    void sliderValueChanged(juce::Slider* slider) override;
    
    void configureEnvelopeControls();
    void configureEnvelopePositions(int h, int w);
    
    void configureDelayControls();
    void configureDelayPositions(int h, int w);
        
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynloveSynthAudioProcessor& audioProcessor;
    
    //======SYNTH CONTROLS======
    
    juce::Slider attackSlider;
    
    juce::Label  attackLabel;
    
    juce::Slider decaySlider;
    juce::Label  decayLabel;
    
    juce::Slider sustainSlider;
    juce::Label  sustainLabel;
    
    juce::Slider releaseSlider;
    juce::Label  releaseLabel;
    
    //======DELAY CONTROLS======
    juce::Slider delayTimeSlider;
    juce::Label  delayTimeLabel;
    
    juce::Slider feedbackSlider;
    juce::Label  feedbackLabel;
    
    juce::Slider drywetSlider;
    juce::Label  drywetLabel;
    
    juce::Slider panSlider;
    juce::Label  panLabel;
    
    juce::Slider bpmSlider;
    juce::Label  bpmLabel;
        
    //    juce::Button[4] bpmDivisionButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynloveSynthAudioProcessorEditor)
};
