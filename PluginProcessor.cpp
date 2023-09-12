/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyDelayAudioProcessor::MyDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    
//    maxDelayTime = 5.0;
    
}

MyDelayAudioProcessor::~MyDelayAudioProcessor()
{
}

//==============================================================================
const juce::String MyDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MyDelayAudioProcessor::acceptsMidi() const
{
    if (JucePlugin_WantsMidiInput)
        return true;
    else
        return false;
   
}

bool MyDelayAudioProcessor::producesMidi() const
{
    if (JucePlugin_ProducesMidiOutput)
        return true;
    else
        return false;

}

bool MyDelayAudioProcessor::isMidiEffect() const
{
   if (JucePlugin_IsMidiEffect)
       return true;
   else
       return false;
   
}

double MyDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MyDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MyDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MyDelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MyDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void MyDelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MyDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // prepare circular buffer for delay
    const int numInputChannels = getTotalNumInputChannels();
    const int delayBufferSize = 5.0 * (sampleRate + samplesPerBlock); // TODO change to match max delay time
    mSampleRate = sampleRate;
    
    delayBuffer.setSize(numInputChannels, delayBufferSize);
    
}

void MyDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

//static float getMaxDelayTime() {
//    return MyDelayAudioProcessor::maxDelayTime;
//}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MyDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MyDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback     
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    
    // for ease of writing and readability
    const int audioBufferLength = buffer.getNumSamples();
    const int delayBufferLength = delayBuffer.getNumSamples();
    
   
    if (buffer.getNumSamples() != 0){
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            
            // create read pointers, points to start of array
            const float* audioBufferReadPointer = buffer.getReadPointer(channel);
            const float* delayBufferReadPointer = delayBuffer.getReadPointer(channel);
            float* audioBufferWritePointer = buffer.getWritePointer(channel);
            fillDelayBuffer(channel,
                            audioBufferLength,
                            delayBufferLength,
                            audioBufferReadPointer,
                            delayBufferReadPointer);
            
            copyFromDelayBuffer(channel,
                                buffer,
                                audioBufferLength,
                                delayBufferLength,
                                audioBufferReadPointer,
                                delayBufferReadPointer);
            
            feedbackDelay(channel,
                          audioBufferLength,
                          delayBufferLength,
                          audioBufferWritePointer);
            
        }
        
    }
    
    
    // move write position number of samples that were copied, for all channels
    delayWritePosition += audioBufferLength;
    
    // check for wraparound index
    delayWritePosition %= delayBufferLength;
    
//    std::cout << "audioBufferLength = ";
//    std::cout << audioBufferLength;
//    std::cout << "\n";
//
//    std::cout << "delayWritePosition = ";
//    std::cout << delayWritePosition;
//    std::cout << "--------------------------------\n";
    
}

void MyDelayAudioProcessor::copyFromDelayBuffer(int channel, juce::AudioBuffer<float>& audioBuffer, const int audioBufferLength, const int delayBufferLength, const float* audioBufferReadPointer, const float* delayBufferReadPointer)
{
    
    
    
    // TODO this is always the same value
    const int readPosition = (delayBufferLength + delayWritePosition - (static_cast<int>(mSampleRate * delayTimeVal))) % delayBufferLength;
    
    
    // is there enough space in the rest of the delay buffer to copy without wrapping
    if (delayBufferLength > audioBufferLength + readPosition) // yes!
    {
        audioBuffer.copyFrom(
                            channel,
                            0,
                            delayBufferReadPointer+readPosition,
                            audioBufferLength);
    }
    else // no.
    {
        int bufferRemaining = delayBufferLength - readPosition;
        int bufferExtra = audioBufferLength - bufferRemaining;
        
        // addFrom(channel, dest_start_sample, source_buffer, dest_end channel
        
        // add first part of delay from end of delay buffer to start of audio buffer
        audioBuffer.copyFrom(channel,
                            0,
                            delayBufferReadPointer+readPosition,
                            bufferRemaining);
        
        // add second part of delay from start of delay buffer to middle of audio buffer
        audioBuffer.copyFrom(channel,
                            bufferRemaining,
                            delayBufferReadPointer,
                            bufferExtra);
    }
    
    
}


void MyDelayAudioProcessor::feedbackDelay(int channel, const int audioBufferLength, const int delayBufferLength, float* audioBufferWritePointer)
{
    
    //delayFeedbackVal = 0.6;
    
    if (delayBufferLength > audioBufferLength + delayWritePosition)
    {
        delayBuffer.addFromWithRamp(channel,
                                    delayWritePosition,
                                    audioBufferWritePointer,
                                    audioBufferLength,
                                    delayFeedbackVal,
                                    delayFeedbackVal);
    }
    else
    {
        
        const int delayBufferRemaining = delayBufferLength - delayWritePosition;
       // const int delayBufferExtra =
        
        delayBuffer.addFromWithRamp(channel,
                                    delayBufferRemaining,
                                    audioBufferWritePointer,
                                    delayBufferRemaining,
                                    delayFeedbackVal,
                                    delayFeedbackVal);
        
        delayBuffer.addFromWithRamp(channel,
                                    0,
                                    audioBufferWritePointer,
                                    audioBufferLength - delayBufferRemaining,
                                    delayFeedbackVal,
                                    delayFeedbackVal);
        
    }
    
}


void MyDelayAudioProcessor::fillDelayBuffer(int channel, const int audioBufferLength, const int delayBufferLength, const float* audioBufferReadPosition, const float* delayBufferReadPosition)
{
    
    //delayFeedbackVal = 0.6;
    
    // copy number of samples from input buffer to delay buffer
    if (delayBufferLength > audioBufferLength + delayWritePosition) // no need to wraparound write
    {
        delayBuffer.copyFromWithRamp(channel,
                                     delayWritePosition,
                                     audioBufferReadPosition,
                                     audioBufferLength,
                                     delayFeedbackVal,
                                     delayFeedbackVal);
        
    }
    else // need to wraparound write
    {
        // number of samples to write first part of delay at end of audiobuffer
        const int delayBufferRemaining = delayBufferLength - delayWritePosition;
        
        // number of samples to write second part of delay at start of audiobuffer
        const int delayBufferWrap = audioBufferLength - delayBufferRemaining;
        
        
        // audioBuffer.copyFrom(channel, destStartSample, source, numSamples, startgain, endgain)
        
        // write before wraparound
        delayBuffer.copyFromWithRamp(channel,
                                     delayWritePosition,
                                     audioBufferReadPosition,
                                     delayBufferRemaining,
                                     delayFeedbackVal,
                                     delayFeedbackVal);
        
        // now write from beginning of buffer (the wraparound!)
        delayBuffer.copyFromWithRamp(channel,
                                     0,
                                     audioBufferReadPosition+delayBufferRemaining, // check!
                                     delayBufferWrap,
                                     delayFeedbackVal,
                                     delayFeedbackVal);
        
    }
}


//==============================================================================
bool MyDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MyDelayAudioProcessor::createEditor()
{
    return new MyDelayAudioProcessorEditor (*this);
}

//==============================================================================
void MyDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MyDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyDelayAudioProcessor();
}
