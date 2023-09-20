
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthVoice.h"

//==============================================================================
SynloveSynthAudioProcessor::SynloveSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
#endif
{
    mySynth.clearVoices();

    int maxVoices = 1;
    for (int i = 0; i < maxVoices; i++)
    {
        mySynth.addVoice(new SynthVoice());
    }

    mySynth.clearSounds();

    mySynth.addSound(new SynthSound());
    
    


}

SynloveSynthAudioProcessor::~SynloveSynthAudioProcessor()
{
}

//==============================================================================
const juce::String SynloveSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynloveSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynloveSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynloveSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynloveSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynloveSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynloveSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynloveSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynloveSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynloveSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynloveSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    const int numInputChannels = getTotalNumInputChannels();

    //==================SYNTH=======================================

    juce::ignoreUnused(samplesPerBlock);
    lastSampleRate = sampleRate;
    mySynth.setCurrentPlaybackSampleRate(lastSampleRate);


    //==================DELAY=======================================
    // TODO change delaybuffersize to match max delay time dynamically
    const int delayBufferSize = 5.0 * (sampleRate + samplesPerBlock);
    mSampleRate = sampleRate;

    delayBuffer.setSize(numInputChannels, delayBufferSize);
    dryAudioBufferCopy.setSize(numInputChannels, samplesPerBlock);


}

void SynloveSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynloveSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SynloveSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // for ease of writing and readability
    const int audioBufferLength = buffer.getNumSamples();
    const int delayBufferLength = delayBuffer.getNumSamples();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i){
        buffer.clear (i, 0, audioBufferLength);
    }

    mySynth.renderNextBlock(buffer, midiMessages, 0, audioBufferLength);



    //==============PRE  PROCESSING====================================================




    
    
    if (audioBufferLength != 0){
        
        
        for (int channel = 0; channel < totalNumOutputChannels; ++channel)
        {

            //==========AUDIO PROCESSING===========================================

            // create read pointers, points to start of array
            const float* audioBufferReadPointer  = buffer.getReadPointer (channel);
            const float* audioBufferWritePointer = buffer.getWritePointer(channel);
            const float* delayBufferReadPointer  = delayBuffer.getReadPointer(channel);

            //==============SYNTH==================================================


            




            //=============EFFECTS=================================================
            effectDelayProcessing(channel,
                                  buffer,
                                  audioBufferWritePointer,
                                  audioBufferReadPointer, delayBufferReadPointer,
                                  audioBufferLength,      delayBufferLength);
            

        }

    }


    //==============POST PROCESSING====================================================
    effectDelayProcessingPost(audioBufferLength, delayBufferLength);


}

void SynloveSynthAudioProcessor::effectDelayProcessing(const int channel, juce::AudioBuffer<float>& audioBuffer, const float* audioBufferWritePointer, const float* audioBufferReadPointer, const float* delayBufferReadPointer, const int audioBufferLength, const int delayBufferLength)
{
    // set up dry copy for later mixing
    dryAudioBufferCopy.copyFromWithRamp(channel,
                                        0,
                                        audioBufferReadPointer,
                                        audioBufferLength,
                                        1-drywetVal, 1-drywetVal);

    // copy dry audio to delay buffer in appropriate time
    fillDelayBuffer(channel,
                    audioBufferLength,
                    delayBufferLength,
                    audioBufferReadPointer,
                    delayBufferReadPointer);

    // copy from delay buffer to output buffer from appropriate time in past
    copyFromDelayBuffer(channel,
                        audioBuffer,
                        audioBufferLength,
                        delayBufferLength,
                        audioBufferReadPointer,
                        delayBufferReadPointer);

    //
    feedbackDelay(channel,
                  audioBufferLength,
                  delayBufferLength,
                  audioBufferWritePointer);


    // apply proper gain to wet version
    audioBuffer.applyGain(channel,
                          0,
                          audioBufferLength,
                          drywetVal);

    // sum dry and wet
    audioBuffer.addFrom(channel,
                        0,
                        dryAudioBufferCopy.getReadPointer(channel),
                        audioBufferLength);
    
    

}

void SynloveSynthAudioProcessor::effectDelayProcessingPost(const int audioBufferLength, const int delayBufferLength)
{
    // move write position number of samples that were copied, for all channels
    delayWritePosition += audioBufferLength;

    // check for wraparound index
    delayWritePosition %= delayBufferLength;
}

void SynloveSynthAudioProcessor::copyFromDelayBuffer(int channel, juce::AudioBuffer<float>& audioBuffer, const int audioBufferLength, const int delayBufferLength, const float* audioBufferReadPointer, const float* delayBufferReadPointer)
{


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


void SynloveSynthAudioProcessor::feedbackDelay(int channel, const int audioBufferLength, const int delayBufferLength, const float* audioBufferWritePointer)
{

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
        const int delayBufferExtra     = audioBufferLength - delayBufferRemaining;

        delayBuffer.addFromWithRamp(channel,
                                    delayBufferRemaining,
                                    audioBufferWritePointer,
                                    delayBufferRemaining,
                                    delayFeedbackVal,
                                    delayFeedbackVal);

        delayBuffer.addFromWithRamp(channel,
                                    0,
                                    audioBufferWritePointer,
                                    delayBufferExtra,
                                    delayFeedbackVal,
                                    delayFeedbackVal);

    }

}


void SynloveSynthAudioProcessor::fillDelayBuffer(int channel, const int audioBufferLength, const int delayBufferLength, const float* audioBufferReadPosition, const float* delayBufferReadPosition)
{

    float channelPanValue = 1.0;
    if (channel == 0) { channelPanValue = 1 - panVal; } // left channel gain
    else              { channelPanValue = panVal; }     // right channel gain

    const float gain = channelPanValue * delayFeedbackVal;

    // copy number of samples from input buffer to delay buffer
    if (delayBufferLength > audioBufferLength + delayWritePosition) // no need to wraparound write
    {
        delayBuffer.copyFromWithRamp(channel,
                                     delayWritePosition,
                                     audioBufferReadPosition,
                                     audioBufferLength,
                                     gain,
                                     gain);

    }
    else // need to wraparound write
    {
        // number of samples to write first part of delay at end of audiobuffer
        const int delayBufferRemaining = delayBufferLength - delayWritePosition;

        // number of samples to write second part of delay at start of audiobuffer
        const int delayBufferWrap = audioBufferLength - delayBufferRemaining;


        // audioBuffer.copyFrom(channel, destStartSample, source, numSamples, startgain, endgain) // TODO what

        // write before wraparound
        delayBuffer.copyFromWithRamp(channel,
                                     delayWritePosition,
                                     audioBufferReadPosition,
                                     delayBufferRemaining,
                                     gain,
                                     gain);

        // now write from beginning of buffer (the wraparound!)
        delayBuffer.copyFromWithRamp(channel,
                                     0,
                                     audioBufferReadPosition+delayBufferRemaining, // check!
                                     delayBufferWrap,
                                     gain,
                                     gain);

    }
}


//==============================================================================
bool SynloveSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynloveSynthAudioProcessor::createEditor()
{
    return new SynloveSynthAudioProcessorEditor (*this);
}

//==============================================================================
void SynloveSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SynloveSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynloveSynthAudioProcessor();
}
