#include "CSVWriter.h"

#include <stdexcept>

CSVWriter::CSVWriter(const std::string& fileName, char delimiter) {
  this->delimiter = delimiter;
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

void CSVWriter::writeHeader(const std::vector<uint32_t>& header) {
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