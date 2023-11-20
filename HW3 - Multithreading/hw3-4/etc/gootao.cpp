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

vector<int> local_count;

vector<thread> threads;
int thread_idx = 0;
vector<mutex> mutexes;
mutex m1;

void solve_in_thread(int index, uint64_t current, int idx) {
    if (index == m) {
        if (current == (one << n) - 1) local_count[idx]++;
    } else {
        solve_in_thread(index + 1, current, idx);
        solve_in_thread(index + 1, current | subsets[index], idx);
    }
}

void solve(int index, uint64_t current) {
    if (index == m) {
        if (current == (one << n) - 1) {
            global_count++;
        }
    } else if (int(pow(2, index)) == num_threads) {
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
    local_count = vector<int>(num_threads, 0);
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
