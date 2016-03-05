#include <iostream>
#include <cassert>

extern "C" { 
    #include <ptrset.h> 
}

using namespace std;

int main() {
    for(int i = 0; i < 128; i++) {
        uint64_t *p = (uint64_t*)malloc((i+1)*200*sizeof(uint64_t));
        assert(ptrset_test_or_insert((uint64_t)p) != 0 && "reused addr");
        free(p);
    }
    //ptrset_dump();
    ptrset_clear_safe();
    //ptrset_dump();
}

