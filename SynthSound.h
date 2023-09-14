/*
  ==============================================================================

    SynthSound.h
    Created: 14 Sep 2023 5:43:01pm
    Author:  Derek Nadeau

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SynthSound : public juce::SynthesiserSound
{
  
public:
    bool appliesToNote(int midiNoteNumber)
    {
        return true;
    }
    
    bool appliesToChannel(int midiChannel)
    {
        return true;
    }

private:
    
};
