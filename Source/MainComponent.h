#pragma once

#include <JuceHeader.h>
#include "C:\Users\RAZER BLADE\Documents\JUCE_DSP\LikeToHearPrototype\LikeToHear_Prototype\Builds\VisualStudio2022\BandPass.h"


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
    void setLastSampleRate(double sampleRate);
    double getlastSampleRate();
private:
    //==============================================================================
    // Your private member variables go here...
    juce::Label freqLabel;
    juce::Label resonanceLabel;
    juce::Slider middlefrequency;
    juce::Slider resonance;
    double sampleRate{ 44100 };

    void UpdateFilter();

    std::atomic<double> value;
    std::atomic<double> resValue;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
