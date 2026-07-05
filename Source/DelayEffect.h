#pragma once
// Prevent multiple inclusions of this header

#include <JuceHeader.h>
// Gives access to JUCE audio classes (AudioBuffer, etc.)

#include "Effect.h"
// Import base class (our DSP interface)

//==============================================================================
// DelayEffect = concrete implementation of an Effect
// Inherits from Effect, so it MUST implement:
// - prepare()
// - process()
class DelayEffect : public Effect
{
public:
// Called before audio processing starts
// Used to initialize buffers and state
void prepare(double sampleRate, int samplesPerBlock) override;

// Real-time DSP function
// Processes audio samples in-place
void process(float* channelData, int numSamples, int channel) override;


private:
//==============================================================================

// Circular buffer that stores past audio samples
// This is what makes delay possible:
// we "look back in time" by reading from this buffer
juce::AudioBuffer<float> delayBuffer;

// One write position per channel (VERY important)
//
// Example:
// writePositions[0] → left channel
// writePositions[1] → right channel
//
// Each channel must have its own pointer to avoid glitches
std::vector<int> writePositions;

// Number of samples to delay (distance between write and read)
//
// Example:
// 22050 samples ≈ 0.5 seconds at 44.1kHz
//
// This controls the delay time
int delaySamples = 22050;


};
