/*****************************************************************************
 * You already know
 **************************************************************************/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


struct CustomRotarySlider:juce::Slider
{
    CustomRotarySlider():juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                        juce::Slider::TextEntryBoxPosition::NoTextBox)
    {

    }
};

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

    CustomRotarySlider peakFreqSlider,
    peakGainSlider,
    peakQualitySlider,
    lowCutFreqSlider,
    highCutFreqSlider,
    lowCutSlopeSlider,
    highCutSlopeSlider;

    using APVTS = juce::AudioProcessorValueTreeState;

    using Attachment = APVTS::SliderAttachment;

    Attachment peakFreqSliderAttachment,
               peakGainSliderAttachment,
               peakQualitySliderAttachment,
               lowCutFreqSliderAttachment,
               highCutFreqSliderAttachment,
               lowCutSlopeSliderAttachment,
               highCutSlopeSliderAttachment;

    std::vector<juce::Component*> getComps();


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SandAudioProcessorEditor)
};
