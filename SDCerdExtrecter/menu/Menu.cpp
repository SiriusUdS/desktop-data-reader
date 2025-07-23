#include "Menu.h"
#include <iostream>
#include <limits>

#include "SDCardReader.h"
#include "../utility/FileUtility.h"

namespace fs = std::filesystem;

void Menu::run() {
  while (!exitRequested) {
    #ifdef _WIN32
      system("cls");
    #else
      system("clear");
    #endif
    displayMenu();
    const int choice = getUserChoice(1, 5, "Enter choice: ");
    handleMenuChoice(choice);
  }
}

void Menu::displayMenu() const {
  printLogo();
  std::cout << "\n===== Main Menu =====\n";
  std::cout << "1. Select SD card file";
  if (!selectedFile.empty()) {
    std::cout << " [" << selectedFile << "]";
  }
  std::cout << "\n2. Select output directory";
  if (!outputDir.empty()) {
    std::cout << " [" << outputDir << "]";
  } else {
    std::cout << " [Not set]";
  }
  std::cout << "\n3. Read SD card";
  std::cout << "\n4. Show SD card directory tree\n";
  std::cout << "5. Toggle sound";
  if (enableEarrape) {
    std::cout << " [LOUD]\n";
  } else {
    std::cout << " [ON]\n";
  }
  std::cout << "6. Exit\n";
  if (!lastAction.empty()) {
    std::cout << "\n" << lastAction << "\n";
  }
}

void Menu::handleMenuChoice(int choice) {
  switch (choice) {
    case 1:
      selectFile();
      break;
    case 2:
      selectOutputDir();
      break;
    case 3:
      readSDCard();
      break;
    case 4:
      showDirectoryTree();
      break;
    case 5:
      toggleSound();
      break;
    case 6:
      exitRequested = true;
      break;
    default:
      lastAction = "Invalid choice.";
      break;
  }
}

void Menu::readSDCard() {
  if (selectedFile.empty()) {
    lastAction = "No file selected. Please select a file first.";
    return;
  }
  if (outputDir.empty()) {
    lastAction = "No output directory set. Please set an output directory first.";
    return;
  }
  if (!fs::exists(selectedFile)) {
    lastAction = "Selected file does not exist.";
    return;
  }
  SDCardReader reader(selectedFile);
  while (!reader.isEndOfFile()) {
    SDCardFormattedData data = reader.readNext();

    std::string outputFile = outputDir + "/" + std::to_string(reader.getBytesRead()) + ".csv";

    //std::string outputFile = outputDir + "/" + std::to_string(reader.getBytesRead()) + ".bin";
    //std::ofstream outFile(outputFile, std::ios::binary);
    //if (!outFile) {
    //  lastAction = "Failed to open output file: " + outputFile;
    //  return;
    //}
    //outFile.write(reinterpret_cast<const char*>(data.data()), data.size());
    //outFile.close();
  }
}

void Menu::selectFile() {
  std::vector<fs::path> directories;
  FileUtility::collectDirectories(sdCardRootDir, directories);
  if (directories.empty()) {
    lastAction = "No directories found on SD card.";
    return;
  }
  std::cout << "\nSelect a directory:\n";
  for (size_t i = 0; i < directories.size(); i++) {
    std::cout << "  [" << i + 1 << "] " << directories[i].filename().string() << '\n';
  }
  int dirChoice = getUserChoice(1, static_cast<int>(directories.size()), "Enter directory index: ");
  fs::path selectedDir = directories[dirChoice - 1];
  std::vector<fs::path> files;
  FileUtility::collectFilesInDir(selectedDir, files);
  if (files.empty()) {
    lastAction = "No files found in selected directory.";
    return;
  }
  std::cout << "\nSelect a file:\n";
  for (size_t i = 0; i < files.size(); i++) {
    std::cout << "  [" << i + 1 << "] " << files[i].filename().string() << '\n';
  }
  int fileChoice = getUserChoice(1, static_cast<int>(files.size()), "Enter file index: ");
  selectedFile = files[fileChoice - 1].string();
  lastAction = "Selected file: " + selectedFile;
}

void Menu::showDirectoryTree() {
  std::cout << "\nSD Card Directory Structure:\n";
  FileUtility::printDirectoryTree(sdCardRootDir);
  lastAction = "Displayed SD card directory tree.";
  std::cout << "\nPress [Enter] to return...";
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Menu::selectOutputDir() {
  std::cout << "\nEnter the output directory path:\n";
  std::cin >> outputDir;
  if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid output directory.\n";
    return;
  }
  if (outputDir.empty()) {
    lastAction = "Output directory cannot be empty.";
    return;
  }
  if (!fs::exists(outputDir) || !fs::is_directory(outputDir)) {
    lastAction = "Invalid output directory.";
    outputDir = "";
    return;
  }
  lastAction = "Output directory set to: " + outputDir;
}

void Menu::toggleSound() {
  enableEarrape = !enableEarrape;
  lastAction = enableEarrape ? "Sound set to [LOUD]" : "Sound set to [ON]";
}

int Menu::getUserChoice(int min, int max, const std::string& prompt) {
  int choice = 0;
  while (true) {
    std::cout << prompt;
    std::cin >> choice;
    if (std::cin.fail() || choice < min || choice > max) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid choice.\n";
    } else {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      break;
    }
  }
  return choice;
}

void Menu::printLogo() {
  std::cout << R"(
  ____  ____     ____             _   _____      _                 _            
 / ___||  _ \   / ___|___ _ __ __| | | ____|_  _| |_ _ __ ___  ___| |_ ___ _ __ 
 \___ \| | | | | |   / _ \ '__/ _` | |  _| \ \/ / __| '__/ _ \/ __| __/ _ \ '__|
  ___) | |_| | | |__|  __/ | | (_| | | |___ >  <| |_| | |  __/ (__| ||  __/ |   
 |____/|____/   \____\___|_|  \__,_| |_____/_/\_\\__|_|  \___|\___|\__\___|_|                                                                
)" << '\n';
  std::cout << "==================== SDCerdExtrecter ====================\n" << '\n';
}