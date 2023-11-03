#include <iostream>
#include <cstring>
#include <thread>
#include <vector>

using namespace std;

int n, global_count = 0;

bool is_prime(int num) {
    if (num == 1) return false;

    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

void count_primes(int start, int end) {
    int local_count = 0;
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) local_count++;
    }
    global_count += local_count;
}

int main(int argc, char* argv[]) {
    int num_threads = 1; // Default number of threads

    // Parse command line arguments to get the number of threads
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            num_threads = atoi(argv[i + 1]);
        }
    }

    cin >> n;

    // Divide the work among threads
    vector<thread> threads;
    int chunk_size = n / num_threads;
    for (int i = 0; i < num_threads; i++) {
        int start = i * chunk_size + 1;
        int end = (i == num_threads - 1) ? n : (i + 1) * chunk_size;
        threads.emplace_back(count_primes, start, end);
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    cout << global_count << endl;
    return 0;
}
