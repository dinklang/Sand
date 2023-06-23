/*****************************************************************************
 * Implementing sand
 **************************************************************************/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SandAudioProcessor::SandAudioProcessor()
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
}

SandAudioProcessor::~SandAudioProcessor()
{
}

//==============================================================================
const juce::String SandAudioProcessor::getName() const
{
    // return JucePlugin_Name;
    return "Sand";
}

bool SandAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SandAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SandAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SandAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SandAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SandAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SandAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SandAudioProcessor::getProgramName (int index)
{
    return {};
}

void SandAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SandAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    juce::dsp::ProcessSpec spec;

    spec.maximumBlockSize = samplesPerBlock;

    spec.numChannels = 1;

    spec.sampleRate = sampleRate;

    leftChain.prepare(spec);
    rightChain.prepare(spec);

    auto chainSettings = getChainSettings(apvts);

    auto peakCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 
                                                                                chainSettings.peakFreq, 
                                                                                chainSettings.peakQuality, 
                                                                                juce::Decibels::decibelsToGain(chainSettings.peakGainInDecibels));

    *leftChain.get<ChainPositions::Peak>().coefficients = *peakCoefficients;
    *rightChain.get<ChainPositions::Peak>().coefficients = *peakCoefficients;

}

void SandAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SandAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SandAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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

    auto chainSettings = getChainSettings(apvts);

    auto peakCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 
                                                                                chainSettings.peakFreq, 
                                                                                chainSettings.peakQuality, 
                                                                                juce::Decibels::decibelsToGain(chainSettings.peakGainInDecibels));

    *leftChain.get<ChainPositions::Peak>().coefficients = *peakCoefficients;
    *rightChain.get<ChainPositions::Peak>().coefficients = *peakCoefficients;


    juce::dsp::AudioBlock<float> block(buffer);

    // Each audio block represents each individual channel, it is retrivied from the buffer
    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);

    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    leftChain.process(leftContext);
    rightChain.process(rightContext);
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    // for (int channel = 0; channel < totalNumInputChannels; ++channel)
    // {
    //     auto* channelData = buffer.getWritePointer (channel);

    //     // ..do something to the data...
    // }
}

//==============================================================================
bool SandAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SandAudioProcessor::createEditor()
{
    // return new SandAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void SandAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SandAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


ChainSettings getChainSettings(juce::AudioProcessorValueTreeState &apvts)
{
    ChainSettings settings;


    settings.lowCutFreq = apvts.getRawParameterValue("LCF")->load();
    settings.highCutFreq = apvts.getRawParameterValue("HCF")->load();
    settings.peakFreq = apvts.getRawParameterValue("PKF")->load();
    settings.peakGainInDecibels = apvts.getRawParameterValue("PKG")->load();
    settings.peakQuality = apvts.getRawParameterValue("PKQ")->load();
    settings.lowCutSlope = apvts.getRawParameterValue("LCS")->load();
    settings.highCutSlope = apvts.getRawParameterValue("HCS")->load();

    return settings;
}

/******************************************************************************
 * Things to know:
 * * ParameterLayout contains AudioParameters to be passed to an AudioProcessor
 *****************************************************************************/
juce::AudioProcessorValueTreeState::ParameterLayout SandAudioProcessor::createParameterLayout()
{
    // mt layout
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // added bunch of sliding bars
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("LCF", 1),
        "LowCut Freq",
        juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f),
        20.f
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("HCF"),
        "HighCut Freq",
        juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f),
        20000.f
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("PKF"),
        "Peak Freq",
        juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f),
        750.f
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("PKG"),
        "Peak Gain",
        juce::NormalisableRange<float>(-24.f, 24.f, 0.5f, 1.f),
        0.0f // you do not want to destroy someone's ears
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("PKQ", 1),
        "Peak Quality",
        juce::NormalisableRange<float>(0.1f, 10.f, 0.05f, 1.f),
        1.f
    ));

    juce::StringArray stringArray;

    // Add in increments of 12 up 48 db/Oct
    for (int i = 0; i < 4; i++)
    {
        juce::String str;
        str << (12 + (i * 12));
        str << " db/Oct";
        stringArray.add(str);
    }

    // Different type: this one is a dropdown
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("LCS", 1),
        "LowCut Slope",
        stringArray,
        0
    ));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("HCS", 1),
        "HighCut Slope",
        stringArray,
        0
    ));

    return layout;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SandAudioProcessor();
}
