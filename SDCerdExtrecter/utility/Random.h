#pragma once
#include <cstdlib>

inline int random(int min, int max) {
  return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}
