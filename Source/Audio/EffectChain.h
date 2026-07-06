#pragma once
#include <vector>
#include <memory>
#include "../Effects/Effect.h"

//==============================================================================
// EffectChain = holds multiple effects and processes them in order
class EffectChain
{
public:
    // Add an effect to the chain
    void addEffect(std::unique_ptr<Effect> effect)
    {
        effects.push_back(std::move(effect));
    }

    // Prepare all effects (called before audio starts)
    void prepare(double sampleRate, int samplesPerBlock)
    {
        for (auto& effect : effects)
            effect->prepare(sampleRate, samplesPerBlock);
    }

    // Process audio through ALL effects
    void process(float* channelData, int numSamples, int channel)
    {
        for (auto& effect : effects)
            effect->process(channelData, numSamples, channel);
    }

    // TEMP: get effect by index (will improve later)
    Effect* getEffect(int index)
    {
        if (index < 0 || index >= (int)effects.size())
            return nullptr;

        return effects[index].get();
    }

private:
    std::vector<std::unique_ptr<Effect>> effects;
};