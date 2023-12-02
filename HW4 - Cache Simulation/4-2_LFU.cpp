// design a k-way set associative cache with LFU replacement policy

#include <iostream>
#include <vector>

using namespace std;

// #define DEBUG 1

int main() {
    int blocks;  // Number of cache lines/ blocks
    int k;       // Number of cache lines/ blocks of a set
    int n;       // Reference data stream size
    int data;    // Reference data
    int cache_misses = 0;
    cin >> blocks >> k >> n;
    vector<vector<int>> cache(blocks / k, vector<int>(k, -1));
    vector<vector<int>> used_time(blocks / k, vector<int>(k, 0));

    for (int address = 0; address < n; address++) {
        cin >> data;
        int set = address % (blocks / k);
        int block = address % k;
        bool cache_hit = false;

        // Check if the data is in the cache
        for (int i = 0; i < blocks / k; i++) {
            for (int j = 0; j < k; j++) {
                if (cache[i][j] == data) {
                    // If the data is in the cache, update the used time
                    cache_hit = true;
                    used_time[i][j]++;
#ifdef DEBUG
                    cout << "[O] Cache Hit!" << endl;
#endif
                    break;
                }
            }
        }

        // If the data is not in the cache, find the least frequently used block
        if (!cache_hit) {
            cache_misses++;
            int least_frequently_used = 0;
            // Find the least frequently used block in the set
            for (int i = 0; i < k; i++) {
                // If there is a not used block, use it
                if (cache[set][i] == -1) {
                    least_frequently_used = i;
                    break;
                }
                if (used_time[set][i] < used_time[set][least_frequently_used]) {
                    least_frequently_used = i;
                }
            }
            // Replace the least frequently used block with the new data
            cache[set][least_frequently_used] = data;
            used_time[set][least_frequently_used] = 1;
#ifdef DEBUG
            cout << "[X] Cache Miss!" << endl;
#endif
        }
#ifdef DEBUG
        cout << "Address: " << address << endl;
        cout << "Data: " << data << endl;
        cout << "Set: " << set << endl;
        cout << "Block: " << block << endl;
        cout << "Cache: ";
        for (int i = 0; i < blocks / k; i++) {
            for (int j = 0; j < k; j++) {
                cout << cache[i][j] << " ";
            }
        }
        cout << endl;
        cout << "Counter: ";
        for (int i = 0; i < blocks / k; i++) {
            for (int j = 0; j < k; j++) {
                cout << counter[i][j] << " ";
            }
        }
        cout << endl
             << "--------------" << endl;
#endif
    }
    cout << "Total Cache Misses:" << cache_misses;
    return 0;
}