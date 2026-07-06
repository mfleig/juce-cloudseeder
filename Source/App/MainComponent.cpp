#include "MainComponent.h"

//==============================================================================
// Constructor: runs when your app starts
MainComponent::MainComponent()
// deviceSelector is a JUCE UI component that lets you choose audio input/output devices
: deviceSelector (deviceManager,
0, 2,   // min/max input channels
0, 2,   // min/max output channels
false,   // show MIDI input options
false,   // show MIDI output options
true,   // show audio channels
false)  // hide advanced settings
{
// Set window size
setSize (600, 400);

effectChain.addEffect(
    std::make_unique<DelayEffect>()
);

// Initialize audio system:
// (inputs, outputs)
// This tells JUCE: "I want 2 inputs and 2 outputs"
setAudioChannels (2, 2);

// Add UI components to the window so they are visible
addAndMakeVisible (deviceSelector);
addAndMakeVisible (toggleButton);

// Define what happens when button is clicked
toggleButton.onClick = [this]()
{
    auto* effect = effectChain.getEffect(0);

    if (effect == nullptr)
    {
        std::cout << "Effect not found!" << std::endl;
        return;
    }

    bool enabled = !effect->isActive();
    effect->setEnabled(enabled);

    toggleButton.setButtonText(enabled ? "Delay ON" : "Delay OFF");
};
}

// Destructor: runs when app closes
MainComponent::~MainComponent()
{
// Properly shut down audio device
shutdownAudio();
}

//==============================================================================
// 🎧 AUDIO CALLBACKS (core of DSP flow)

// Called BEFORE audio starts
// This is where you initialize buffers, sample rate dependent stuff, etc.
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
// Forward preparation to your Effect Chain
// sampleRate = how many samples per second (e.g. 44100)
// samplesPerBlockExpected = chunk size JUCE processes each callback
effectChain.prepare(sampleRate, samplesPerBlockExpected);
}

// Called when audio stops
void MainComponent::releaseResources()
{
// Not used now, but good place to free memory if needed
}

// 🔥 REAL-TIME AUDIO PROCESSING
// This runs MANY times per second (audio thread)
// DO NOT block or do heavy work here
void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
// Get pointer to audio buffer (this contains incoming guitar signal)
auto* buffer = bufferToFill.buffer;

// Loop through each audio channel (usually 2 = stereo)
for (int channel = 0; channel < buffer->getNumChannels(); ++channel)
{
    // Get pointer to raw float samples for this channel
    // startSample ensures we process correct region of buffer
    auto* channelData = buffer->getWritePointer(channel, bufferToFill.startSample);

    // Send audio data to your Effect chain
    // This modifies the signal IN PLACE
    effectChain.process(channelData, bufferToFill.numSamples, channel);
}

}

//==============================================================================
// 🎨 GUI DRAWING

void MainComponent::paint (juce::Graphics& g)
{
// Fill background with default UI color
g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

// Set font size
g.setFont (juce::FontOptions (16.0f));

// Set text color
g.setColour (juce::Colours::white);

// Draw centered text
g.drawText ("Martin's Cloud Seeder", getLocalBounds(), juce::Justification::centred, true);

}

// Called whenever window is resized
void MainComponent::resized()
{
// Make device selector fill whole window
deviceSelector.setBounds (getLocalBounds());

// Position toggle button at bottom-left
toggleButton.setBounds(10, getHeight() - 40, 120, 30);


}
