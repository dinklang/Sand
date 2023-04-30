/*****************************************************************************
 * The beggining of Sand!
 **************************************************************************/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class SandAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    SandAudioProcessor();
    ~SandAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    /**************************************************************************
     * "who gave Sandro a car?"
     *                 -Yeah
    **************************************************************************/

    /*******************************************************************
    * createParamterLayout():
    * Function describes itself.
    ********************************************************************/
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    /*******************************************************************
    * AudioProcessorValueTreeState(apvts):
    * This tracks the state of the parameters to sync the GUI and DSP
    ********************************************************************/
    juce::AudioProcessorValueTreeState apvts 
    {*this, nullptr, "Parameters", createParameterLayout()};

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SandAudioProcessor)
};
