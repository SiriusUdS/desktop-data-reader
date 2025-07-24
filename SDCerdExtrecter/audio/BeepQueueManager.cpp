#include "BeepQueueManager.h"
#include <random>
#include <iostream>
#include <SFML/Audio.hpp>
#include <thread>

void BeepQueueManager::enqueueBeep(const AudioBeepConfiguration& config) {
  auto buffer = std::make_unique<sf::SoundBuffer>();
  const auto samples = AudioSignalGenerator::generate(config);
  if (!buffer->loadFromSamples(samples.data(), samples.size(), 1, config.sampleRate)) {
    std::cerr << "Failed to load samples into SoundBuffer.\n";
    return;
  }
  auto sound = std::make_unique<sf::Sound>(*buffer);
  sound->play();
  buffers.push_back(std::move(buffer));
  sounds.push_back(std::move(sound));
  cleanupStoppedSounds();
}

void BeepQueueManager::cleanupStoppedSounds() {
  for (size_t i = 0; i < sounds.size();) {
    if (sounds[i]->getStatus() == sf::Sound::Stopped) {
      sounds.erase(sounds.begin() + i);
      buffers.erase(buffers.begin() + i);
    } else {
      i++;
    }
  }
}
