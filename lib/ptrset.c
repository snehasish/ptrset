#include <ptrset.h>

static uint64_t ptr_bins[NBINS][NSLOT+1] = {0};

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

static inline uint64_t
tombstone() {
    return mask(0);
}

static void 
clear_alloc(uint64_t* bin) {
    if(bin[NSLOT] != 0) {
        clear_alloc(INT_TO_PTR(bin[NSLOT]));
        free(INT_TO_PTR(bin[NSLOT]));
#ifndef NDEBUG
        printf("FREE: %p\n", (void *)bin[NSLOT]);
#endif
    }
}

static inline uint32_t 
test_helper(uint64_t** pbin, int *idx, uint64_t addr) {
    uint64_t *bin = *pbin;
    while(bin[*idx] != 0) {
        if(bin[*idx] == mask(addr)) return 0;
        if(*idx == NSLOT) {
            bin = INT_TO_PTR(bin[*idx]);
            *idx = 0;
        } else {
            (*idx)++;
        }
    } 
    *pbin = bin;
    return 1;
}

/* Exported Functions */

uint32_t 
ptrset_test(uint64_t addr) {
    uint64_t *bin = ptr_bins[hash(addr)];
    int idx = 0;
    if(test_helper(&bin, &idx, addr) == 0)
        return 0;
    return 1;
}

uint32_t
ptrset_test_or_insert(uint64_t addr) {
    uint64_t *bin = ptr_bins[hash(addr)];
    int idx = 0;

    if(test_helper(&bin, &idx, addr) == 0)
        return 0;

    if(idx != NSLOT) {
        /* fast path insert */
        bin[idx] = mask(addr);
        return 1;
    } else {
        /* slow path calloc */
        bin[idx] = PTR_TO_INT(calloc(NSLOT + 1, sizeof(uint64_t)));
        *(INT_TO_PTR(bin[idx])) = mask(addr);
#ifndef NDEBUG
        printf("ALLOC: %p\n", (void *)bin[idx]);
#endif
        return 2;
    }
}

uint32_t
ptrset_erase(uint64_t addr) {
    uint64_t *bin = ptr_bins[hash(addr)];
    int idx = 0;
    if(test_helper(&bin, &idx, addr) == 0) {
        bin[idx] = tombstone();
        return 0;
    }
    return 1;
}

void
ptrset_dump() {
#ifndef NDEBUG
    for(int i = 0; i < NBINS; i++) {
        uint64_t *bin = ptr_bins[i];
        printf("[%d] ", i);
        int j = 0;
        while(bin[j] != 0) {
            if(j == NSLOT) {
                bin = INT_TO_PTR(bin[j]);
                printf("-> ");
                j = 0;
            } else {
                printf("%p ", (void*)unmask(bin[j]));
                j++;
            }
        }
        printf("\n");
    }
#endif
}


void
ptrset_clear_safe() {
    for(int i = 0; i < NBINS; i++) {
        uint64_t *bin = ptr_bins[i];
        if(bin[NSLOT] != 0) {
            clear_alloc(INT_TO_PTR(bin[NSLOT]));
            free(INT_TO_PTR(bin[NSLOT]));
#ifndef NDEBUG
            printf("FREE: %p\n", (void *)bin[NSLOT]);
#endif
        }
    }
    ptrset_clear_unsafe();
}

void
ptrset_clear_unsafe() {
    memset(ptr_bins, 0, sizeof(uint64_t) * NBINS * (NSLOT + 1));
}

