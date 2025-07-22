#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <SFML/Audio/SoundBuffer.hpp>

#include "AudioBeepConfiguration.h"
#include "AudioSignalGenerator.h"

class BeepQueueManager {
public:
  void produceBeeps(int count);
  void consumeBeeps();

private:
  void pushBeepConfig(const AudioBeepConfiguration& config, int index);
  void setProducerDone();
  bool fetchNextBeepConfig(AudioBeepConfiguration& config);
  static void playBeep(const AudioBeepConfiguration& config, 
											 std::vector<std::unique_ptr<sf::SoundBuffer>> &buffers, 
											 std::vector<std::unique_ptr<sf::Sound>> &sounds);
  static void cleanupStoppedSounds(std::vector<std::unique_ptr<sf::SoundBuffer>> &buffers, 
																	 std::vector<std::unique_ptr<sf::Sound>> &sounds);

private:
  std::queue<AudioBeepConfiguration> beepConfigurationQueue;
  std::mutex beepQueueMutex;
  std::condition_variable beepQueueCondition;
  bool isBeepProducerDone = false;
};
