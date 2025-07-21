#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>
#include <random>

struct BeepConfig {
  float frequency;
  float duration_sec;
  float fadeIn_sec;
  float fadeOut_sec;
  int sampleRate = 44100;
  float amplitude = 30000.f;
};

struct BeepGenerator {
  static std::vector<int16_t> generate(const BeepConfig& config) {
    const int sampleCount = static_cast<int>(static_cast<float>(config.sampleRate) * config.duration_sec);
    std::vector<int16_t> samples(sampleCount);
    for (int sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++) {
      const float time_sec = static_cast<float>(sampleIndex) / static_cast<float>(config.sampleRate);
      // amplitudeMultiplier scales the amplitude for fade-in and fade-out effects
      float amplitudeMultiplier = 1.f;
      if (config.fadeIn_sec > 0 && time_sec < config.fadeIn_sec) {
        amplitudeMultiplier = time_sec / config.fadeIn_sec;
      }
      else if (config.fadeOut_sec > 0 && time_sec > config.duration_sec - config.fadeOut_sec) {
        amplitudeMultiplier = (config.duration_sec - time_sec) / config.fadeOut_sec;
      }
      const float value = config.amplitude * amplitudeMultiplier * std::sin(2 * 3.14159265f * config.frequency * time_sec);
      samples[sampleIndex] = static_cast<int16_t>(value);
    }
    return samples;
  }
};

struct BeepPlayer {
  static void play(const BeepConfig& config) {
    const std::vector<int16_t> samples = BeepGenerator::generate(config);
    sf::SoundBuffer buffer;
    if (!buffer.loadFromSamples(samples.data(), samples.size(), 1, config.sampleRate)) {
      std::cerr << "Failed to load samples into SoundBuffer.\n";
      return;
    }
    sf::Sound sound(buffer);
    sound.play();
    sf::sleep(sf::seconds(config.duration_sec));
  }
};

void testRandomBeeps(int count) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> freqDist(200.0f, 2000.0f);
  std::uniform_real_distribution<float> durDist(0.05f, 0.2f); // 50ms to 200ms
  for (int i = 0; i < count; i++) {
    float freq = freqDist(gen);
    float dur = durDist(gen);
    float fade = dur / 2.0f; // 25% of beep duration
    BeepPlayer::play({freq, dur, fade, fade});
  }
}

int main() {
  testRandomBeeps(100);
  return 0;
}