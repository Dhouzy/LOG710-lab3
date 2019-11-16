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

/* FUNCTION DECLARATION */

bloc * find_first_allocation_fit(int size);


/* INTERNAL DATA STUCTURE */

// Create a new allocated bloc at the start of the selected bloc
bloc * split_bloc(bloc *original_bloc, int new_bloc_size){

    if(original_bloc->size < new_bloc_size){
	return NULL;
    }

    bloc *new_bloc = (bloc*) calloc(1, sizeof(*new_bloc));
    // Create new bloc
    new_bloc->is_free = 0;
    new_bloc->size = new_bloc_size;
    /* new_bloc->start = original_bloc->start; */
    memcpy(&new_bloc->virtual_start, &original_bloc->virtual_start, sizeof(int));
    new_bloc->previous = original_bloc->previous; 
    new_bloc->next = original_bloc;

    // Change the next pointeur of the previous bloc of the original one
    if (original_bloc->previous != NULL){
	original_bloc->previous->next = new_bloc;
    }

    // Adjust the original bloc
    // FIXME If original_bloc size become 0, delete the bloc
    /* original_bloc.start = new_bloc.start + ? */
    original_bloc->virtual_start = new_bloc->virtual_start + new_bloc->size;
    original_bloc->size = original_bloc->size - new_bloc->size;
    original_bloc->previous = new_bloc;
    
    if (new_bloc->virtual_start == 0){
	FIRST_BLOC = new_bloc;
    }

    return new_bloc;
}

int mergeBloc(bloc * bloc1, bloc * bloc2){
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
    int counter_allocated_bloc = 0;
    bloc *current_bloc = FIRST_BLOC;
    
    while(current_bloc->next != NULL){
	if (current_bloc->is_free == 0){
	    counter_allocated_bloc++;
	}
	current_bloc = current_bloc->next;
    }

    if (current_bloc->is_free == 0){
	counter_allocated_bloc ++;
    }

    return counter_allocated_bloc;
}

int nbloclibres(){
    int counter_free_bloc = 0;
    bloc *current_bloc = FIRST_BLOC;
    
    while(current_bloc->next != NULL){
	if (current_bloc->is_free == 1){
	    counter_free_bloc++;
	}

	current_bloc = current_bloc->next;
    }

    if (current_bloc->is_free == 1){
	counter_free_bloc ++;
    }

    return counter_free_bloc;
}

int memlibre(){
    int counter_free_mem = 0;
    bloc *current_bloc = FIRST_BLOC;
    
    while(current_bloc->next != NULL){
	if (current_bloc->is_free == 1){
	    counter_free_mem += current_bloc->size;
	}
	current_bloc = current_bloc->next;
    }

    if (current_bloc->is_free == 1){
	counter_free_mem += current_bloc->size;
    }

    return counter_free_mem;
}

int mem_pgrand_libre(){
    int biggest_free_bloc = -1;
    bloc *current_bloc = FIRST_BLOC;

    while(current_bloc->next != NULL){
	if (current_bloc->is_free == 1 && current_bloc->size > biggest_free_bloc){
	    biggest_free_bloc = current_bloc->size;
	}
	current_bloc = current_bloc->next;
    }

    if (current_bloc->is_free == 1 && current_bloc->size > biggest_free_bloc){
	biggest_free_bloc = current_bloc->size;
    }
    return biggest_free_bloc;
}

int alloumem(int size){
    bloc * free_bloc = NULL;
    bloc * new_bloc = NULL;

    switch(STRATEGY) {
    case first_fit  :
	free_bloc = find_first_allocation_fit(size);
	if(free_bloc == NULL){
	    return -1;
	}
	new_bloc = split_bloc(free_bloc, size);
	break;
	
    default : 
	printf("ERROR, unknown strategy\n");
	return -1;
    }
    return new_bloc->virtual_start;  // TODO return the memory address
}


/*
  Return the first free bloc that can be split to fit the future bloc 
*/
bloc * find_first_allocation_fit(int size){
    bloc* current_bloc = FIRST_BLOC;

    while (current_bloc->next!= NULL) {
	if (current_bloc->is_free && current_bloc->size >= size){
	    return current_bloc;
	}else {
	    current_bloc = current_bloc->next;
	}
    } 
    if (current_bloc->is_free && current_bloc->size >= size){
	return current_bloc;
    }
    return NULL;
}



