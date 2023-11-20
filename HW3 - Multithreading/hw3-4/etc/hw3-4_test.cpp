#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

using namespace std;

mutex mut;
int n, m;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1), global_count = 0;
vector<thread> threads;
int thread_count = 1;
int local_counts[8] = {0,0,0,0,0,0,0,0};

void solve(int index, uint64_t current, int thread_index) {
    if (index == m) {
        if (current == (one << n) - 1) local_counts[thread_index]++;
    } 
    else {
            solve(index + 1, current, thread_index);
            solve(index + 1, current | subsets[index], thread_index);
    }
}

int main(int argc, char* argv[]) {
    if (argc > 2 && string(argv[1]) == "-t"){
        thread_count = stoi(argv[2]);
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
    if (thread_count>=8 && m>=3) {
        threads.emplace_back(thread(solve, 3, 0, 0));
        threads.emplace_back(thread(solve, 3, subsets[0], 1));
        threads.emplace_back(thread(solve, 3, subsets[1], 2));
        threads.emplace_back(thread(solve, 3, subsets[2], 3));
        threads.emplace_back(thread(solve, 3, subsets[0]|subsets[1], 4));
        threads.emplace_back(thread(solve, 3, subsets[0]|subsets[2], 5));
        threads.emplace_back(thread(solve, 3, subsets[1]|subsets[2], 6));
        threads.emplace_back(thread(solve, 3, subsets[0]|subsets[1]|subsets[2], 7));
    }
    else if (thread_count>=4 && m>=2) {
        threads.emplace_back(thread(solve, 2, 0, 0));
        threads.emplace_back(thread(solve, 2, subsets[0], 1));
        threads.emplace_back(thread(solve, 2, subsets[1], 2));
        threads.emplace_back(thread(solve, 2, subsets[0]|subsets[1], 3));
    }
    else if (thread_count>=2 && m>=1) {
        threads.emplace_back(thread(solve, 1, 0, 0));
        threads.emplace_back(thread(solve, 1, subsets[0], 1));
    }
    else{
        solve(0, 0, 0);
    } 

    for (auto &t : threads)
        t.join();

    for(int i=0; i<7; i++)
        global_count += local_counts[i];

    cout << global_count << endl;
    return 0;
}
