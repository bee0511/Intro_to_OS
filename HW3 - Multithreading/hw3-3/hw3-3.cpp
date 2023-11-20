#include <atomic>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

int n;
int num_threads = 1;
atomic<int> global_count{0};

inline bool is_prime(int num) {
    if (num == 1) return false;

    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

void count_primes(int thread_index) {
    int local_count = 0;

    bool flip_flag = false;
    for (int i = thread_index; i <= n; i += num_threads) {
        // if num_thread = 4 -> distribute 1, 2, 3, 4 to thread 1, 2, 3, 4
        if (!flip_flag && is_prime(i))
            local_count++;
        // if num_thread = 4 -> distribute 4, 3, 2, 1 to thread 1, 2, 3, 4
        else if (flip_flag && is_prime(i + (num_threads + 1) - 2 * thread_index))
            local_count++;

        flip_flag = !flip_flag;
    }

    global_count += local_count;
}

int main(int argc, char* argv[]) {
    // Parse command line arguments to get the number of threads
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            num_threads = atoi(argv[i + 1]);
        }
    }

    cin >> n;

    // Divide the work among threads
    vector<thread> threads;

    for (int i = 1; i <= num_threads; i++) {
        threads.emplace_back(thread(count_primes, i));
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    cout << global_count << endl;

    return 0;
}
