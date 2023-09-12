/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyDelayAudioProcessorEditor::MyDelayAudioProcessorEditor (MyDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (700, 700);
    
    // delay time slider
    delayTimeSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    delayTimeSlider.setRange (0.0, 5.0, 0.01);
    
    // settextboxstyle( location, readonly, width, height
    delayTimeSlider.setTextBoxStyle (juce::Slider::TextBoxBelow,
                                     false,
                                     90,
                                     20);
    delayTimeSlider.setPopupDisplayEnabled (false, false, this);
    delayTimeSlider.setTextValueSuffix (" Seconds");
    delayTimeSlider.setValue(1.0);
 
    // this function adds the slider to the editor
    addAndMakeVisible (&delayTimeSlider);
    
    // add the listener to the slider
    delayTimeSlider.addListener (this);
    
    // add label to slider
    addAndMakeVisible (delayTimeLabel);
    delayTimeLabel.setText ("Delay Time", juce::dontSendNotification);
    delayTimeLabel.attachToComponent (&delayTimeSlider, false);
    delayTimeLabel.setJustificationType(juce::Justification::horizontallyCentred);
    
    
    
    // ===========================================================================
    // feedback time slider
    feedbackSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    feedbackSlider.setRange (0.0, 100.0, 0.01);
    feedbackSlider.setTextBoxStyle (juce::Slider::TextBoxBelow,
                                    false,
                                    90,
                                    20);
    feedbackSlider.setPopupDisplayEnabled (false, false, this);
    feedbackSlider.setTextValueSuffix ("%");
    feedbackSlider.setValue(50.0);
 
    // this function adds the slider to the editor
    addAndMakeVisible (&feedbackSlider);
    
    // add the listener to the slider
    feedbackSlider.addListener (this);
    feedbackLabel.setText ("Feedback Amount", juce::dontSendNotification);
    feedbackLabel.attachToComponent (&feedbackSlider, false);
    feedbackLabel.setJustificationType(juce::Justification::horizontallyCentred);
    
    
    // ===========================================================================
    // dry/wet slider
    drywetSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    drywetSlider.setRange (0.0, 100.0, 0.01);
    drywetSlider.setTextBoxStyle (juce::Slider::TextBoxBelow,
                                  false,
                                  90,
                                  20);
    drywetSlider.setPopupDisplayEnabled (false, false, this);
    drywetSlider.setTextValueSuffix ("%");
    drywetSlider.setValue(50.0);
 
    // this function adds the slider to the editor
    addAndMakeVisible (&drywetSlider);
    
    // add the listener to the slider
    drywetSlider.addListener (this);
    drywetLabel.setText ("Dry/Wet Amount", juce::dontSendNotification);
    drywetLabel.attachToComponent (&drywetSlider, false);
    drywetLabel.setJustificationType(juce::Justification::horizontallyCentred);
}

MyDelayAudioProcessorEditor::~MyDelayAudioProcessorEditor()
{
}

void MyDelayAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    audioProcessor.delayTimeVal = delayTimeSlider.getValue();
    audioProcessor.delayFeedbackVal = feedbackSlider.getValue() / 100.0; // convert from percentage to decimal
    audioProcessor.drywetVal = drywetSlider.getValue() / 100.0 ; // convert from percentage to decimal
}

//==============================================================================
void MyDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

//    g.setColour (juce::Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void MyDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    int h = getHeight();
    int w = getWidth();
    int sliderDim = 100;
    
    
    delayTimeSlider.setBounds (static_cast<int>(1.0*w/4.0-sliderDim/2), // x pos
                               static_cast<int>(h/2-sliderDim/2),       // y pos
                               sliderDim,sliderDim);                    // dimensions
    
    feedbackSlider.setBounds  (static_cast<int>(2.0*w/4.0-sliderDim/2), // x pos
                               static_cast<int>(h/2-sliderDim/2),       // y pos
                               sliderDim,sliderDim);                    // dimensions
    
    drywetSlider.setBounds    (static_cast<int>(3.0*w/4.0-sliderDim/2), // x pos
                               static_cast<int>(h/2-sliderDim/2),       // y pos
                               sliderDim,sliderDim);                    // dimensions
}
