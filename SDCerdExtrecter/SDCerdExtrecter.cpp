#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <limits>

#include "BeepQueueManager.h"
#include "SDCardReader.h"
#include "fileUtility.h"

namespace fs = std::filesystem;

void printLogo();

int main() {
  printLogo();

  std::string sdCardRootDir;
  std::cout << "Enter SD card root directory (e.g., E:/): ";
  std::getline(std::cin, sdCardRootDir);

  if (!fs::exists(sdCardRootDir) || !fs::is_directory(sdCardRootDir)) {
    std::cerr << "Invalid SD card root directory.\n";
    return 1;
  }

  std::string selectedFile;
  std::string lastAction;
  bool exitProgramRequested = false;
  while (!exitProgramRequested) {
    #ifdef _WIN32
      system("cls");
    #else
      system("clear");
    #endif

    printLogo();
    std::cout << "\n===== Main Menu =====\n";
    std::cout << "1. Select SD card file";
    if (!selectedFile.empty()) {
      std::cout << " [" << selectedFile << "]";
    }
    std::cout << "\n";
    std::cout << "2. Show SD card directory tree\n";
    std::cout << "3. Exit\n";
    if (!lastAction.empty()) {
      std::cout << "\n" << lastAction << "\n";
    }
    std::cout << "Enter choice: ";

    int mainMenuChoice = 0;
    std::cin >> mainMenuChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (mainMenuChoice) {
      case 1: {
        std::vector<fs::path> directories;
        FileUtility::collectDirectories(sdCardRootDir, directories);
        if (directories.empty()) {
          lastAction = "No directories found on SD card.";
          break;
        }
        std::cout << "\nSelect a directory:\n";
        for (size_t i = 0; i < directories.size(); ++i) {
          std::cout << "  [" << i + 1 << "] " << directories[i].filename().string() << '\n';
        }
        unsigned int directoryChoice = 0;
        while (true) {
          std::cout << "Enter directory index: ";
          std::cin >> directoryChoice;
          if (std::cin.fail() || directoryChoice < 1 || directoryChoice > static_cast<unsigned int>(directories.size())) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice.\n";
          } else {
            break;
          }
        }
        fs::path selectedDir = directories[directoryChoice - 1];
        std::vector<fs::path> files;
        FileUtility::collectFilesInDir(selectedDir, files);
        if (files.empty()) {
          lastAction = "No files found in selected directory.";
          break;
        }
        std::cout << "\nSelect a file:\n";
        for (size_t i = 0; i < files.size(); i++) {
          std::cout << "  [" << i + 1 << "] " << files[i].filename().string() << '\n';
        }
        size_t fileChoice = 0;
        while (true) {
          std::cout << "Enter file index: ";
          std::cin >> fileChoice;
          if (std::cin.fail() || fileChoice < 1 || fileChoice > files.size()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice.\n";
          } else {
            break;
          }
        }
        selectedFile = files[fileChoice - 1].string();
        lastAction = "Selected file: " + selectedFile;
        break;
      }
      case 2:
        std::cout << "\nSD Card Directory Structure:\n";
        FileUtility::printDirectoryTree(sdCardRootDir);
        lastAction = "Displayed SD card directory tree.";
        std::cout << "\nPress Enter to return to menu...";
        std::cin.get();
        break;
      case 3:
        exitProgramRequested = true;
        break;
      default:
        lastAction = "Invalid choice.";
        break;
    }
  }
  return 0;
}

void printLogo() {
  std::cout << R"(
  ____  ____     ____             _   _____      _                 _            
 / ___||  _ \   / ___|___ _ __ __| | | ____|_  _| |_ _ __ ___  ___| |_ ___ _ __ 
 \___ \| | | | | |   / _ \ '__/ _` | |  _| \ \/ / __| '__/ _ \/ __| __/ _ \ '__|
  ___) | |_| | | |__|  __/ | | (_| | | |___ >  <| |_| | |  __/ (__| ||  __/ |   
 |____/|____/   \____\___|_|  \__,_| |_____/_/\_\\__|_|  \___|\___|\__\___|_|                                                                
)" << '\n';
  std::cout << "==================== SDCerdExtrecter ====================\n" << '\n';
}