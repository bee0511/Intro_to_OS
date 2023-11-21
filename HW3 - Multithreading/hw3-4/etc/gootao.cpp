#include <math.h>

#include <atomic>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
using namespace std;

int n, m;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1);
atomic<int> global_count{0};

int num_threads = 1;

uint64_t local_count[8] = {0, 0, 0, 0, 0, 0, 0, 0};
vector<thread> threads;
int thread_idx = 0;
vector<mutex> mutexes;
mutex m1;

void solve_in_thread(int index, uint64_t current, int tid) {
    if (index == m) {
        if (current == (one << n) - 1) local_count[tid]++;
    } else {
        solve_in_thread(index + 1, current, tid);
        solve_in_thread(index + 1, current | subsets[index], tid);
    }
}

void solve(int index, uint64_t current) {
    if (index == m) {
        if (current == (one << n) - 1) global_count++;
    } else if ((index == 0 && num_threads == 1) ||
               (index == 1 && num_threads == 2) ||
               (index == 2 && num_threads == 4) ||
               (index == 3 && num_threads == 8)) {
        threads[thread_idx] = thread(solve_in_thread, index, current, thread_idx);
        thread_idx += 1;
        return;
    } else {
        solve(index + 1, current);
        solve(index + 1, current | subsets[index]);
    }
    return;
}

int main(int argc, char* argv[]) {
    cin >> n >> m;

    // Parse command line arguments to get the number of threads
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            num_threads = atoi(argv[i + 1]);
        }
    }

    threads.resize(num_threads);
    subsets.resize(m);

    for (int i = 0; i < m; i++) {
        int p, temp;
        cin >> p;
        for (int j = 0; j < p; j++) {
            cin >> temp;
            subsets[i] |= (one << temp);
        }
    }

    solve(0, 0);

    for (int i = 0; i < num_threads; i++) {
        threads[i].join();
        global_count += local_count[i];
    }

    cout << global_count << endl;
    return 0;
}
