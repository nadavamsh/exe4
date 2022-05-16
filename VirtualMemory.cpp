//
// Created by user on 16/05/2022.
//
#include "VirtualMemory.h"
#include "PhysicalMemory.h"


/**  Defines  **/
#define BASE_ADD 0

/** Helper functions **/

/**  Exe functions  **/

/*
 * Initialize the virtual memory.
 */
void VMinitialize(){
  for (uint64_t i =0; i < PAGE_SIZE;i++){
	  PMwrite(BASE_ADD+i,0);
  }
}

/* Reads a word from the given virtual address
 * and puts its content in *value.
 *
 * returns 1 on success.
 * returns 0 on failure (if the address cannot be mapped to a physical
 * address for any reason)
 */
int VMread(uint64_t virtualAddress, word_t* value){
  uint64_t d = virtualAddress << (VIRTUAL_ADDRESS_WIDTH-OFFSET_WIDTH);
  d >> (VIRTUAL_ADDRESS_WIDTH-OFFSET_WIDTH);
  uint64_t p = virtualAddress >> OFFSET_WIDTH;
  uint64_t bits_per_table [TABLES_DEPTH-1];



}

/* Writes a word to the given virtual address.
 *
 * returns 1 on success.
 * returns 0 on failure (if the address cannot be mapped to a physical
 * address for any reason)
 */
int VMwrite(uint64_t virtualAddress, word_t value){}
