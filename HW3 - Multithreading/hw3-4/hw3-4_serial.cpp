#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <atomic>
#include <cstring>

using namespace std;

int n, m;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1);
atomic<uint64_t> global_count(0);

void solve(int index, uint64_t current) {
    try {
        if (index == m) {
            if (current == (one << n) - 1) global_count++;
        } else {
            solve(index + 1, current);
            solve(index + 1, current | subsets[index]);
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}


void parallel_solve(int index, uint64_t current, int num_threads) {
    if (index == m) {
        if (current == (one << n) - 1) global_count++;
    } else {
        // Check if we have more work to do
        if (index < m - num_threads + 1) {
            thread t(parallel_solve, index + 1, current, num_threads);
            parallel_solve(index + 1, current | subsets[index], num_threads);
            t.join();
        } else {
            solve(index, current);
        }
    }
}

int main(int argc, char* argv[]) {
    int num_threads = 1; // Default number of threads

    // Parse command line arguments to get the number of threads
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

    parallel_solve(0, 0, num_threads);

    cout << global_count << endl;
    return 0;
}
