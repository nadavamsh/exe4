void DFS (word_t* found_frame, word_t* frames, word_t* max_frame_index
	,int* min_distance, uint64_t* min_page,int cur_level,word_t
		  cur_frame,word_t parent_idx, word_t* min_page_frame, word_t *
min_page_parent_frame,int * break_DFS,uint64_t p){
  if (*break_DFS) {
	  return;
	}
  if (cur_level == TABLES_DEPTH){


	  return;
	}
  int not_zero = 0;
  for (int table_idx = 0; table_idx <= PAGESIZE;table_idx++){
	  word_t value;
	  PMread (cur_frame*PAGESIZE+table_idx,&value);
	  if (value != 0){
		  *max_frame_index = (*max_frame_index > value) ? *max_frame_index :
							 value;
		  not_zero = 1;
		  DFS (found_frame, frames, max_frame_index,min_distance,min_page,
			   cur_level+1 ,value, cur_frame*PAGESIZE+table_idx,
			   min_page_frame,min_page_parent_frame, break_DFS, p);
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
			  PMwrite (parent_idx,0);
			  *found_frame = cur_frame;
			  *break_DFS = 1; // run isn't finished because we found an empty table
			}
		}
	  return;
//
// Created by user on 20/05/2022.
//

