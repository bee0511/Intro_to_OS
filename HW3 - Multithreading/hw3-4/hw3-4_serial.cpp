#include <iostream>
#include <cstdint>
#include <vector>

using namespace std;

int n, m;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1), global_count = 0;
uint64_t total_calculation = 0;

void solve(int index, uint64_t current) {
    // total_calculation++;

    if (index == m) {
        if (current == (one << n) - 1) global_count++;
    } else {
        solve(index + 1, current);
        solve(index + 1, current | subsets[index]);
    }
}

int main() {
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

    solve(0, 0);

    cout << global_count << endl;
    // cout << "Total calculation: " << total_calculation << endl;
    return 0;
}