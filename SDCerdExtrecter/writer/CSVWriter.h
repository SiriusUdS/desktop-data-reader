#pragma once
#include <fstream>
#include <string>
#include <vector>

class CSVWriter {
public:
	CSVWriter(const std::string& fileName, char delimiter = ',');
	~CSVWriter();
	CSVWriter(const CSVWriter&) = delete;
	CSVWriter& operator=(const CSVWriter&) = delete;
	CSVWriter(CSVWriter&&) noexcept = default;
	CSVWriter& operator=(CSVWriter&&) noexcept = default;

	void open(const std::string& fileName);
	void close();
	void writeHeader(const std::string& header);
	void writeHeader(const std::vector<uint32_t>& header);
	void writeRow(const std::string& row);
	void writeRow(const std::vector<uint32_t>& row);

private:
	[[nodiscard]] bool validateRowData(const std::string& data) const;
	[[nodiscard]] bool validateRowData(const std::vector<uint32_t>& data) const;

private:
	std::ofstream file;
	std::string fileName;
	size_t headerColumnCount = 0;
  char delimiter = ',';
};
