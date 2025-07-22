#include "AudioBeepPlaybackManager.h"
#include "AudioSignalGenerator.h"
#include <SFML/Audio.hpp>
#include <iostream>

bool AudioBeepPlaybackManager::play(const AudioBeepConfiguration& config) {
  const std::vector<int16_t> samples = AudioSignalGenerator::generate(config);
  sf::SoundBuffer buffer;
  if (!buffer.loadFromSamples(samples.data(), samples.size(), 1, config.sampleRate)) {
    std::cerr << "Failed to load samples into SoundBuffer.\n";
    return false;
  }
  sf::Sound sound(buffer);
  sound.play();
  while (sound.getStatus() == sf::Sound::Playing) {
    sf::sleep(sf::milliseconds(1));
  }
  return true;
}
