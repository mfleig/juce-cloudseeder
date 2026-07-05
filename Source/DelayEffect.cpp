#include "DelayEffect.h"

//==============================================================================
// Called once before audio processing starts
// Used to initialize buffers and state
void DelayEffect::prepare(double sampleRate, int samplesPerBlock)
{
// Calculate buffer size:
// sampleRate = samples per second (e.g. 44100)
// * 2 = we want max 2 seconds of delay memory
int delayBufferSize = (int) sampleRate * 2;

// Allocate delay buffer:
// 2 channels (stereo), delayBufferSize samples each
delayBuffer.setSize(2, delayBufferSize);

// Clear buffer (VERY important)
// Otherwise you'll get random noise from uninitialized memory
delayBuffer.clear();

// Initialize write positions for each channel
// Example:
// writePositions[0] = left channel
// writePositions[1] = right channel
writePositions.clear();
writePositions.resize(2, 0);

}

//==============================================================================
// Real-time audio processing function
// This runs continuously on the audio thread
void DelayEffect::process(float* channelData, int numSamples, int channel)
{
// Get pointer to delay buffer for this channel
// This gives access to stored past samples
auto* delayData = delayBuffer.getWritePointer(channel);


// Total size of delay buffer
int delayBufferSize = delayBuffer.getNumSamples();

// Reference to write position for this channel
// Using reference (&) so we modify the actual stored value
int& writePos = writePositions[channel];

// Process each sample in this audio block
for (int i = 0; i < numSamples; ++i)
{
    // Current input sample (guitar signal)
    float input = channelData[i];

    // Calculate read position (where delayed signal comes from)
    // This looks BACK in time by delaySamples
    int readPos = writePos - delaySamples;

    // Wrap around buffer if negative (circular buffer logic)
    if (readPos < 0)
        readPos += delayBufferSize;

    // Read delayed sample from buffer
    float delayed = delayData[readPos];

    // Default output = clean signal
    float output = input;

    // If effect is enabled, mix delayed signal
    if (isEnabled)
        output = input + delayed * 0.4f;
        // delayed * 0.4f = wet signal (echo volume)

    // Write output back to audio buffer (what you hear)
    channelData[i] = output;

    // Store current input into delay buffer
    // IMPORTANT:
    // We store input, NOT output
    // Otherwise we'd create feedback loop
    delayData[writePos] = input;

    // Move write position forward (like a tape head moving)
    writePos++;

    // Wrap around if we reach end of buffer
    if (writePos >= delayBufferSize)
        writePos = 0;
}

}
