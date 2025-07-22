#pragma once
#include <string>
#include "SDCardBuffer.h"

class SDCardReader {
public:
	SDCardReader() = default;
	~SDCardReader() = default;

  bool readFromFile(const std::string& filename);
  size_t getBytesRead() const;
  const SDCardBuffer& getBuffer() const;
  void printHex(size_t count = 16) const;

private:
  SDCardBuffer buffer;
  size_t bytesRead_ = 0;
};