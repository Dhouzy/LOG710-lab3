#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Compile library: 
  gcc -o memory.o -c memory.c
*/

typedef struct Bloc {
    int start;
    int virtual_start;
    int size;
    int is_free; // 1 free, 0 occupied 
    struct Bloc *next; 
    struct Bloc *previous;

}bloc;


enum strategy STRATEGY = -1;
int MEMORY_SIZE = -1;

bloc *FIRST_BLOC = NULL; 

/* INTERNAL DATA STUCTURE */

// Create a new allocated bloc at the start of the current bloc
int splitBloc(struct Bloc *original_bloc, int new_bloc_size){
    bloc *new_bloc = (bloc*) calloc(1, sizeof(*new_bloc));


    new_bloc->is_free = 0;
    new_bloc->size = new_bloc_size;
    /* new_bloc->start = original_bloc->start; */
    memcpy(&new_bloc->virtual_start, &original_bloc->virtual_start, sizeof(int));
    new_bloc->previous = original_bloc->previous; 
    new_bloc->next = original_bloc;

    /* original_bloc.start = new_bloc.start + ? */
    original_bloc->virtual_start = new_bloc->virtual_start + new_bloc->size;
    original_bloc->size = original_bloc->size - new_bloc->size;
    original_bloc->previous = new_bloc;

    if (new_bloc->virtual_start == 0){
    	FIRST_BLOC = new_bloc;
    }

    return new_bloc->virtual_start; 
}

int mergeBloc(struct Bloc bloc1, struct Bloc bloc2){
    //Verify both bloc are adjacent.
    return -1;
}


/* PUBLIC FUNCTION */

int * initmem(int size, enum strategy strategy) {
    int *ptr =  (int*) malloc(size);

    FIRST_BLOC = (bloc*) calloc(1, sizeof(*FIRST_BLOC));

    STRATEGY = strategy;
    MEMORY_SIZE = size;

    // initialise first bloc
    FIRST_BLOC->is_free = 1;
    FIRST_BLOC->next = NULL;
    FIRST_BLOC->previous = NULL;
    /* FIRST_BLOC->start = (int) ptr; */ // TODO adjust start when spliting & merging bloc
    FIRST_BLOC->virtual_start = 0;
    FIRST_BLOC->size = size;

    return ptr;
}

int nblocalloues(){
    int counter_allocated_block = 0;
    bloc *current_bloc = FIRST_BLOC;
    
    while(current_bloc->next != NULL){

	if (current_bloc->is_free == 0){
    	    counter_allocated_block++;
    	}
    	current_bloc = current_bloc->next;
    }

    if (current_bloc->is_free == 0){
    	counter_allocated_block ++;
    }

    return counter_allocated_block;
}

int alloumem(int size){
    //TODO find the correct location 
    /* return splitBloc(FIRST_BLOC, size); */
}


