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
    // set size before constructor finishes
    setSize (700, 700);
    
    // sets up sliders for synth envelope controls
    configureEnvelopeControls();
    
    // sets up four sliders for delay controls and bpm slider
    configureDelayControls();
    
    
}

void SynloveSynthAudioProcessorEditor::configureEnvelopeControls()
{
    
    // attack time slider
    attackSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    attackSlider.setRange (0.0, 5.0);
    attackSlider.setValue(0.2);
    attackSlider.setNumDecimalPlacesToDisplay(2);
    attackSlider.setDoubleClickReturnValue(true, 1.0);
    attackSlider.setSkewFactor(0.5);
    
    
    // settextboxstyle( location, readonly, width, height
    attackSlider.setTextBoxStyle (juce::Slider::TextBoxBelow,
                                     false,
                                     90,
                                     20);
    attackSlider.setPopupDisplayEnabled (false, false, this);
    attackSlider.setTextValueSuffix (" Seconds");
 
    // this function adds the slider to the editor
    addAndMakeVisible (&attackSlider);
    
    // add the listener to the slider
    attackSlider.addListener(this);
    
    // add label to slider
    addAndMakeVisible(attackLabel);
    attackLabel.setText ("Attack Time", juce::dontSendNotification);
    attackLabel.attachToComponent (&attackSlider, false);
    attackLabel.setJustificationType(juce::Justification::horizontallyCentred);
    
    //================================================================
    // decay time slider
    decaySlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    decaySlider.setRange (0.0, 5.0);
    decaySlider.setValue(0.5);
    decaySlider.setNumDecimalPlacesToDisplay(2);
    decaySlider.setDoubleClickReturnValue(true, 1.0);
    decaySlider.setSkewFactor(0.5);
    
    
    // settextboxstyle( location, readonly, width, height
    decaySlider.setTextBoxStyle (juce::Slider::TextBoxBelow,
                                     false,
                                     90,
                                     20);
    decaySlider.setPopupDisplayEnabled (false, false, this);
    decaySlider.setTextValueSuffix (" Seconds");
 
    // this function adds the slider to the editor
    addAndMakeVisible (&decaySlider);
    
    // add the listener to the slider
    decaySlider.addListener(this);
    
    // add label to slider
    addAndMakeVisible(decayLabel);
    decayLabel.setText ("Decay Time", juce::dontSendNotification);
    decayLabel.attachToComponent (&decaySlider, false);
    decayLabel.setJustificationType(juce::Justification::horizontallyCentred);
    
    //=========================================================================
    // attack time slider
    sustainSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sustainSlider.setRange (0.0, 100.0);
    sustainSlider.setValue(70.0);
    sustainSlider.setNumDecimalPlacesToDisplay(2);
    sustainSlider.setDoubleClickReturnValue(true, 1.0);
    //sustainSlider.setSkewFactor(0.5);
    
    
    // settextboxstyle( location, readonly, width, height
    sustainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow,
                                     false,
                                     90,
                                     20);
    sustainSlider.setPopupDisplayEnabled (false, false, this);
    sustainSlider.setTextValueSuffix ("% Gain");
 
    // this function adds the slider to the editor
    addAndMakeVisible (&sustainSlider);
    
    // add the listener to the slider
    sustainSlider.addListener(this);
    
    // add label to slider
    addAndMakeVisible(attackLabel);
    sustainLabel.setText ("Sustain Level", juce::dontSendNotification);
    sustainLabel.attachToComponent (&sustainSlider, false);
    sustainLabel.setJustificationType(juce::Justification::horizontallyCentred);
    
    //===========================================================================
    // attack time slider
    releaseSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    releaseSlider.setRange (0.0, 5.0);
    releaseSlider.setValue(0.5);
    releaseSlider.setNumDecimalPlacesToDisplay(2);
    releaseSlider.setDoubleClickReturnValue(true, 1.0);
    releaseSlider.setSkewFactor(0.5);
    
    
    // settextboxstyle( location, readonly, width, height
    releaseSlider.setTextBoxStyle (juce::Slider::TextBoxBelow,
                                     false,
                                     90,
                                     20);
    releaseSlider.setPopupDisplayEnabled (false, false, this);
    releaseSlider.setTextValueSuffix (" Seconds");
 
    // this function adds the slider to the editor
    addAndMakeVisible (&releaseSlider);
    
    // add the listener to the slider
    releaseSlider.addListener(this);
    
    // add label to slider
    addAndMakeVisible(attackLabel);
    releaseLabel.setText ("Release Time", juce::dontSendNotification);
    releaseLabel.attachToComponent (&releaseSlider, false);
    releaseLabel.setJustificationType(juce::Justification::horizontallyCentred);

}

