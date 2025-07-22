//#include <iostream>
//#include <thread>
//
//#include "BeepQueueManager.h"
//#include "SDCardReader.h"
//
//int main() {
//
//
//
//  const std::string filename = "E:/4/Data.bin";
//  SDCardReader reader(filename);
//  SDCardFormattedData buffer = reader.readNext();
//
//  BeepQueueManager manager;
//  std::thread prod(&BeepQueueManager::produceBeeps, &manager, 250);
//  std::thread cons(&BeepQueueManager::consumeBeeps, &manager);
//  prod.join();
//  cons.join();
//
//  return 0;
//}


#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <limits>

#include "BeepQueueManager.h"
#include "SDCardReader.h"

namespace fs = std::filesystem;

// Logo function
void printLogo() {
  std::cout << R"(
  ____  ____     ____             _   _____      _                 _            
 / ___||  _ \   / ___|___ _ __ __| | | ____|_  _| |_ _ __ ___  ___| |_ ___ _ __ 
 \___ \| | | | | |   / _ \ '__/ _` | |  _| \ \/ / __| '__/ _ \/ __| __/ _ \ '__|
  ___) | |_| | | |__|  __/ | | (_| | | |___ >  <| |_| | |  __/ (__| ||  __/ |   
 |____/|____/   \____\___|_|  \__,_| |_____/_/\_\\__|_|  \___|\___|\__\___|_|                                                                
)" << std::endl;
  std::cout << "==================== SDCerdExtrecter ====================\n" << '\n';
}

// Recursively print directory tree
void printDirectoryTree(const fs::path& root, int indent = 0) {
  for (const auto& entry : fs::directory_iterator(root)) {
    std::cout << std::string(indent, ' ');
    if (entry.is_directory()) {
      std::cout << "[DIR]  " << entry.path().filename().string() << '\n';
      printDirectoryTree(entry.path(), indent + 4);
    } else {
      std::cout << "[FILE] " << entry.path().filename().string() << '\n';
    }
  }
}

// Collect all files for selection
void collectFiles(const fs::path& root, std::vector<fs::path>& files) {
  for (const auto& entry : fs::recursive_directory_iterator(root)) {
    if (entry.is_regular_file()) {
      files.push_back(entry.path());
    }
  }
}

int main() {
  printLogo();

  std::string sdRoot;
  std::cout << "Enter SD card root directory (e.g., E:/): ";
  std::getline(std::cin, sdRoot);

  if (!fs::exists(sdRoot) || !fs::is_directory(sdRoot)) {
    std::cerr << "Invalid SD card root directory.\n";
    return 1;
  }

  std::cout << "\nSD Card Directory Structure:\n";
  printDirectoryTree(sdRoot);

  std::vector<fs::path> files;
  collectFiles(sdRoot, files);

  if (files.empty()) {
    std::cerr << "No files found on SD card.\n";
    return 1;
  }

  std::cout << "\nSelect a file to use as target:\n";
  for (size_t i = 0; i < files.size(); ++i) {
    std::cout << "  [" << i + 1 << "] " << files[i].string() << '\n';
  }

  size_t choice = 0;
  while (true) {
    std::cout << "Enter file number: ";
    std::cin >> choice;
    if (std::cin.fail() || choice < 1 || choice > files.size()) {
			std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid choice. Try again.\n";
    } else {
      break;
    }
  }

  std::string filename = files[choice - 1].string();
  std::cout << "\nSelected file: " << filename << "\n";

  SDCardReader reader(filename);
  SDCardFormattedData buffer = reader.readNext();

  // Continue with your application logic...

  return 0;
}