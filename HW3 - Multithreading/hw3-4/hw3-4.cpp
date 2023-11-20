#include <math.h>

#include <cstdint>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
using namespace std;

int n, m, t;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1), global_count = 0;

vector<int> counts;

vector<thread> threads;
int thread_idx = 0;
vector<mutex> mutexes;
mutex m1;

void solve_in_thread(int index, uint64_t current, int idx) {
    if (index == m) {
        if (current == (one << n) - 1) {
            // mutexes[idx].lock();
            counts[idx] += 1;
            // mutexes[idx].unlock();
        }
    } else {
        solve_in_thread(index + 1, current, idx);
        solve_in_thread(index + 1, current | subsets[index], idx);
    }
    // queue<pair<int, uint64_t> > q;
    // q.push({index, current});
    // while(!q.empty())
    //{
    //     auto p = q.front();
    //     q.pop();
    //     if(p.first == m)
    //     {
    //         if (p.second == (one << n) - 1){
    //             mutexes[idx].lock();
    //             counts[idx]+=1;
    //             mutexes[idx].unlock();
    //         }
    //     } else{
    //         q.push({p.first+1, p.second});
    //         q.push({p.first+1, p.second | subsets[p.first]});
    //     }
    // }
}

void solve(int index, uint64_t current) {
    if (index == m) {
        if (current == (one << n) - 1) {
            m1.lock();
            global_count++;
            m1.unlock();
        }
    } else if (int(pow(2, index)) == t) {
        threads[thread_idx] = thread(solve_in_thread, index, current, thread_idx);
        // threads[thread_idx].join();
        thread_idx += 1;
        // if(thread_idx == t)
        //{
        //     for(int i=0;i<t;i++) threads[i].join();
        // }
        return;
    } else {
        solve(index + 1, current);
        solve(index + 1, current | subsets[index]);
    }
    return;
}

int main(int argc, char* argv[]) {
    cin >> n >> m;

    t = 1;
    std::vector<std::string> arguments(argv, argv + argc);
    for (int i = 0; i < argc; i++) {
        if (i != argc - 1 && arguments[i] == "-t") {
            t = arguments[i + 1][0] - '0';
        }
    }
    threads.resize(t);
    counts = vector<int>(t, 0);
    subsets.resize(m);
    mutexes = vector<mutex>(t);
    for (int i = 0; i < m; i++) {
        int p, temp;
        cin >> p;
        for (int j = 0; j < p; j++) {
            cin >> temp;
            subsets[i] |= (one << temp);
        }
    }

    solve(0, 0);

    for (int i = 0; i < t; i++) threads[i].join();

    int ans = 0;
    for (int i = 0; i < t; i++) ans += counts[i];
    cout << ans << endl;
    return 0;
}
