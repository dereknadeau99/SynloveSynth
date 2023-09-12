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
    const int delayBufferSize = 5.0 * (sampleRate + samplesPerBlock); // 2 seconds of delay buffer, plus a little extra
    mSampleRate = sampleRate;
    
    mDelayBuffer.setSize(numInputChannels, delayBufferSize);
    
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
    const int bufferLength = buffer.getNumSamples();
    const int delayBufferLength = mDelayBuffer.getNumSamples();
    
   
//    if (buffer.getNumSamples() != 0){
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            
            // create read pointers
            const float* bufferData = buffer.getReadPointer(channel);
            const float* delayBufferData = mDelayBuffer.getReadPointer(channel);
            
            fillDelayBuffer(channel, bufferLength, delayBufferLength, bufferData, delayBufferData);
            getFromDelayBuffer(buffer, channel, bufferLength, delayBufferLength, bufferData, delayBufferData);
            
        }
//    }
    
    
    // move write position number of samples that were copied, for all channels
    mWritePosition += bufferLength;
    // check for wraparound index
    mWritePosition %= delayBufferLength;
    
}

void MyDelayAudioProcessor::getFromDelayBuffer(juce::AudioBuffer<float>& buffer, int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData)
{
    
    int delayTime = 500 ; // in ms, TODO change to variable from user
    const int readPosition = static_cast<int>(delayBufferLength + (mSampleRate * delayTime/1000) % delayBufferLength) % delayBufferLength;
    
    
    // is there enough space in the rest of the delay buffer to copy without wrapping
    if (delayBufferLength > bufferLength + readPosition) // yes!
    {
        buffer.addFrom(channel, 0, delayBufferData+readPosition, bufferLength);
    }
    else // no.
    {
        int bufferRemaining = delayBufferLength - readPosition;
        int bufferExtra = bufferLength-bufferRemaining;
        
        buffer.addFrom(channel, 0, delayBufferData+readPosition, bufferRemaining);
        buffer.addFrom(channel, bufferRemaining, delayBufferData, bufferExtra);
    }
    
    
}

void MyDelayAudioProcessor::fillDelayBuffer(int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData)
{
    
    // copy number of samples from input buffer to delay buffer
    if (delayBufferLength > bufferLength + mWritePosition) // no need to wraparound write
    {
        mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferLength, 0.8, 0.8);
    }
    else // need to wraparound write
    {
        // write before wraparound
        const int bufferRemaining = delayBufferLength - mWritePosition;
        mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferRemaining, 0.8, 0.8);
        
        // now write from beginning of buffer (the wraparound!)
        mDelayBuffer.copyFromWithRamp(channel, 0, bufferData, bufferLength-bufferRemaining, 0.8, 0.8);
        
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
