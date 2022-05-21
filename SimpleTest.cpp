#include "VirtualMemory.h"

#include <cstdio>
#include <cassert>
#include "PhysicalMemory.h"
#include "VirtualMemory.h"

int main(int argc, char **argv) {
//    VMinitialize();
//    for (uint64_t i = 0; i < (2 * NUM_FRAMES); ++i) {
//        printf("writing to %llu\n", (long long int) i);
//        VMwrite(5 * i * PAGE_SIZE, i);
//    }
//
//    for (uint64_t i = 0; i < (2 * NUM_FRAMES); ++i) {
//        word_t value;
//        VMread(5 * i * PAGE_SIZE, &value);
//        printf("reading from %llu %d\n", (long long int) i, value);
//        assert(uint64_t(value) == i);
//    }
//    printf("success\n");
//
//    return 0;
  printf("page size is = %d\n", PAGE_SIZE);
  printf("depth is = %d\n", TABLES_DEPTH);
  uint64_t virtual_address = 8;
  int table[TABLES_DEPTH];
  for (int i = TABLES_DEPTH; i>=1;i--){
	table[i] = virtual_address & (uint64_t) (PAGESIZE-1);
	virtual_address = virtual_address >> OFFSET_WIDTH;
  }

}
