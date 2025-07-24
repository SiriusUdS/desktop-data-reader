#pragma once
#include <vector>
#include <memory>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include "AudioBeepConfiguration.h"
#include "AudioSignalGenerator.h"

class BeepQueueManager {
public:
  BeepQueueManager() = default;
  ~BeepQueueManager() = default;
  BeepQueueManager(const BeepQueueManager&) = delete;
  BeepQueueManager& operator=(const BeepQueueManager&) = delete;
  BeepQueueManager(BeepQueueManager&&) = delete;
  BeepQueueManager& operator=(BeepQueueManager&&) = delete;

  void enqueueBeep(const AudioBeepConfiguration& config);
  void cleanupStoppedSounds();

private:
  std::vector<std::unique_ptr<sf::SoundBuffer>> buffers;
  std::vector<std::unique_ptr<sf::Sound>> sounds;
};
