#include <iostream>
#include <cassert>
#include <chrono>
#include <set>
#include <unordered_set>

extern "C" { 
    #include <ptrset.h> 
}

using namespace std;

int main() {
    uint64_t addrs[128] = {0};
    for(int i = 0; i < 128; i++) {
        uint64_t *p = (uint64_t*)malloc((i+1)*200*sizeof(uint64_t));
        addrs[i] = (uint64_t)p;
        free(p);
    }

    cout << "ptrset_clear_safe\n";
    auto start = chrono::steady_clock::now();
    for(int j = 0; j < 100000; j++) {
        for(int i = 0; i < 128; i++) {
            ptrset_test_or_insert(addrs[i]);
        }
        ptrset_clear_safe();
    }
    auto end = chrono::steady_clock::now();
    cout << "time: "<< chrono::duration <double, milli> (end-start).count() << " ms" << endl;

    cout << "ptrset_clear_unsafe\n";
    start = chrono::steady_clock::now();
    for(int j = 0; j < 100000; j++) {
        for(int i = 0; i < 128; i++) {
            ptrset_test_or_insert(addrs[i]);
        }
        ptrset_clear_unsafe();
    }
    end = chrono::steady_clock::now();
    cout << "time: "<< chrono::duration <double, milli> (end-start).count() << " ms" << endl;

    cout << "std::set\n";
    std::set<uint64_t> ptrs;
    start = chrono::steady_clock::now();
    for(int j = 0; j < 100000; j++) {
        for(int i = 0; i < 128; i++) {
            ptrs.insert(addrs[i]);
        }
        ptrs.clear();
    }
    end = chrono::steady_clock::now();
    cout << "time: "<< chrono::duration <double, milli> (end-start).count() << " ms" << endl;

    cout << "std::unordered_set \n";
    std::unordered_set<uint64_t> uptrs;
    start = chrono::steady_clock::now();
    for(int j = 0; j < 100000; j++) {
        for(int i = 0; i < 128; i++) {
            uptrs.insert(addrs[i]);
        }
        uptrs.clear();
    }
    end = chrono::steady_clock::now();
    cout << "time: "<< chrono::duration <double, milli> (end-start).count() << " ms" << endl;

    cout << "std::unordered_set \n";
    auto hash = [](const uint64_t val) -> uint64_t {
        return ((val >> 4) * 2654435761);
    };

    std::unordered_set<uint64_t, decltype(hash)> uptrs_h(16, hash);
    start = chrono::steady_clock::now();
    for(int j = 0; j < 100000; j++) {
        for(int i = 0; i < 128; i++) {
            uptrs_h.insert(addrs[i]);
        }
        uptrs_h.clear();
    }
    end = chrono::steady_clock::now();
    cout << "time: "<< chrono::duration <double, milli> (end-start).count() << " ms" << endl;
}

