#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <cstring>

using namespace std;

int n, m;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1);
atomic<uint64_t> global_count(0);
mutex count_mutex;

void solve(int start, int end) {
    uint64_t local_count = 0;

    for (int i = start; i < end; i++) {
        uint64_t current = 0;

        for (int j = 0; j < m; j++) {
            if ((i & (1 << j)) != 0) {
                current |= subsets[j];
            }
        }

        if (current == (one << n) - 1) {
            local_count++;
        }
    }

    lock_guard<mutex> lock(count_mutex);
    global_count += local_count;
}

int main(int argc, char* argv[]) {
    int num_threads = 1; // Default number of threads

    // Parse command-line arguments to get the number of threads
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            num_threads = atoi(argv[i + 1]);
        }
    }

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

    vector<thread> threads;
    int chunk_size = (1 << m) / num_threads;
    int start = 0;

    for (int i = 0; i < num_threads; i++) {
        int end = (i == num_threads - 1) ? (1 << m) : start + chunk_size;
        threads.emplace_back(solve, start, end);
        start = end;
    }

    for (auto& t : threads) {
        t.join();
    }

    cout << global_count << endl;
    return 0;
}
