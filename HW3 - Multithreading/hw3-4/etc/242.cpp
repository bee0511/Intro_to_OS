#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

int threadCount = 1;
uint64_t local_count[8] = {0, 0, 0, 0, 0, 0, 0, 0};
thread t[8];
int requiredThreadCountAtIndex[3] = {2, 4, 8};

int n, m;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1), global_count = 0;

void solve(int index, uint64_t current, int tid) {
    if (index == m) {
        if (current == (one << n) - 1) local_count[tid]++;
    } else if (index < 3 && requiredThreadCountAtIndex[index] <= threadCount) {
        int newTID = tid + (1 << index);
        t[newTID] = thread(solve, index + 1, current | subsets[index], newTID);
        solve(index + 1, current, tid);
        t[newTID].join();
    } else {
        solve(index + 1, current, tid);
        solve(index + 1, current | subsets[index], tid);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3 || string(argv[1]) != "-t") {
        threadCount = 1;
    } else {
        threadCount = stoi(argv[2]);
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

    t[0] = thread(solve, 0, 0, 0);
    t[0].join();

    for (int i = 0; i < threadCount; i++) {
        global_count += local_count[i];
    }

    cout << global_count << endl;
    return 0;
}