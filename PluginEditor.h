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
class MyDelayAudioProcessorEditor : public juce::AudioProcessorEditor,
                                    private juce::Slider::Listener //

{
public:
    MyDelayAudioProcessorEditor (MyDelayAudioProcessor&);
    ~MyDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    void sliderValueChanged(juce::Slider* slider) override;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MyDelayAudioProcessor& audioProcessor;
    
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyDelayAudioProcessorEditor)
};
