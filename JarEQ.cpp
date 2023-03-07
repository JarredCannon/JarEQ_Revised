#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JarEqAudioProcessor::JarEqAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
#if JucePlugin_IsMidiEffect
        .withInput("Input", juce::AudioChannelSet::mono(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::mono(), true)
#endif
#endif
    )
#endif
{
    addParameter(freqParam = new juce::AudioParameterFloat("frequency", "Frequency", 20.0f, 20000.0f, 1000.0f));
    addParameter(gainParam = new juce::AudioParameterFloat("gain", "Gain", -24.0f, 24.0f, 0.0f));
    addParameter(qParam = new juce::AudioParameterFloat("q", "Q", 0.1f, 10.0f, 1.0f));
    addParameter(filterTypeParam = new juce::AudioParameterChoice("filterType", "Filter Type", juce::StringArray("Bell", "High Pass", "Low Pass", "High Shelf", "Low Shelf"), 0));
    addParameter(globalGainParam = new juce::AudioParameterFloat("globalGain", "Global Gain", -24.0f, 24.0f, 0.0f));
    addParameter(mixParam = new juce::AudioParameterFloat("mix", "Mix", 0.0f, 1.0f, 1.0f));
}

JarEqAudioProcessor::~JarEqAudioProcessor()
{
}

//==============================================================================
const juce::String JarEqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JarEqAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool JarEqAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool JarEqAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double JarEqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JarEqAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JarEqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JarEqAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String JarEqAudioProcessor::getProgramName(int index)
{
    return {};
}

void JarEqAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void JarEqAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    dsp::ProcessSpec spec{ sampleRate, static_cast<juce::uint32>(samplesPerBlock), 1 };
    state = dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 20000.0f, 1.0f);
    processorChain.prepare(spec);
}

void JarEqAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JarEqAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
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
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;

#endif
}
#endif

void JarEqAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    if (!isBypassed())
    {
        // Get the parameters
        auto freq = *freqParam;
        auto gain = *gainParam;
        auto q = *qParam;
        auto filterType = *filterTypeParam;
        auto globalGain = *globalGainParam;
        auto mix = *mixParam;

        // Update the filter coefficients
        updateCoefficients();

        // Apply the filter
        juce::dsp::AudioBlock<float> block(buffer);
        processorChain.process(juce::dsp::ProcessContextReplacing<float>(block));

        // Apply the global gain
        buffer.applyGain(juce::Decibels::decibelsToGain(globalGain));

        // Mix dry and wet signals
        if (mix < 1.0f)
        {
            juce::dsp::AudioBlock<float> dry(block);
            juce::dsp::AudioBlock<float> wet(block);
            dry.multiplyBy(1.0f - mix);
            wet.multiplyBy(mix);
            dry.add(wet);
        }
    }

}

void JarEqAudioProcessor::updateCoefficients()
{
    auto freq = *freqParam;
    auto gain = *gainParam;
    auto q = *qParam;
    auto filterType = *filterTypeParam;

    switch (filterType)
    {
    case 0: // Bell
    {
        auto coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), freq, q, juce::Decibels::decibelsToGain(gain));
        processorChain.setBypassed<1>(false);
        processorChain.get<1>().setCoefficients(coefficients);
        break;
    }
    case 1: // High Pass
    {
        auto coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), freq, q);
        processorChain.setBypassed<1>(false);
        processorChain.get<1>().setCoefficients(coefficients);
        break;
    }
    case 2: // Low Pass
    {
        auto coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), freq, q);
        processorChain.setBypassed<1>(false);
        processorChain.get<1>().setCoefficients(coefficients);
        break;
    }
    case 3: // High Shelf
    {
        auto coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(getSampleRate(), freq, q, juce::Decibels::decibelsToGain(gain));
        processorChain.setBypassed<1>(false);
        processorChain.get<1>().setCoefficients(coefficients);
        break;
    }
    case 4: // Low Shelf
    {
        auto coefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(getSampleRate(), freq, q, juce::Decibels::decibelsToGain(gain));
        processorChain.setBypassed<1>(false);
        processorChain.get<1>().setCoefficients(coefficients);
        break;
    }
    default:
    {
        jassertfalse;
        break;
    }
    }
}

void JarEqAudioProcessor::processBlockBypassed(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
}

void JarEqAudioProcessor::reset()
{
    processorChain.reset();
    state = dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 20000.0f, 1.0f);
}

juce::MemoryBlock JarEqAudioProcessor::getStateInformation() const
{
    juce::MemoryBlock data;

    juce::MemoryOutputStream stream(data, true);
    stream << *freqParam;
    stream << *gainParam;
    stream << *qParam;
    stream << *filterTypeParam;
    stream << *globalGainParam;
    stream << *mixParam;

    return data;
}

void JarEqAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    juce::MemoryInputStream stream(data, static_cast<size_t> (sizeInBytes), false);

    float freq, gain, q, globalGain, mix;
    int filterType;

    stream >> freq;
    stream >> gain;
    stream >> q;
    stream >> filterType;
    stream >> globalGain;
    stream >> mix;

    freqParam->setValueNotifyingHost(freq);
    gainParam->setValueNotifyingHost(gain);
    qParam->setValueNotifyingHost(q);
    filterTypeParam->setValueNotifyingHost(filterType);
    globalGainParam->setValueNotifyingHost(globalGain);
    mixParam->setValueNotifyingHost(mix);
}

juce::AudioProcessorEditor* JarEqAudioProcessor::createEditor()
{
    return new JarEqAudioProcessorEditor(*this);
}