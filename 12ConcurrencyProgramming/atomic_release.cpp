#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
using std::memory_order_relaxed;

std::atomic<bool> is_ready;
std::atomic<int> data[3];

void producer() {
  data[0].store(1, memory_order_relaxed);
  data[1].store(2, memory_order_relaxed);
  data[2].store(3, memory_order_relaxed);
  is_ready.store(true, std::memory_order_release);
}

void consumer() {
  // data 가 준비될 때 까지 기다린다.
  while (!is_ready.load(std::memory_order_acquire)) {
  }

  std::cout << "data[0] : " << data[0].load(memory_order_relaxed) << std::endl;
  std::cout << "data[1] : " << data[1].load(memory_order_relaxed) << std::endl;
  std::cout << "data[2] : " << data[2].load(memory_order_relaxed) << std::endl;
}

int main() {
  std::vector<std::thread> threads;

  threads.push_back(std::thread(producer));
  threads.push_back(std::thread(consumer));

  for (int i = 0; i < 2; i++) {
    threads[i].join();
  }
}