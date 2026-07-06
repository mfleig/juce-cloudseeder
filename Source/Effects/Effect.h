#pragma once
// Prevents multiple inclusions of this header file

//==============================================================================
// Base class for ALL audio effects (Delay, Reverb, Chorus, etc.)
// This defines a COMMON INTERFACE that every effect must implement
class Effect
{
public:
// Virtual destructor:
// Ensures proper cleanup when deleting derived classes via base pointer
virtual ~Effect() = default;

//==============================================================================

// Called BEFORE audio processing starts
// Each effect uses this to:
// - allocate buffers
// - store sample rate
// - initialize state
virtual void prepare(double sampleRate, int samplesPerBlock) = 0;

// Core DSP function (REAL-TIME)
// This processes audio samples IN PLACE
//
// channelData → pointer to raw float samples (audio signal)
// numSamples  → number of samples to process
// channel     → which channel (0 = left, 1 = right)
//
// "= 0" means this is a PURE VIRTUAL function:
// → every child class MUST implement it
virtual void process(float* channelData, int numSamples, int channel) = 0;

//==============================================================================

// Enable / disable effect (bypass control)
// Used by UI (e.g., toggle button)
virtual void setEnabled(bool enabled) { isEnabled = enabled; }

// Returns current state of effect (ON or OFF)
bool isActive() const { return isEnabled; }

protected:
// Shared state for all effects:
// Determines if effect should process audio or act as bypass
bool isEnabled = true;
};
