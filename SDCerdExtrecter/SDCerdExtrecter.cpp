#include <iostream>
#include <thread>

#include "BeepQueueManager.h"
#include "SDCardReader.h"

int main() {
  const std::string filename = "E:/4/Data.bin";
  SDCardReader reader(filename);
  auto buffer = reader.readNext();

  BeepQueueManager manager;
  std::thread prod(&BeepQueueManager::produceBeeps, &manager, 250);
  std::thread cons(&BeepQueueManager::consumeBeeps, &manager);
  prod.join();
  cons.join();

  return 0;
}
