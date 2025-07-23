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

constexpr size_t SD_CARD_DATA_SIZE = (SD_CARD_BUFFER_SIZE_BYTES / 2) - sizeof(SDCardFooter);
constexpr size_t ADC_CHUNK_SIZE = 0x100;
constexpr size_t ADC_CHANNEL_SECTION_SIZE = 0x10;
constexpr size_t ADC_SECTIONS_PER_CHUNK = ADC_CHUNK_SIZE / ADC_CHANNEL_SECTION_SIZE;
constexpr size_t CHUNKS_PER_PAGE = SD_CARD_DATA_SIZE / ADC_CHUNK_SIZE;

struct ADCChunk {
  uint16_t adcChannelData[ADC_CHANNEL_SECTION_SIZE];
};

struct SDCardFormattedData {
  union {
    uint8_t data[SD_CARD_DATA_SIZE];
    ADCChunk chunks[CHUNKS_PER_PAGE];
  };
  SDCardFooter footer;
};