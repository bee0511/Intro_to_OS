// design a k-way set associative cache with LRU replacement policy

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
    vector<vector<int>> counter(blocks / k, vector<int>(k, 0));

    for (int address = 0; address < n; address++) {
        cin >> data;
        int set = address % (blocks / k);
        int block = address % k;
        bool cache_hit = false;
        // If the data is not -1, add the counter in all blocks
        for (int i = 0; i < blocks / k; i++) {
            for (int j = 0; j < k; j++) {
                if (cache[i][j] != -1) {
                    counter[i][j]++;
                }
            }
        }
        // Check if the data is in the cache
        for(int i = 0 ; i < blocks / k ; i++){
            for(int j = 0 ; j < k ; j++){
                if(cache[i][j] == data){
                    cache_hit = true;
                    counter[i][j] = 0;
                    break;
                }
            }
        }


        if (!cache_hit) {
            cache_misses++;
            int least_recently_used = 0;
            // Find the least recently used block in the set
            for (int i = 0; i < k; i++) {
                // If there is a not used block, use it
                if (cache[set][i] == -1) {
                    least_recently_used = i;
                    break;
                }
                if (counter[set][i] > counter[set][least_recently_used]) {
                    least_recently_used = i;
                }
            }
            cache[set][least_recently_used] = data;
            counter[set][least_recently_used] = 0;
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
        cout << endl << "--------------" << endl;
#endif
    }
    cout << "Total Cache Misses:" << cache_misses;

    return 0;
}