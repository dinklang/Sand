/*****************************************************************************
 * You already know
 **************************************************************************/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SandAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SandAudioProcessorEditor (SandAudioProcessor&);
    ~SandAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SandAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SandAudioProcessorEditor)
};