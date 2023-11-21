#include <cstdint>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

int num_threads = 1;
uint64_t local_count[8] = {0, 0, 0, 0, 0, 0, 0, 0};
thread threads[8];

int n, m;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1), global_count = 0;

void solve(int index, uint64_t current, int thread_id) {
    if (index == m) {
        if (current == (one << n) - 1) local_count[thread_id]++;
    } else if ((index == 0 && num_threads >= 2) ||
               (index == 1 && num_threads >= 4) ||
               (index == 2 && num_threads >= 8)) { // Check if we can create new threads

        // Create new threads to solve the left child
        int new_thread_id = thread_id + (1 << index);
        threads[new_thread_id] = thread(solve, index + 1, current, new_thread_id);

        // Use the original thread to solve the right child
        solve(index + 1, current | subsets[index], thread_id);
    } else {
        solve(index + 1, current, thread_id);
        solve(index + 1, current | subsets[index], thread_id);
    }
}

int main(int argc, char* argv[]) {
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

    threads[0] = thread(solve, 0, 0, 0);

    for (int i = 0; i < num_threads; i++) {
        threads[i].join();
        global_count += local_count[i];
    }

    cout << global_count << endl;
    return 0;
}