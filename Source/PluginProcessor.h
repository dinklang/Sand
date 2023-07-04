/*****************************************************************************
 * The beggining of Sand!
 **************************************************************************/

#pragma once

#include <JuceHeader.h>


enum Slope
{
    Slope_12,
    Slope_24,
    Slope_36,
    Slope_48
};

// Data Struct to keep settings organized
struct ChainSettings
{
    float peakFreq { 0 }, peakGainInDecibels{ 0 }, peakQuality { 1.f };
    float lowCutFreq { 0 }, highCutFreq { 0 };
    Slope lowCutSlope { Slope::Slope_12 }, highCutSlope { Slope::Slope_12 };
};

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts);

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

    // Aliases
    using Filter = juce::dsp::IIR::Filter<float>;
    
    // Normally 12 dB per octave response
    using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>;

    using MonoChain = juce::dsp::ProcessorChain<CutFilter, Filter, CutFilter>;

    MonoChain leftChain, rightChain;

    enum ChainPositions
    {
        LowCut,
        Peak,
        HighCut
    };

    void updatePeakFilter(const ChainSettings& chainSettings);
    using Coefficients = Filter::CoefficientsPtr;

    // Will have to move this function
    static void updateCoefficients(Coefficients& old, const Coefficients& replacements);

    template<int Index, typename ChainType, typename CoefficientType>
    void update(ChainType& chain, const CoefficientType& coefficients)
    {
        updateCoefficients(chain.template get<Index>().coefficients, coefficients[Index]);
        chain.template setBypassed<Index>(false);
    }

    // Refactoring
    template<typename ChainType, typename CoefficientType>
    void updateCutFilter(ChainType& leftLowCut,
                        const CoefficientType& cutCoefficients,
                        const Slope& lowCutSlope)
    {

        // ByPass States, optional requirements
        leftLowCut.template setBypassed<0>(true);
        leftLowCut.template setBypassed<1>(true);
        leftLowCut.template setBypassed<2>(true);
        leftLowCut.template setBypassed<3>(true);

        switch (lowCutSlope)
        {
            case Slope_48:
                update<3>(leftLowCut, cutCoefficients);
            case Slope_36:
                update<2>(leftLowCut, cutCoefficients);
            case Slope_24:
                update<1>(leftLowCut, cutCoefficients);
            case Slope_12:
                update<0>(leftLowCut, cutCoefficients);
        }
    }

    void updateLowCutFilters(const ChainSettings& ChainSettings);
    void updateHighCutFilters(const ChainSettings& ChainSettings);
    void updateFilters();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SandAudioProcessor)
};
