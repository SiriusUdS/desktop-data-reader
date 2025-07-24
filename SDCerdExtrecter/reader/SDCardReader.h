#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "SDCardBuffer.h"

class SDCardReader {
public:
	SDCardReader(const std::string& filename);
	~SDCardReader() = default;
	SDCardReader(const SDCardReader&) = default; // Copy constructor
	SDCardReader& operator=(const SDCardReader&) = default; // Copy assignment operator
	SDCardReader(SDCardReader&&) noexcept = default; // Move constructor
	SDCardReader& operator=(SDCardReader&&) noexcept = default; // Move assignment operator

  [[nodiscard]] SDCardPageBuffer& readNext();
  [[nodiscard]] size_t getBytesRead() const;
  [[nodiscard]] const SDCardPageBuffer& getBuffer() const;
  [[nodiscard]] bool isEndOfFile() const;

private:
	std::ifstream file;
	std::string filename;
	size_t fileSize;
	size_t numberOfFilledPages;
	size_t pagesRead;
  SDCardPageBuffer buffer;
  size_t bytesRead = 0;
};