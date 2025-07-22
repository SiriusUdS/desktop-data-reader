#pragma once
#include "AudioBeepConfiguration.h"

class AudioBeepPlaybackManager {
public:
  static bool play(const AudioBeepConfiguration& config);
};
