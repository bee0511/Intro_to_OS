#include <iostream>
#include <vector>

using namespace std;

int main() {
    int blocks, n;
    cin >> blocks >> n;
    int cache_misses = 0;
    vector<int> cache(blocks, -1);
    for (int address = 0; address < n; address++) {
        int data;
        cin >> data;
        int block = address % blocks;
        bool cache_hit = false;
        for(int i = 0; i < blocks; i++) {
            if(cache[i] == data) {
                cache_hit = true;
                break;
            }
        }
        if(!cache_hit) {
            cache_misses++;
            cache[block] = data;
        }
    }
    cout << "Total Cache Misses:" << cache_misses;
    return 0;
}