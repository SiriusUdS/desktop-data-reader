#include "CSVWriter.h"

CSVWriter::CSVWriter(const std::string& fileName, char delimiter, const std::shared_ptr<BeepQueueManager>& beepManager) {
  this->delimiter = delimiter;
  this->beepManager = beepManager;
  open(fileName);
}

CSVWriter::~CSVWriter() {
  if (file.is_open()) {
    file.flush();
    file.close();
  }
}

void CSVWriter::open(const std::string& fileName) {
  this->fileName = fileName;
  file.open(fileName, std::ios::out | std::ios::trunc);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + fileName);
  }
}

void CSVWriter::close() {
  if (file.is_open()) {
    file.close();
  }
}

void CSVWriter::writeHeader(const std::string& header) {
  if (!file.is_open()) {
    throw std::runtime_error("File is not open: " + fileName);
  }
  const size_t columnCount = std::count(header.begin(), header.end(), delimiter) + 1;
  if (headerColumnCount != 0 && columnCount != headerColumnCount) {
    // Might do something later if we wanna check that the user doesn't change the header format
  }
  headerColumnCount = columnCount;
  if (headerColumnCount == 0) {
    throw std::runtime_error("Header is empty or invalid (Missing delimiter?): " + header);
  }
  file << header << '\n';
}

void CSVWriter::writeHeader(const std::vector<std::string>& header) {
  if (!file.is_open()) {
    throw std::runtime_error("File is not open: " + fileName);
  }
  const size_t columnCount = header.size();
  if (headerColumnCount != 0 && columnCount != headerColumnCount) {
    // Might do something later if we wanna check that the user doesn't change the header format
  }
  headerColumnCount = columnCount;
  for (size_t i = 0; i < header.size(); i++) {
    file << header[i];
    if (i < header.size() - 1) {
      file << delimiter;
    }
  }
  file << '\n';
}

void CSVWriter::writeDefaultHeader() {
  writeHeader(DEFAULT_COLUMNS);
}

void CSVWriter::writeRow(const std::string& row) {
  if (!file.is_open()) {
    throw std::runtime_error("File is not open: " + fileName);
  }
  if (!validateRowData(row)) {
    throw std::runtime_error("Invalid row data: " + row);
  }
  file << row << '\n';
}

void CSVWriter::writeRow(const std::vector<uint32_t>& row) {
  if (!file.is_open()) {
    throw std::runtime_error("File is not open: " + fileName);
  }
  if (!validateRowData(row)) {
    throw std::runtime_error("Invalid row data: size mismatch");
  }
  for (size_t i = 0; i < row.size(); i++) {
    file << row[i];
    if (i < row.size() - 1) {
      file << delimiter;
    }
  }
  file << '\n';
}

void CSVWriter::writeRow(const SDCardFormattedData& rowData) {
  if (!file.is_open()) {
    throw std::runtime_error("File is not open: " + fileName);
  }
  const ADCChunk* chunks = reinterpret_cast<const ADCChunk*>(rowData.data);
  const SDCardFooter footer = rowData.footer;

  static uint32_t firstTimestamp_us = 0;
  static uint32_t timeSinceIgnite_us = 0;

  for (size_t chunkIdx = 0; chunkIdx < CHUNKS_PER_PAGE; chunkIdx++) {
    const ADCChunk& chunk = chunks[chunkIdx];

    if (firstTimestamp_us == 0) {
      firstTimestamp_us = footer.timestamp_ms * 1000;
      timeSinceIgnite_us = firstTimestamp_us;
    }

    file << firstTimestamp_us - timeSinceIgnite_us << delimiter //(deltaTimestamp == 0u ? footer.timestamp_ms : footer.timestamp_ms + (deltaTimestamp * (chunkIdx + 1))) << delimiter
         << footer.status                          << delimiter
         << footer.errorStatus                     << delimiter
         << footer.valveStatus[0]                  << delimiter
         << footer.valveStatus[1]                  << delimiter
         << footer.valveErrorStatus[0]             << delimiter
         << footer.valveErrorStatus[1]             << delimiter
         << footer.currentCommand[0]               << delimiter
         << footer.currentCommand[1]               << delimiter
         << footer.currentCommand[2]               << delimiter
         << footer.signature                       << delimiter
         << footer.crc                             << delimiter;

    uint64_t adcMean = 0;
    firstTimestamp_us += ((213 * 1000) / 256);

    for (const uint16_t adcValue : chunk.adcChannelData) {
      file << adcValue << delimiter;
      adcMean += adcValue;
    }
    adcMean /= ADC_CHANNEL_SECTION_SIZE;

    AudioBeepConfiguration config;
    config.frequency_hz = static_cast<float>(adcMean) * random(1, 50);
    config.duration_sec = 0.25f;
    config.fadeIn_sec   = 0.05f;
    config.fadeOut_sec  = 0.05f;
    config.sampleRate   = random(50, 44100);
    config.amplitude    = beepManager->enableEarrape ? 10000.f : 5000.f;
    beepManager->enqueueBeep(config);

    file << footer.padding << '\n';
    //i++;
  }
}

bool CSVWriter::validateRowData(const std::string& data) const {
  const size_t dataColumnCount = std::count(data.begin(), data.end(), delimiter) + 1;
  if (dataColumnCount != headerColumnCount) {
    return false;
  }
  return true;
}

bool CSVWriter::validateRowData(const std::vector<uint32_t>& data) const {
  const size_t dataColumnCount = data.size();
  if (dataColumnCount != headerColumnCount) {
    return false;
  }
  return true;
}