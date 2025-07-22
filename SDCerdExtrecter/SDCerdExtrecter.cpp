#include "BeepQueueManager.h"
#include <thread>

int main() {
  BeepQueueManager manager;
  std::thread prod(&BeepQueueManager::produceBeeps, &manager, 250);
  std::thread cons(&BeepQueueManager::consumeBeeps, &manager);
  prod.join();
  cons.join();
  return 0;
}