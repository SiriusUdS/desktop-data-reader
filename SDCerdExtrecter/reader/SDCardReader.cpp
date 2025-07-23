#include "SDCardReader.h"

SDCardReader::SDCardReader(const std::string& filename) :pagesRead(0) {
	buffer = SDCardFormattedData();
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
  numberOfFilledPages = fileSize / sizeof(SDCardFormattedData);
	file.seekg(0, std::ios::beg);
}

SDCardFormattedData& SDCardReader::readNext() {
  file.read(reinterpret_cast<char*>(buffer.data), sizeof(buffer.data));
  const std::streamsize dataBytesRead = file.gcount();

  file.read(reinterpret_cast<char*>(&buffer.footer), sizeof(buffer.footer));
  const std::streamsize footerBytesRead = file.gcount();

  bytesRead += static_cast<size_t>(dataBytesRead + footerBytesRead);
  pagesRead++;

  if (dataBytesRead != sizeof(buffer.data) || footerBytesRead != sizeof(buffer.footer)) {
		// TODO: Create custom exception class for this
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

const SDCardFormattedData& SDCardReader::getBuffer() const {
  return buffer;
}