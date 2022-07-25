#pragma once

#include <JuceHeader.h>



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Coefficients<float>>
        BandPassFilter;
    juce::dsp::Gain<float> Gain;
    void setLastSampleRate(double sampleRate);
    double getlastSampleRate();
private:
    //==============================================================================
    // Your private member variables go here...
    juce::Label freqLabel;
    juce::Label resonanceLabel;
    juce::Slider middlefrequency;
    juce::Slider resonance;
    juce::Slider volume;
    juce::Label volumeLabel;
    double sampleRate{ 44100 };

    void UpdateFilter();

    std::atomic<double> value;
    std::atomic<double> resValue;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
