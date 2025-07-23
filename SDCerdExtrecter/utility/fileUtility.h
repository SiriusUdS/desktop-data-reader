#pragma once
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace FileUtility {
	static void printDirectoryTree(const fs::path& root, const std::string& prefix = "") {
    std::vector<fs::directory_entry> entries;
    for (const auto& entry : fs::directory_iterator(root)) {
      entries.push_back(entry);
    }
    for (size_t i = 0; i < entries.size(); ++i) {
      const auto& entry = entries[i];
      bool isLast = (i == entries.size() - 1);
      std::cout << prefix
                << (isLast ? "`-- " : "|-- ")
                << (entry.is_directory() ? "[DIR]  " : "[FILE] ")
                << entry.path().filename().string() << '\n';
      if (entry.is_directory()) {
        printDirectoryTree(entry.path(), prefix + (isLast ? "    " : "|   "));
      }
    }
	}

  static void collectDirectories(const fs::path& root, std::vector<fs::path>& dirs) {
    for (const auto& entry : fs::directory_iterator(root)) {
      if (entry.is_directory()) {
        dirs.push_back(entry.path());
      }
    }
  }

  static void collectFilesInDir(const fs::path& dir, std::vector<fs::path>& files) {
    for (const auto& entry : fs::directory_iterator(dir)) {
      if (entry.is_regular_file()) {
        files.push_back(entry.path());
      }
    }
  }
}