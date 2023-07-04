/*****************************************************************************
 * This will be updated too
 **************************************************************************/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SandAudioProcessorEditor::SandAudioProcessorEditor (SandAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
     peakFreqSliderAttachment(audioProcessor.apvts, "PKF", peakFreqSlider),
     peakGainSliderAttachment(audioProcessor.apvts, "PKG", peakGainSlider),
     peakQualitySliderAttachment(audioProcessor.apvts, "PKQ", peakQualitySlider),
     lowCutFreqSliderAttachment(audioProcessor.apvts, "LCF", lowCutFreqSlider),
     highCutFreqSliderAttachment(audioProcessor.apvts, "HCF", highCutFreqSlider),
     lowCutSlopeSliderAttachment(audioProcessor.apvts, "LCS", lowCutSlopeSlider),
     highCutSlopeSliderAttachment(audioProcessor.apvts, "HCS", highCutSlopeSlider)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    // This is where we load the sliders
    for (auto* comp:getComps())
    {
        addAndMakeVisible(comp);
    }

    setSize (600, 400);
}

SandAudioProcessorEditor::~SandAudioProcessorEditor()
{
}

//==============================================================================
void SandAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SandAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();

    // Area for the response curve
    auto responseArea = bounds.removeFromTop(bounds.getHeight() * 0.33);

    // High and Low respectively
    auto lowCutArea = bounds.removeFromLeft(bounds.getWidth() * 0.33);
    auto highCutArea = bounds.removeFromRight(bounds.getWidth() * 0.5);

    lowCutFreqSlider.setBounds(lowCutArea.removeFromTop(lowCutArea.getHeight() * 0.5));
    lowCutSlopeSlider.setBounds(lowCutArea);

    highCutFreqSlider.setBounds(highCutArea.removeFromTop(highCutArea.getHeight() * 0.5));
    highCutSlopeSlider.setBounds(highCutArea);

    peakFreqSlider.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.33));
    peakGainSlider.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.5));
    peakQualitySlider.setBounds(bounds);
}

std::vector<juce::Component*> SandAudioProcessorEditor::getComps()
{
    return
    {
        &peakFreqSlider,
        &peakGainSlider,
        &peakQualitySlider,
        &lowCutFreqSlider,
        &highCutFreqSlider,
        &lowCutSlopeSlider,
        &highCutSlopeSlider
    };
}
