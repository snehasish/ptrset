#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NBINS 16 
#define NSLOT 7

uint32_t 
ptrset_test(uint64_t addr);

uint32_t
ptrset_test_or_insert(uint64_t);

uint32_t
ptrset_erase(uint64_t addr);

void
ptrset_dump();

void
ptrset_clear_safe();

void
ptrset_clear_unsafe();
