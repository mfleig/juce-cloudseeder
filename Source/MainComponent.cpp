#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : deviceSelector (deviceManager,
                      0, 2,   // min/max inputs
                      0, 2,   // min/max outputs
                      true,   // show MIDI input
                      true,   // show MIDI output
                      true,   // show channels
                      false)  // hide advanced
{
    setSize (600, 400);

    setAudioChannels (2, 2);

    addAndMakeVisible (deviceSelector);
    addAndMakeVisible(toggleButton);

    toggleButton.onClick = [this]()
    {
        delayEnabled = !delayEnabled;

        toggleButton.setButtonText(delayEnabled ? "Delay ON" : "Delay OFF");
    };
}

MainComponent::~MainComponent()
{
    shutdownAudio(); // 🧹 clean up audio
}

//==============================================================================
// 🎧 AUDIO CALLBACKS

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    int delayBufferSize = (int) sampleRate * 2; // 2 seconds max delay

    delayBuffer.setSize (2, delayBufferSize);
    delayBuffer.clear();

    delayWritePosition = 0;
}

void MainComponent::releaseResources()
{
    // Called when audio stops
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* buffer = bufferToFill.buffer;

    int delayBufferSize = delayBuffer.getNumSamples();

    int delaySamples = 22050; // ~0.5 sec delay (assuming 44.1kHz)

    for (int channel = 0; channel < buffer->getNumChannels(); ++channel)
    {
        auto* channelData = buffer->getWritePointer(channel, bufferToFill.startSample);
        auto* delayData = delayBuffer.getWritePointer(channel);

        int writePosition = delayWritePosition;

        for (int i = 0; i < bufferToFill.numSamples; ++i)
        {
            int readPosition = (writePosition + delayBufferSize - delaySamples) % delayBufferSize;

            float delayedSample = delayData[readPosition];

            float inputSample = channelData[i];

            // Mix dry + wet
            if (delayEnabled)
                channelData[i] = inputSample + delayedSample * 0.5f;
            else
                channelData[i] = inputSample;

            // Write into delay buffer
            delayData[writePosition] = inputSample;

            writePosition++;
            if (writePosition >= delayBufferSize)
                writePosition = 0;
        }
    }

    delayWritePosition += bufferToFill.numSamples;
    delayWritePosition %= delayBufferSize;
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::FontOptions (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    deviceSelector.setBounds (getLocalBounds());
    toggleButton.setBounds(10, getHeight() - 40, 120, 30);
}