void SynloveSynthAudioProcessorEditor::configureDelayControls ()
{
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
    
    
}



void SynloveSynthAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    
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
        // convert from percentage to decimal
        audioProcessor.drywetVal = drywetSlider.getValue() / 100.0 ;
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
    
    if (slider == &attackSlider || slider == &decaySlider || slider == &sustainSlider || slider == &releaseSlider)
    {
        audioProcessor.setEnvelope(attackSlider. getValue(),
                                   decaySlider.  getValue(),
                                   sustainSlider.getValue() / 100.0,
                                   releaseSlider.getValue());
    }
    
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
    
    configureDelayPositions   (h, w);
    configureEnvelopePositions(h, w);
    
    
}

void SynloveSynthAudioProcessorEditor::configureDelayPositions(int h, int w)
{
    int sliderDim = 100;
    
    delayTimeSlider.setBounds (static_cast<int>(1*w/5 - sliderDim/2),   // x pos
                               static_cast<int>(5*h/6 - sliderDim/2),   // y pos
                               sliderDim, sliderDim);                   // dimensions
    
    feedbackSlider.setBounds  (static_cast<int>(2*w/5 - sliderDim/2),   // x pos
                               static_cast<int>(5*h/6 - sliderDim/2),   // y pos
                               sliderDim, sliderDim);                   // dimensions
    
    drywetSlider.setBounds    (static_cast<int>(3*w/5 - sliderDim/2),   // x pos
                               static_cast<int>(5*h/6 - sliderDim/2),   // y pos
                               sliderDim, sliderDim);                   // dimensions
    
    panSlider.setBounds       (static_cast<int>(4*w/5 - sliderDim/2),   // x pos
                               static_cast<int>(5*h/6 - sliderDim/2),   // y pos
                               sliderDim, sliderDim);                   // dimensions
    
    bpmSlider.setBounds(0, 0, 200, 20);
}

void SynloveSynthAudioProcessorEditor::configureEnvelopePositions(int h, int w)
{
    int sliderDim = 100;
    
    attackSlider.setBounds    (static_cast<int>(1*w/5 - sliderDim/2),   // x pos
                               static_cast<int>(3*h/6 - sliderDim/2),   // y pos
                               sliderDim, sliderDim);                   // dimensions
    
    decaySlider.setBounds     (static_cast<int>(2*w/5 - sliderDim/2),   // x pos
                               static_cast<int>(3*h/6 - sliderDim/2),   // y pos
                               sliderDim, sliderDim);                   // dimensions
    
    sustainSlider.setBounds   (static_cast<int>(3*w/5 - sliderDim/2),   // x pos
                               static_cast<int>(3*h/6 - sliderDim/2),   // y pos
                               sliderDim, sliderDim);                   // dimensions
    
    releaseSlider.setBounds   (static_cast<int>(4*w/5 - sliderDim/2),   // x pos
                               static_cast<int>(3*h/6 - sliderDim/2),   // y pos
                               sliderDim, sliderDim);                   // dimensions
    
    bpmSlider.setBounds(0, 0, 200, 20);
}
