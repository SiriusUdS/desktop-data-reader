#include "SDCardReader.h"

SDCardReader::SDCardReader(const std::string& filename) :pagesRead(0) {
  buffer = SDCardPageBuffer{};
  this->filename = filename;
  file.open(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Failed to open file: " << filename << '\n';
    return;
	}
	file.seekg(0, std::ios::end);
	fileSize = static_cast<size_t>(file.tellg());
  if (fileSize == 0) {
    std::cerr << "File is empty: " << filename << '\n';
	}
  numberOfFilledPages = fileSize / sizeof(SDCardPageBuffer);
	file.seekg(0, std::ios::beg);
}

SDCardPageBuffer& SDCardReader::readNext() {
    file.read(reinterpret_cast<char*>(&buffer.formatted), sizeof(SDCardFormattedData));
    const std::streamsize bytesReadNow = file.gcount();
    bytesRead += static_cast<size_t>(bytesReadNow);
    pagesRead++;
    if (bytesReadNow != sizeof(SDCardFormattedData)) {
        throw std::runtime_error("End of file reached or insufficient data for SDCardFormattedData.");
    }
    return buffer;
}

bool SDCardReader::isEndOfFile() const {
  if (!file) {
    return true;
  }
  if (file.eof() || bytesRead >= fileSize) {
    return true;
  }
  return false;
}

size_t SDCardReader::getBytesRead() const {
  return bytesRead;
}

const SDCardPageBuffer& SDCardReader::getBuffer() const {
  return buffer;
}