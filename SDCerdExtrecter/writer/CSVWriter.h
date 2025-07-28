#pragma once
#include <fstream>
#include <string>
#include <vector>

#include "BeepQueueManager.h"
#include "SDCardBuffer.h"
#include "Random.h"

inline const std::vector<std::string> DEFAULT_COLUMNS = {
  "Timestamp_ms",
  "Status",
  "ErrorStatus",
  "ValveStatus_0",
  "ValveStatus_1",
  "ValveErrorStatus_0",
  "ValveErrorStatus_1",
  "CurrentCommand_0",
  "CurrentCommand_1",
  "CurrentCommand_2",
  "Signature",
  "Crc",
  "ADC_0",
  "ADC_1",
  "ADC_2",
  "ADC_3",
  "ADC_4",
  "ADC_5",
  "ADC_6",
  "ADC_7",
  "ADC_8",
  "ADC_9",
  "ADC_10",
  "ADC_11",
  "ADC_12",
  "ADC_13",
  "ADC_14",
  "ADC_15",
  "Padding"
};

class CSVWriter {
public:
	CSVWriter(const std::string& fileName, char delimiter = ',', const std::shared_ptr<BeepQueueManager>& beepManager = nullptr);
	~CSVWriter();
	CSVWriter(const CSVWriter&) = delete;
	CSVWriter& operator=(const CSVWriter&) = delete;
	CSVWriter(CSVWriter&&) noexcept = delete;
	CSVWriter& operator=(CSVWriter&&) noexcept = delete;

	void open(const std::string& fileName);
	void close();
	void writeHeader(const std::string& header);
	void writeHeader(const std::vector<std::string>& header);
  void writeDefaultHeader();
	void writeRow(const std::string& row);
	void writeRow(const std::vector<uint32_t>& row);
  void writeRow(const SDCardFormattedData& rowData);

private:
	[[nodiscard]] bool validateRowData(const std::string& data) const;
	[[nodiscard]] bool validateRowData(const std::vector<uint32_t>& data) const;
  [[nodiscard]] bool validateRowData(const SDCardFormattedData& data) const;

private:
  std::shared_ptr<BeepQueueManager> beepManager;
	std::ofstream file;
	std::string fileName;
	size_t headerColumnCount = 0;
  char delimiter = ',';
};
