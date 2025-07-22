#include "SDCardReader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdio>

bool SDCardReader::readFromFile(const std::string& filename) {
  uint8_t tempBuffer[SD_CARD_BUFFER_SIZE_BYTES] = { 0 };
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Failed to open file: " << filename << '\n';
    bytesRead_ = 0;
    return false;
  }
  file.read(reinterpret_cast<char*>(tempBuffer), SD_CARD_BUFFER_SIZE_BYTES);
  bytesRead_ = static_cast<size_t>(file.gcount());
  for (size_t i = 0; i < bytesRead_; i++) {
    buffer.raw[i] = tempBuffer[i];
  }
  file.close();
  return true;
}

size_t SDCardReader::getBytesRead() const {
  return bytesRead_;
}

const SDCardBuffer& SDCardReader::getBuffer() const {
  return buffer;
}