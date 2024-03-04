/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleDelayAudioProcessor::SimpleDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, juce::Identifier("PARAMETERS"),
                           { std::make_unique<juce::AudioParameterFloat>("delayLTime", "Left Delay (samples)", 0.f, 10000.f, 1000.f), std::make_unique<juce::AudioParameterFloat>("delayRTime", "Right Delay (samples)", 0.f, 10000.f, 1000.f), std::make_unique<juce::AudioParameterFloat>("dryWetValue", "Dry/Wet", 0.f, 1.00f, 0.5f),
                             std::make_unique<juce::AudioParameterFloat>("feedback", "Feedback", 0.f, 0.99f, 0.3f) })
#endif
{
    treeState.addParameterListener("delayLTime", this);
    treeState.addParameterListener("delayRTime", this);
    treeState.addParameterListener("dryWetValue", this);
treeState.addParameterListener("feedback", this);
}

SimpleDelayAudioProcessor::~SimpleDelayAudioProcessor()
{
}

//==============================================================================
const juce::String SimpleDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleDelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleDelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    juce::dsp::ProcessSpec spec;
    
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize  = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    mDelayLine.reset();
    mDelayLine.prepare(spec);
    
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SimpleDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void SimpleDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

   
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

   
    
    
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float delayTemp = mDelayLTime;
        
        if (channel == 0)
        {
            mDelayLine.setDelay(mDelayLTime);
           delayTemp = mDelayLTime;
            
        }
        
        else if (channel == 1)
        {
            mDelayLine.setDelay(mDelayRTime);
           delayTemp = mDelayRTime;
        }
        
        
        
        auto* channelData = buffer.getWritePointer (channel);
        
        
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            
            
           
            float in = channelData[i];
            float temp = mDelayLine.popSample(channel, delayTemp);
            mDelayLine.pushSample(channel, in + (temp * mFeedback));
            channelData[i] = ((in * (1- mDryWet))  + (temp * mDryWet));
            
        }

        // ..do something to the data...
    }
}

//==============================================================================
bool SimpleDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleDelayAudioProcessor::createEditor()
{
    return new SimpleDelayAudioProcessorEditor (*this, treeState);
}

//==============================================================================
void SimpleDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimpleDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleDelayAudioProcessor();
}

// Function called when parameter is changed
void SimpleDelayAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    
    if (parameterID == "delayLTime")
        
    {
        mDelayLTime = newValue;
       // mDelayLine.setDelay(newValue);
        
    }
    
    else if (parameterID == "delayRTime")
    {
        mDelayRTime = newValue;
    //    mDelayLine.setDelay(newValue);
        
    }
    
    else if (parameterID == "dryWetValue")
    {
        mDryWet = newValue;
        
    }
    
    else if (parameterID == "feedback")
        
    {
        mFeedback = newValue;
    }

}
