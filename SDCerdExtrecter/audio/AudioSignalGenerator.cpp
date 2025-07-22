#include "AudioSignalGenerator.h"
#include <cmath>
#include <numbers>

std::vector<int16_t> AudioSignalGenerator::generate(const AudioBeepConfiguration& config) {
  const int sampleCount = static_cast<int>(static_cast<float>(config.sampleRate) * config.duration_sec);
  std::vector<int16_t> samples(sampleCount);
  for (int sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++) {
    const float time_sec = static_cast<float>(sampleIndex) / static_cast<float>(config.sampleRate);
    float amplitudeMultiplier = getFadeMultiplier(config, time_sec);
    const float value = config.amplitude * amplitudeMultiplier * std::sin(2 * std::numbers::pi_v<float> * config.frequency_hz * time_sec);
    samples[sampleIndex] = static_cast<int16_t>(value);
  }
  return samples;
}

float AudioSignalGenerator::getFadeMultiplier(const AudioBeepConfiguration& config, float time_sec) {
  if (config.fadeIn_sec > 0 && time_sec < config.fadeIn_sec) {
    return time_sec / config.fadeIn_sec;
  }
  if (config.fadeOut_sec > 0 && time_sec > config.duration_sec - config.fadeOut_sec) {
    return (config.duration_sec - time_sec) / config.fadeOut_sec;
  }
  return 1.f;
}
