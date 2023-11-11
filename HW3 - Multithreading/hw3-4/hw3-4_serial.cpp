#include <atomic>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

int n, m;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1);
atomic<uint64_t> global_count(0);
mutex count_mutex;

void solve(int start, int end) {
    // Local count to store the number of valid subsets for the current thread
    uint64_t local_count = 0;

    // Iterate through the range of subsets assigned to this thread
    for (int i = start; i < end; i++) {
        // Initialize a variable to represent the bitwise OR of selected elements in the subset
        uint64_t current = 0;

        // Iterate through the elements in the subset
        for (int j = 0; j < m; j++) {
            // Check if the j-th bit is set in the binary representation of i
            if ((i & (1 << j)) != 0) {
                // If set, perform bitwise OR with the corresponding element in subsets
                current |= subsets[j];
            }
        }

        // Check if the bitwise OR of the current subset equals the target value
        if (current == (one << n) - 1) {
            // If yes, increment the local count
            local_count++;
        }
    }

    // Use a lock_guard to safely update the global count across multiple threads
    lock_guard<mutex> lock(count_mutex);
    global_count += local_count;
}

int main(int argc, char *argv[]) {
    int num_threads = 1;  // Default number of threads

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

    for (auto &t : threads) {
        t.join();
    }

    cout << global_count << endl;
    return 0;
}
