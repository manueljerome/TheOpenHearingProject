#include "MainComponent.h"
#include <JuceHeader.h>


//==============================================================================
MainComponent::MainComponent():BandPassFilter(juce::dsp::IIR::Coefficients<float>::makeBandPass(44100, 20000, 0.1f))
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (400, 400);
    middlefrequency.setRange(1.0, 100.0);
    volume.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    volume.setRange(6.0, 12.0, 3.0);
    volume.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    volumeLabel.setText("Volume", juce::NotificationType::dontSendNotification);
    middlefrequency.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    freqLabel.setText("Frequency", juce::NotificationType::dontSendNotification);
    resonance.setRange(1.0, 5);
    resonance.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    resonanceLabel.setText("Resonance", juce::NotificationType::dontSendNotification);

    addAndMakeVisible(middlefrequency);
    addAndMakeVisible(freqLabel);
    addAndMakeVisible(resonance);
    addAndMakeVisible(resonanceLabel);
    addAndMakeVisible(volume);
    addAndMakeVisible(volumeLabel);

    

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.sampleRate = sampleRate;
    spec.numChannels = 2;
    setLastSampleRate(sampleRate);
    BandPassFilter.prepare(spec);
    Gain.prepare(spec);
    BandPassFilter.reset();
    Gain.reset();
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!
    
    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)

    auto* device = deviceManager.getCurrentAudioDevice();
    auto activeInputChannels = device->getActiveInputChannels();
    auto activeOutputChannels = device->getActiveOutputChannels();
    auto maxInputChannels = activeInputChannels.getHighestBit() + 1;
    auto maxOutputChannels = activeOutputChannels.getHighestBit() + 1;

    juce::dsp::AudioBlock<float> block(bufferToFill.buffer->getArrayOfWritePointers(),
        bufferToFill.buffer->getNumChannels(),
        bufferToFill.startSample,
        bufferToFill.numSamples);

    auto MidFreq = (float)middlefrequency.getValue();
    auto res = (float)resonance.getValue();
    Gain.setGainDecibels((float)volume.getValue());

    UpdateFilter();
    BandPassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
    Gain.process(juce::dsp::ProcessContextReplacing<float>(block));
    middlefrequency.onValueChange = [this]() {value.store(middlefrequency.getValue()); };
    resonance.onValueChange = [this]() {resValue.store(resonance.getValue()); };

    for (auto channel = 0; channel < maxOutputChannels; ++channel)
    {
        if ((!activeOutputChannels[channel]) || maxInputChannels == 0)
        {
            bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
        }
        else
        {
            auto actualInputChannel = channel % maxInputChannels; // [1]

            if (!activeInputChannels[channel]) // [2]
            {
                bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
            }
            else // [3]
            {
                auto* inBuffer = bufferToFill.buffer->getReadPointer(actualInputChannel,
                    bufferToFill.startSample);
                
                auto* outBuffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
                
                
                for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
                    outBuffer[sample] = inBuffer[sample];
            }
        }
    }

}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colour(0));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    freqLabel.setBounds(10, 10, 90, 20);
    middlefrequency.setBounds(100, 10, getWidth() - 110, 20);
    resonanceLabel.setBounds(10, 40, 90, 20);
    resonance.setBounds(100, 40, getWidth() - 110, 20);
    volumeLabel.setBounds(20, 100, 90, 70 );
    volume.setBounds(100, 100, getWidth() - 110, 100);
    //commit
}

void MainComponent::setLastSampleRate(double sampleRate)
{
    this->sampleRate = sampleRate;
}

double MainComponent::getlastSampleRate()
{
    return sampleRate;
}

void MainComponent::UpdateFilter()
{
    auto MidFreq = (float)middlefrequency.getValue();
    auto res = (float)resonance.getValue();
    
    if(MidFreq > 0 && res > 0)
        *BandPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeBandPass(getlastSampleRate(), MidFreq, res);
   
}


