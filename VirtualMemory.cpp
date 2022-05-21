//
// Created by user on 16/05/2022.
//
#include "VirtualMemory.h"
#include "PhysicalMemory.h"


/**  Defines  **/
#define BASE_ADD 0

/** Helper functions **/

/**  Exe functions  **/

uint64_t get_p_address (uint64_t p, uint64_t *address_split);
word_t translate_address (uint64_t p, const uint64_t *address_split);
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
  if (virtualAddress > VIRTUAL_MEMORY_SIZE){
	return 0;
  }
  uint64_t d = virtualAddress << (VIRTUAL_ADDRESS_WIDTH-OFFSET_WIDTH);
  d >> (VIRTUAL_ADDRESS_WIDTH-OFFSET_WIDTH);
  uint64_t p = virtualAddress >> OFFSET_WIDTH;
  uint64_t address_split [TABLES_DEPTH-1];
  p = get_p_address (p, address_split);
  word_t addr1 = translate_address (p, address_split);
  PMread(addr1*PAGESIZE+ d,value);
  return 1;
}

int get_min(int a, int b){
  if(a < b){
	return a;
  }
  return b;
}


int get_abs (uint64_t p, uint64_t page){
  int y =  (int) (page - p);
  if(y<0){
	  y = -y;
	  return y;
	}
}

void DFS(word_t& max_dist_frame, word_t& max_dist_parent, uint64_t&
		max_dist_page, word_t cur_frame, word_t cur_parent, int depth ,
		 word_t* max_frame_index, int* max_dist, word_t* frames, int* break_dfs,
		 uint64_t p, uint64_t path){
  if (*break_dfs){
	return;
  }
  if (depth == TABLES_DEPTH){
	int y = get_abs (p, path);
	int new_min = get_min (NUM_PAGES - y, y);
	if(new_min > *max_dist){
		*max_dist = new_min;
		max_dist_frame = cur_frame;
		max_dist_parent = cur_parent;
		max_dist_page = path;
	}
  }
  int not_zero = 0;
  for (int table_idx = 0; table_idx <= PAGESIZE;table_idx++)
	{
	  word_t value;
	  PMread (cur_frame * PAGESIZE + table_idx, &value);
	  if (value != 0){
		*max_frame_index = (*max_frame_index > value) ? *max_frame_index :
						   value;
		not_zero = 1;
		DFS (max_dist_frame, max_dist_parent, max_dist_page,
			 value, cur_frame * PAGESIZE + table_idx, depth + 1,
			 max_frame_index, max_dist, frames, break_dfs,
			 p, (path << OFFSET_WIDTH) + table_idx);
	  }
	}
	int used_frame = 0;
	if (not_zero==0){
	  for (int i =0;i<TABLES_DEPTH;i++){
		if (frames[i] == cur_frame){
		  used_frame = 1;
		  break;
		}
	  }
	  if (used_frame == 0){
		PMwrite (cur_parent, 0);
		max_dist_frame = cur_frame;
		*break_dfs = 1; // run isn't finished because we found an empty table
	  }
	}
}

word_t find_frame(word_t* frames,uint64_t p, int i)
{
  word_t max_frame_index = 0;
  word_t parent_idx; // removed Frame parent's frame
  int max_dist = -1;
  uint64_t max_dist_page = -1;
  word_t max_dist_frame = -1;
  word_t max_dist_parent=-1;
  int *  break_dfs;
  uint64_t path;
  DFS( max_dist_frame, max_dist_parent, max_dist_page,  0,
	   0, 0 , &max_frame_index, &max_dist, frames,break_dfs, p, path);
  if (break_dfs){ //case 1
  	return max_dist_frame;
  }
  if (max_frame_index+1<NUM_FRAMES){ //case 2
	if(i == TABLES_DEPTH){
	  for (uint64_t j =0; j < PAGE_SIZE;j++){
		  PMwrite((max_frame_index+1)*PAGESIZE + j,0);
	  }
	}
	return (max_frame_index+1);
  }
  else{
	if(i == TABLES_DEPTH){
	  for (uint64_t j =0; j < PAGE_SIZE;j++){
		PMwrite(max_dist_frame*PAGESIZE + j,0);
	  }
	}
	PMwrite (max_dist_parent,0); //remove entry from parent
	PMevict (max_dist_frame,max_dist_page);
	return max_dist_frame;
  }
}
/**
 *
 * @param p
 * @param address_split
 * @return
 */
word_t translate_address (uint64_t p, const uint64_t *address_split)
{
  word_t addr1 = 0;
  word_t addr2 = 0;
  word_t frames [TABLES_DEPTH];
  for (int i =0;i<TABLES_DEPTH;i++){
	PMread (addr1*PAGESIZE+ address_split[i],&addr2);
	if (addr2 == 0){
	  frames[i] = find_frame(frames,p, i);
	  addr1 = frames[i];
	  if (i==(TABLES_DEPTH-1)){
		PMrestore (addr1,p); //TODO verify
	  }
	  PMwrite (addr1*PAGESIZE+ address_split[i],frames[i]);
	  }
	else{
	  addr1 = addr2;
	}
  }
  return addr1;
}

/**
 *
 * @param p
 * @param address_split
 * @return
 */
uint64_t get_p_address (uint64_t p, uint64_t *address_split)
{
  uint64_t mask = (PAGESIZE - 1);
  for (int i = TABLES_DEPTH;i>0;i++){
	address_split[i]= p & mask;
	p = p >> OFFSET_WIDTH;
  }
  return p;
}

/* Writes a word to the given virtual address.
 *
 * returns 1 on success.
 * returns 0 on failure (if the address cannot be mapped to a physical
 * address for any reason)
 */
int VMwrite(uint64_t virtualAddress, word_t value){
  if (virtualAddress > VIRTUAL_MEMORY_SIZE){
	  return 0;
	}
  uint64_t d = virtualAddress << (VIRTUAL_ADDRESS_WIDTH-OFFSET_WIDTH);
  d >> (VIRTUAL_ADDRESS_WIDTH-OFFSET_WIDTH);
  uint64_t p = virtualAddress >> OFFSET_WIDTH;
  uint64_t address_split [TABLES_DEPTH-1];
  p = get_p_address (p, address_split);
  word_t addr1 = translate_address (p, address_split);
  PMwrite(addr1*PAGESIZE+ d,value);
  return 1;

}
