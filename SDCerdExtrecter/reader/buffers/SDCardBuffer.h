#pragma once
#include <cstdint>
#include <cstddef>

constexpr size_t SD_CARD_BUFFER_SIZE_BYTES = 0x10000;
constexpr size_t SD_CARD_FOOTER_SIZE_BYTES = 0x80;
constexpr size_t SD_CARD_FOOTER_SIGNATURE_SIZE_BYTES = 0x40;

struct SDCardFooter {
  uint32_t timestamp_ms;
  uint16_t status;
  uint16_t errorStatus;
  uint16_t valveStatus[2];
  uint16_t valveErrorStatus[2];
  uint32_t currentCommand[3];
  uint8_t padding[32];
  uint32_t signature[SD_CARD_FOOTER_SIGNATURE_SIZE_BYTES / sizeof(uint32_t)];
  uint32_t crc;
};

struct SDCardFormattedData {
  uint8_t data[(SD_CARD_BUFFER_SIZE_BYTES / 2) - sizeof(SDCardFooter)];
  SDCardFooter footer;
};

union SDCardBuffer {
  uint16_t values[SD_CARD_BUFFER_SIZE_BYTES / sizeof(uint16_t)];
  SDCardFormattedData formattedData[2];
  uint8_t raw[SD_CARD_BUFFER_SIZE_BYTES];
};