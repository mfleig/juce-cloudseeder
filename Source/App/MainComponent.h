#pragma once
// Prevents this header from being included multiple times (avoids duplicate definitions)

#include <JuceHeader.h>
// Includes all JUCE core modules (audio, GUI, utilities, etc.)

#include "../Audio/EffectChain.h"
#include "../Effects/Delay/DelayEffect.h"
// Include your custom DSP effect (your delay implementation)

//==============================================================================
// MainComponent is your main UI + audio processing class
// It inherits from AudioAppComponent, which:
// - Handles audio device setup
// - Provides audio callbacks (prepareToPlay, getNextAudioBlock)
class MainComponent  : public juce::AudioAppComponent
{
public:
// Constructor (runs when app starts)
MainComponent();

// Destructor (runs when app closes)
~MainComponent() override;

//==============================================================================

// Called before audio starts
// Used to initialize buffers, sample rate-dependent variables, etc.
void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;

// Called repeatedly in real-time (audio thread)
// This is where audio processing happens
void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;

// Called when audio stops
// Used to release resources (not needed yet in your case)
void releaseResources() override;

//==============================================================================

// GUI drawing function
void paint (juce::Graphics&) override;

// Called when window is resized
void resized() override;

private:
//==============================================================================

// JUCE macro:
// - Prevents copying of this class (important for audio safety)
// - Adds leak detection (helps debug memory issues)
JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)

//===========================
// 🎛️ UI COMPONENTS
//===========================

// Audio device selector UI
// Lets user choose:
// - input device (your Zoom)
// - output device (speakers)
// - buffer size, sample rate, etc.
juce::AudioDeviceSelectorComponent deviceSelector;

// Button to toggle delay ON/OFF
juce::TextButton toggleButton { "Delay ON" };

//===========================
// 🎸 AUDIO EFFECTS
//===========================

// Your Effects Chain DSP object
// This is where audio processing logic lives
// (separate from UI — very important design decision)
EffectChain effectChain;
};
