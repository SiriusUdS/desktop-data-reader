#pragma once
#include <vector>
#include <cstdint>
#include "AudioBeepConfiguration.h"

class AudioSignalGenerator {
public:
  [[nodiscard]] static std::vector<int16_t> generate(const AudioBeepConfiguration& config);
private:
  [[nodiscard]] static float getFadeMultiplier(const AudioBeepConfiguration& config, float time_sec);
};
