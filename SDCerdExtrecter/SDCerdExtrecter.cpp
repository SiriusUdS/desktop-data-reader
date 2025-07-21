#include <SFML/Audio.hpp>
#include <iostream>
#include <numbers>
#include <vector>
#include <cmath>
#include <cstdint>
#include <random>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

struct BeepConfig {
  float frequency_hz = 440.0f;
  float duration_sec = 1.0f;
  float fadeIn_sec = 0.1f;
  float fadeOut_sec = 0.1f;
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
      const float value = config.amplitude * amplitudeMultiplier * std::sin(2 * std::numbers::pi_v<float> * config.frequency_hz * time_sec);
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
		// Wait for the sound to finish playing, used for testing purposes
    while (sound.getStatus() == sf::Sound::Playing) {
      sf::sleep(sf::milliseconds(1));
    }
  }
};

std::queue<BeepConfig> beepQueue;
std::mutex queueMutex;
std::condition_variable queueCV;
bool done = false;

void producer(int count) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> freqDist(200.0f, 2000.0f);
  std::uniform_real_distribution<float> durDist(0.1f, 0.3f);

  for (int i = 0; i < count; i++) {
    BeepConfig config;
    config.frequency_hz = freqDist(gen);
    config.duration_sec = durDist(gen);
    config.fadeIn_sec = 0.02f;
    config.fadeOut_sec = 0.02f;
    config.sampleRate = 44100;
    config.amplitude = 10000.f;
    {
      std::lock_guard<std::mutex> lock(queueMutex);
      beepQueue.push(config);
			std::cout << "Produced beep " << i + 1 << " with frequency " << config.frequency_hz << " Hz\n";
    }
    queueCV.notify_one();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  {
    std::lock_guard<std::mutex> lock(queueMutex);
    done = true;
  }
  queueCV.notify_one();
}

void consumer() {
  std::vector<std::unique_ptr<sf::SoundBuffer>> buffers;
  std::vector<std::unique_ptr<sf::Sound>> sounds;
  while (true) {
    BeepConfig config;
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      queueCV.wait(lock, [] { return !beepQueue.empty() || done; });
      if (beepQueue.empty() && done) break;
      config = beepQueue.front();
      beepQueue.pop();
      std::cout << "Consumed beep with frequency " << config.frequency_hz << " Hz\n";
    }
    auto buffer = std::make_unique<sf::SoundBuffer>();
    const auto samples = BeepGenerator::generate(config);
    if (!buffer->loadFromSamples(samples.data(), samples.size(), 1, config.sampleRate)) {
      std::cerr << "Failed to load samples into SoundBuffer.\n";
      continue;
    }
    auto sound = std::make_unique<sf::Sound>(*buffer);
    sound->play();
    buffers.push_back(std::move(buffer));
    sounds.push_back(std::move(sound));

    for (size_t i = 0; i < sounds.size();) {
      if (sounds[i]->getStatus() == sf::Sound::Stopped) {
        sounds.erase(sounds.begin() + i);
        buffers.erase(buffers.begin() + i);
      } else {
        i++;
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

int main() {
  std::thread prod(producer, 250); // produce 10 beeps
  std::thread cons(consumer);
  prod.join();
  cons.join();
  return 0;
}