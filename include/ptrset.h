#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define NBINS 16 
#define NSLOT 7

uint32_t 
ptrset_create();

uint32_t
ptrset_test_or_insert(uint64_t);

void
ptrset_dump();

uint32_t
ptrset_destroy();
