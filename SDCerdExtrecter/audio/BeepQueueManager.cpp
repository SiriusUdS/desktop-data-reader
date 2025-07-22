#include "BeepQueueManager.h"
#include <random>
#include <iostream>
#include <SFML/Audio.hpp>
#include <thread>

void BeepQueueManager::produceBeeps(int count) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> freqDist(200.0f, 2000.0f);
  std::uniform_real_distribution<float> durDist(0.1f, 0.3f);
  for (int i = 0; i < count; i++) {
    AudioBeepConfiguration config;
    config.frequency_hz = freqDist(gen);
    config.duration_sec = durDist(gen);
    config.fadeIn_sec = 0.02f;
    config.fadeOut_sec = 0.02f;
    config.sampleRate = 44100;
    config.amplitude = 10000.f;
    pushBeepConfig(config, i);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  setProducerDone();
}

void BeepQueueManager::consumeBeeps() {
  std::vector<std::unique_ptr<sf::SoundBuffer>> buffers;
  std::vector<std::unique_ptr<sf::Sound>> sounds;
  while (true) {
    AudioBeepConfiguration config;
    if (!fetchNextBeepConfig(config)) break;
    playBeep(config, buffers, sounds);
    cleanupStoppedSounds(buffers, sounds);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void BeepQueueManager::pushBeepConfig(const AudioBeepConfiguration& config, int index) {
  std::lock_guard<std::mutex> lock(beepQueueMutex);
  beepConfigurationQueue.push(config);
  std::cout << "Produced beep " << index + 1 << " with frequency " << config.frequency_hz << " Hz\n";
  beepQueueCondition.notify_one();
}

void BeepQueueManager::setProducerDone() {
  std::lock_guard<std::mutex> lock(beepQueueMutex);
  isBeepProducerDone = true;
  beepQueueCondition.notify_one();
}

bool BeepQueueManager::fetchNextBeepConfig(AudioBeepConfiguration& config) {
  std::unique_lock<std::mutex> lock(beepQueueMutex);
  beepQueueCondition.wait(lock, [this] { return !beepConfigurationQueue.empty() || isBeepProducerDone; });
  if (beepConfigurationQueue.empty() && isBeepProducerDone) return false;
  config = beepConfigurationQueue.front();
  beepConfigurationQueue.pop();
  std::cout << "Consumed beep with frequency " << config.frequency_hz << " Hz\n";
  return true;
}

void BeepQueueManager::playBeep(const AudioBeepConfiguration& config, std::vector<std::unique_ptr<sf::SoundBuffer>>& buffers, std::vector<std::unique_ptr<sf::Sound>>& sounds) {
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
}

void BeepQueueManager::cleanupStoppedSounds(std::vector<std::unique_ptr<sf::SoundBuffer>>& buffers, std::vector<std::unique_ptr<sf::Sound>>& sounds) {
  for (size_t i = 0; i < sounds.size();) {
    if (sounds[i]->getStatus() == sf::Sound::Stopped) {
      sounds.erase(sounds.begin() + i);
      buffers.erase(buffers.begin() + i);
    } else {
      i++;
    }
  }
}
