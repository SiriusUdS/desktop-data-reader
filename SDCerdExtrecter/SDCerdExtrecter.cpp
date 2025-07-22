#include <iostream>
#include <thread>

#include "BeepQueueManager.h"
#include "SDCardReader.h"

int main() {
  const std::string filename = "E:/4/Data.bin";
  SDCardReader reader;
  if (!reader.readFromFile(filename)) {
    return 1;
  }
  std::cout << "Read " << reader.getBytesRead() << " bytes from file." << '\n';



  BeepQueueManager manager;
  std::thread prod(&BeepQueueManager::produceBeeps, &manager, 250);
  std::thread cons(&BeepQueueManager::consumeBeeps, &manager);
  prod.join();
  cons.join();

  return 0;
}
