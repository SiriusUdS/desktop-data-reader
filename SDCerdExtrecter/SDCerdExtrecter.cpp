#include <iostream>
#include <string>
#include "menu/Menu.h"

int main() {
  Menu menu;
  Menu::printLogo();
  std::string sdCardRootDir;
  std::cout << "Enter SD card root directory (e.g., E:/): ";
  std::getline(std::cin, sdCardRootDir);
  namespace fs = std::filesystem;
  if (!fs::exists(sdCardRootDir) || !fs::is_directory(sdCardRootDir)) {
    std::cerr << "Invalid SD card root directory.\n";
    return 1;
  }
	menu.sdCardRootDir = sdCardRootDir;
  menu.run();
  return 0;
}