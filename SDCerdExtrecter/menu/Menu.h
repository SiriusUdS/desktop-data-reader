#pragma once
#include <string>
#include <filesystem>

class Menu {
public:
  Menu() = default;
  void run();
  static void printLogo();

private:
  void displayMenu() const;
  void handleMenuChoice(int choice);
  void selectFile();
  void showDirectoryTree();
  void selectOutputDir();
  static int getUserChoice(int min, int max, const std::string& prompt);

public:
  std::string sdCardRootDir;

private:
  std::string selectedFile;
  std::string outputDir;
  std::string lastAction;
  bool exitRequested = false;
};
