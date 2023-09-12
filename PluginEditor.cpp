/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynloveSynthAudioProcessorEditor::SynloveSynthAudioProcessorEditor (SynloveSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (700, 700);
    
    // delay time slider
    delayTimeSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    delayTimeSlider.setRange (0.0, 5.0);
    delayTimeSlider.setValue(0.5);
    delayTimeSlider.setNumDecimalPlacesToDisplay(2);
    delayTimeSlider.setDoubleClickReturnValue(true, 1.0);
    delayTimeSlider.setSkewFactor(0.5);
    
    
    // settextboxstyle( location, readonly, width, height
    delayTimeSlider.setTextBoxStyle (juce::Slider::TextBoxBelow,
                                     false,
                                     90,
                                     20);
    delayTimeSlider.setPopupDisplayEnabled (false, false, this);
    delayTimeSlider.setTextValueSuffix (" Seconds");
 
    // this function adds the slider to the editor
    addAndMakeVisible (&delayTimeSlider);
    
    // add the listener to the slider
    delayTimeSlider.addListener(this);
    
    // add label to slider
    addAndMakeVisible (delayTimeLabel);
    delayTimeLabel.setText ("Delay Time", juce::dontSendNotification);
    delayTimeLabel.attachToComponent (&delayTimeSlider, false);
    delayTimeLabel.setJustificationType(juce::Justification::horizontallyCentred);
    
    
    
    // ===========================================================================
    // feedback time slider
    feedbackSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    feedbackSlider.setRange (0.0, 100.0);
    feedbackSlider.setNumDecimalPlacesToDisplay(2);
    feedbackSlider.setValue(50.0);
    feedbackSlider.setDoubleClickReturnValue(true, 50.0);
    
    feedbackSlider.setTextBoxStyle (juce::Slider::TextBoxBelow,
                                    false,
                                    90,
                                    20);
    feedbackSlider.setPopupDisplayEnabled (false, false, this);
    feedbackSlider.setTextValueSuffix ("%");
    
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
    drywetSlider.setRange (0.0, 100.0);
    drywetSlider.setValue(50.0);
    drywetSlider.setNumDecimalPlacesToDisplay(2);
    drywetSlider.setDoubleClickReturnValue(true, 50.0);
    
    drywetSlider.setTextBoxStyle (juce::Slider::TextBoxBelow,
                                  false,
                                  90,
                                  20);
    drywetSlider.setPopupDisplayEnabled (false, false, this);
    drywetSlider.setTextValueSuffix ("%");
    
 
    // this function adds the slider to the editor
    addAndMakeVisible (&drywetSlider);
    
    // add the listener to the slider
    drywetSlider.addListener(this);
    drywetLabel.setText ("Dry/Wet Amount", juce::dontSendNotification);
    drywetLabel.attachToComponent (&drywetSlider, false);
    drywetLabel.setJustificationType(juce::Justification::horizontallyCentred);
    
    // ===========================================================================
    // panning slider
    panSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    panSlider.setRange (-100, 100, 1);
    panSlider.setValue(0);
    panSlider.setDoubleClickReturnValue(true, 0);
    
    panSlider.setTextBoxStyle (juce::Slider::TextBoxBelow,
                                  false,
                                  90,
                                  20);
    panSlider.setPopupDisplayEnabled (false, false, this);
    panSlider.setTextValueSuffix ("C");
    
 
    // this function adds the slider to the editor
    addAndMakeVisible (&panSlider);
    
    // add the listener to the slider
    panSlider.addListener (this);
    
    // add label to slider
    panLabel.setText ("Delay Pan", juce::dontSendNotification);
    panLabel.attachToComponent (&panSlider, false);
    panLabel.setJustificationType(juce::Justification::horizontallyCentred);
    
    
    // ===========================================================================
    // bpm label / changer
    
    bpmSlider.setSliderStyle(juce::Slider::LinearBar);
    bpmSlider.setRange(30, 200, 0.1);
    bpmSlider.setValue(120.0);
    bpmSlider.setDoubleClickReturnValue(true, 120.0);
    bpmSlider.setNumDecimalPlacesToDisplay(1);
    
    bpmSlider.setTextBoxStyle (juce::Slider::TextBoxAbove,
                                  false,
                                  90,
                                  20);
    bpmSlider.setPopupDisplayEnabled (false, false, this);
    bpmSlider.setTextValueSuffix (" BPM");
    
    bpmSlider.addListener(this);
    
    addAndMakeVisible (&bpmSlider);
    
    
    
//    bpmLabel.setText("BPM", juce::dontSendNotification);
//    bpmLabel.setJustificationType(juce::Justification::horizontallyCentred);
//    addAndMakeVisible(&bpmLabel);
    

    
    
    
    
    
}

void SynloveSynthAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    
//    float delayTimeSliderValue = delayTimeSlider.getValue();
//    float feedbackSliderValue  = feedbackSlider.getValue();
//    float drywetSliderValue    = drywetSlider.getValue();
    
//
    
    
    
    // if delay time slider was changed, update bpm, and send both to processor
    if (slider == &delayTimeSlider)
    {
        float delayTimeSliderValue = delayTimeSlider.getValue();
        
        // change from BPM to BPS to reflect delay units, and set delay to resulting value
        bpmSlider.setValue(60.0 / delayTimeSliderValue, juce::dontSendNotification);
        
        // send new values to processor
        audioProcessor.bpmVal = bpmSlider.getValue();
        audioProcessor.delayTimeVal = delayTimeSliderValue;
    }
    
    if (slider == &feedbackSlider)
    {
        audioProcessor.delayFeedbackVal = feedbackSlider.getValue() / 100.0; // convert from percentage to decimal
    }
    
    if (slider == &drywetSlider)
    {
        audioProcessor.drywetVal = drywetSlider.getValue() / 100.0 ; // convert from percentage to decimal
    }
    
    if (slider == &panSlider)
    {
        float panSliderValue = panSlider.getValue();
        
        // set pan slider text appropriately
        if (panSliderValue > 0.0)       { panSlider.setTextValueSuffix ("R"); }
        else if (panSliderValue < 0.0)  { panSlider.setTextValueSuffix ("L"); }
        else                            { panSlider.setTextValueSuffix ("C"); }
        
        // usual range is (-100, +100)
        // normalize to (0, 1) to match gain values, send to processor
        audioProcessor.panVal = (panSliderValue + 100.0) / 200.0;
    }
    
    // if bpm slider was changed, update delay time, and send both to processor
    if (slider == &bpmSlider)
    {
        
        float bpmSliderValue = bpmSlider.getValue();
        
        // change from BPM to BPS to reflect delay units, and set delay to resulting value
        delayTimeSlider.setValue(60.0 / bpmSliderValue, juce::dontSendNotification);
        
        // send new values to processor
        audioProcessor.bpmVal = bpmSliderValue;
        audioProcessor.delayTimeVal = delayTimeSlider.getValue();
        
    }
    
    
    
    
    
    
    
    // update panSlider display
    
    
    
    
}


SynloveSynthAudioProcessorEditor::~SynloveSynthAudioProcessorEditor()
{
}

//==============================================================================
void SynloveSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    //    g.setColour (juce::Colours::white);
    //    g.setFont (15.0f);
    //    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SynloveSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    int h = getHeight();
    int w = getWidth();
    int sliderDim = 100;
    
    delayTimeSlider.setBounds (static_cast<int>(1.0*w/5.0-sliderDim/2), // x pos
                               static_cast<int>(h/2-sliderDim/2),       // y pos
                               sliderDim,sliderDim);                    // dimensions
    
    feedbackSlider.setBounds  (static_cast<int>(2.0*w/5.0-sliderDim/2), // x pos
                               static_cast<int>(h/2-sliderDim/2),       // y pos
                               sliderDim,sliderDim);                    // dimensions
    
    drywetSlider.setBounds    (static_cast<int>(3.0*w/5.0-sliderDim/2), // x pos
                               static_cast<int>(h/2-sliderDim/2),       // y pos
                               sliderDim,sliderDim);                    // dimensions
    
    panSlider.setBounds       (static_cast<int>(4.0*w/5.0-sliderDim/2), // x pos
                               static_cast<int>(h/2-sliderDim/2),       // y pos
                               sliderDim,sliderDim);                    // dimensions
    
    bpmSlider.setBounds(0, 0, 200, 20);
    
}
