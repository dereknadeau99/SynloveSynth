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
    setSize (600, 600);
    
    // these define the parameters of our slider object
    delayTime.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    delayTime.setRange (0.0, 5.0, 0.05);
    delayTime.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 90, 0);
    delayTime.setPopupDisplayEnabled (true, false, this);
    delayTime.setTextValueSuffix (" Seconds Delay");
    delayTime.setValue(1.0);
 
    // this function adds the slider to the editor
    addAndMakeVisible (&delayTime);
    
    // add the listener to the slider
    delayTime.addListener (this);
}

MyDelayAudioProcessorEditor::~MyDelayAudioProcessorEditor()
{
}

void MyDelayAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    audioProcessor.delayTimeVal = delayTime.getValue();
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
    
    delayTime.setBounds ((int)w/2-100, (int)h/2-100, 200,200);
}
