//#include "BandPass.h"
//
//BandPass::BandPass():BandPassFilter(juce::dsp::IIR::Coefficients<float>::makeBandPass(44100, 20000, 0.1f))
//{
//}
//
//void BandPass::Process(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
//	juce::dsp::IIR::Coefficients<float>>&
//	BandPassFilter, juce::dsp::AudioBlock<float>& block)
//{
//	BandPassFilter.process(juce::dsp::ProcessContextReplacing<float> (block));	
//}
//
//void BandPass::UpdateFilter(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
//	juce::dsp::IIR::Coefficients<float>>&
//	BandPassFilter, float freq, float res)
//{
//
//	*BandPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeBandPass(_sampleRate, freq, res);
//}
//
//void BandPass::PrepareFilter(juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
//	juce::dsp::IIR::Coefficients<float>>&
//	BandPassFilter, double sampleRate, juce::dsp::ProcessSpec& spec)
//{
//	
//	_numChannels = spec.numChannels;
//	maxBlockSize = spec.maximumBlockSize;
//	_sampleRate = sampleRate;
//	BandPassFilter.prepare(spec);
//	BandPassFilter.reset();
//}
