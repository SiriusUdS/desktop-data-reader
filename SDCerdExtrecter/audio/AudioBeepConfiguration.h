#pragma once

struct AudioBeepConfiguration {
  float frequency_hz = 440.0f;
  float duration_sec = 1.0f;
  float fadeIn_sec = 0.1f;
  float fadeOut_sec = 0.1f;
  int sampleRate = 44100;
  float amplitude = 30000.f;
};
