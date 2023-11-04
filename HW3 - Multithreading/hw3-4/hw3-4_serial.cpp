#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <cstring>
#include <functional>
#include <queue>

using namespace std;

int n, m;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1), global_count = 0;
mutex global_count_mutex;
mutex queue_mutex;
queue<function<void()>> task_queue;

void worker_thread(int num_threads) {
    while (true) {
        function<void()> task;
        {
            lock_guard<mutex> lock(queue_mutex);
            if (task_queue.empty()) {
                break; // No more tasks, exit the thread
            }
            task = task_queue.front();
            task_queue.pop();
        }
        task();
    }
}

void solve(int index, uint64_t current, int num_threads) {
    if (index == m) {
        if (current == (one << n) - 1) {
            lock_guard<mutex> lock(global_count_mutex);
            global_count++;
        }
    } else {
        if (num_threads > 1) {
            // Add tasks to the queue
            {
                lock_guard<mutex> lock(queue_mutex);
                task_queue.push([=]() { solve(index + 1, current, num_threads - 1); });
                task_queue.push([=]() { solve(index + 1, current | subsets[index], num_threads - 1); });
            }
        } else {
            solve(index + 1, current, 1);
            solve(index + 1, current | subsets[index], 1);
        }
    }
}

int main(int argc, char *argv[]) {
    cin >> n >> m;

    subsets.resize(m);
    for (int i = 0; i < m; i++) {
        int p, temp;
        cin >> p;
        for (int j = 0; j < p; j++) {
            cin >> temp;
            subsets[i] |= (one << temp);
        }
    }

    int num_threads = 1;
    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }

    vector<thread> threads;
    for (int i = 0; i < num_threads; i++) {
        threads.push_back(thread(worker_thread, num_threads));
    }

    thread t([=]() { solve(0, 0, num_threads); });

    t.join();

    // Add a sentinel task for each thread to exit
    for (int i = 0; i < num_threads; i++) {
        task_queue.push([]() {});
    }

    for (auto& thread : threads) {
        thread.join();
    }

    cout << global_count << endl;
    return 0;
}
