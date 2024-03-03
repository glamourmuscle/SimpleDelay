/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleDelayAudioProcessorEditor::SimpleDelayAudioProcessorEditor (SimpleDelayAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), treeState(vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400,    500);

    // Delay time
    delayTimeLValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, "delayLTime", delayTimeLSlider);
    delayTimeLSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    delayTimeLSlider.setRange(0.0f, 5000.0f, 1.0f);
    delayTimeLSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 75, 25);
    addAndMakeVisible(&delayTimeLSlider);

    delayTimeRValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, "delayRTime", delayTimeRSlider);
    delayTimeRSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    delayTimeRSlider.setRange(0.0f, 5000.0f, 1.0f);
    delayTimeRSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 75, 25);
    addAndMakeVisible(&delayTimeRSlider);
    
    addAndMakeVisible(delayTimeLLabel);
    delayTimeLLabel.setText("Left Delay time (samples)", juce::dontSendNotification);
    delayTimeLLabel.attachToComponent(&delayTimeLSlider, false);
    
    addAndMakeVisible(delayTimeRLabel);
    delayTimeRLabel.setText("Right Delay time (samples)", juce::dontSendNotification);
    delayTimeRLabel.attachToComponent(&delayTimeRSlider, false);
    
    dryWetValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, "dryWetValue", dryWetSlider);
    dryWetSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    dryWetSlider.setRange(0.0f, 1.0f, 0.01f);
    dryWetSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 75, 25);
    addAndMakeVisible(&dryWetSlider);
    addAndMakeVisible(dryWetLabel);
    dryWetLabel.setText("Dry/Wet Control", juce::dontSendNotification);
    dryWetLabel.attachToComponent(&dryWetSlider, false);
    

    // Feedback
    feedbackValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, "feedback", feedbackSlider);
    feedbackSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    feedbackSlider.setRange(0.0f, 0.99f, 0.01f);
    feedbackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 75, 25);
    addAndMakeVisible(&feedbackSlider);

    addAndMakeVisible(feedbackLabel);
    feedbackLabel.setText("Feedback", juce::dontSendNotification);
    feedbackLabel.attachToComponent(&feedbackSlider, false);
}

SimpleDelayAudioProcessorEditor::~SimpleDelayAudioProcessorEditor()
{
}

//==============================================================================
void SimpleDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::grey);
    g.setColour(juce::Colours::white);
    g.setFont(20);
    g.drawFittedText("Simple Delay", 150, 20, 130, 30, juce::Justification::centred, 1, 0.0f);
}

void SimpleDelayAudioProcessorEditor::resized()
{
    delayTimeLSlider.setBounds(50, 110, 320, 50);
    delayTimeRSlider.setBounds(50, 180, 320, 50);
    dryWetSlider.setBounds(50, 270, 320, 50);
    feedbackSlider.setBounds(50, 320, 320, 50);
}
