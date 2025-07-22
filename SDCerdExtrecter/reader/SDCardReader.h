#pragma once
#include <string>
#include "SDCardBuffer.h"

class SDCardReader {
public:
	SDCardReader() = default;
	~SDCardReader() = default;
	SDCardReader(const SDCardReader&) = default; // Copy constructor
	SDCardReader& operator=(const SDCardReader&) = default; // Copy assignment operator
	SDCardReader(SDCardReader&&) noexcept = default; // Move constructor
	SDCardReader& operator=(SDCardReader&&) noexcept = default; // Move assignment operator

  bool readFromFile(const std::string& filename);
  [[nodiscard]] size_t getBytesRead() const;
  [[nodiscard]] const SDCardBuffer& getBuffer() const;

private:
  SDCardBuffer buffer;
  size_t bytesRead_ = 0;
};