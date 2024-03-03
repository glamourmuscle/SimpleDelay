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
class SimpleDelayAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SimpleDelayAudioProcessorEditor (SimpleDelayAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~SimpleDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleDelayAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;

    juce::Slider delayTimeLSlider;
    juce::Slider delayTimeRSlider;
    juce::Slider feedbackSlider;
    juce::Slider dryWetSlider;
    

    juce::Label delayTimeLLabel;
    juce::Label delayTimeRLabel;
    juce::Label feedbackLabel;
    juce::Label dryWetLabel;

    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeLValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeRValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> dryWetValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> feedbackValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleDelayAudioProcessorEditor)
};
