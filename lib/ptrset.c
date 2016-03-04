#include <ptrset.h>

uint64_t ptr_bins[NBINS][NSLOT+1] = {0};

/* Macros */

#define PTR_TO_INT(x) ((uint64_t)x) 
#define INT_TO_PTR(x) ((uint64_t*)x) 
#define SET_BIT(n,x) (n |= 1ULL << x)
#define UNSET_BIT(n,x) (n &= ~(1ULL << x))

/* Helper Functions */
static inline uint64_t
hash(uint64_t val) {
    return ((val >> 4) * 2654435761) & (NBINS - 1);
}

static inline uint64_t
mask(uint64_t val) {
    return SET_BIT(val, 63);
}

static inline uint64_t
unmask(uint64_t val) {
    return UNSET_BIT(val, 63);
}

/* Exported Functions */

uint32_t 
ptrset_create() {
    return 0;    
}

uint32_t
ptrset_test_or_insert(uint64_t addr) {
    uint64_t *bin = ptr_bins[hash(addr)];
    int idx = 0;
    while(bin[idx] != 0) {
        if(bin[idx] == mask(addr)) return 0;
        if(idx == NSLOT) {
            bin = INT_TO_PTR(bin[idx]);
            idx = 0;
        } else {
            idx++;
        }
    } 
    if(idx != NSLOT) {
        /* fast path insert */
        bin[idx] = mask(addr);
        return 1;
    } else {
        /* slow path calloc */
        bin[idx] = PTR_TO_INT(calloc(NSLOT + 1, sizeof(uint64_t)));
        *(INT_TO_PTR(bin[idx])) = mask(addr);
        return 2;
    }
}

void
ptrset_dump() {
#ifndef NDEBUG
    /* FIXME : Off by one? */
    for(int i = 0; i < NBINS; i++) {
        uint64_t *bin = ptr_bins[i];
        for(int j = 0; j < NSLOT; j++) {
            printf("%lu\t", unmask(bin[j]));
            if(j+1 == NSLOT && bin[j+1] != 0) {
                bin = INT_TO_PTR(bin[j+1]);
                j = 0;
                printf("->");
            }
        }
        printf("\n");
    }
#endif
}

uint32_t
ptrset_destroy() {
    /* TODO */
    return 0;
}


