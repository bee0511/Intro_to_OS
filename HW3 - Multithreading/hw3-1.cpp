#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>

using namespace std;

sem_t semaphore;
int counter = 0;

void count(int index) {
  static mutex io_mutex;
  int num = 1000000;
  while(counter != index);
  while (num--) {}
  {
    lock_guard<mutex> lock(io_mutex);
    cout << "I'm thread " << index << ", local count: 1000000\n";
    counter++;
  }
}

int main(void) {
  thread t[100];

  for (int i = 0; i < 100; i++)
    t[i] = thread(count, i);

  for (int i = 0; i < 100; i++)
    t[i].join();
}
