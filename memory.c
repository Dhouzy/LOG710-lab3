#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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
bloc* LAST_BLOC_FIT = NULL;

/* FUNCTION DECLARATION */

bloc * find_first_allocation_fit(int size);
bloc * find_best_allocation_fit(int size);
bloc * find_worst_allocation_fit(int size);
bloc * find_next_allocation_fit(int size);


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
    new_bloc->start = original_bloc->start;
    memcpy(&new_bloc->virtual_start, &original_bloc->virtual_start, sizeof(int));
    new_bloc->previous = original_bloc->previous; 
    new_bloc->next = original_bloc;

    // Change the next pointeur of the previous bloc of the original one
    if (original_bloc->previous != NULL){
	original_bloc->previous->next = new_bloc;
    }

    // Remove the original bloc if the new one thake the whole size
    if ((original_bloc->size - new_bloc->size) == 0 ){
	new_bloc->next = original_bloc->next;
	if(original_bloc->next != NULL){
	    original_bloc->next->previous = new_bloc;
	}

    }else{
	// Adjust the original bloc
	original_bloc->start = new_bloc->start + new_bloc->size;
	original_bloc->virtual_start = new_bloc->virtual_start + new_bloc->size;
	original_bloc->size = original_bloc->size - new_bloc->size;
	original_bloc->previous = new_bloc;
    }
    
    if (new_bloc->virtual_start == 0){
	FIRST_BLOC = new_bloc;
    }

    return new_bloc;
}

int merge_bloc(bloc * bloc1, bloc * bloc2){
    //  bloc1 should be before bloc 2 (address wise)
    bloc* second_bloc;
    bloc* first_bloc;
    if(bloc1->start < bloc2->start){
	first_bloc = bloc1;
	second_bloc = bloc2;
    } else {
	first_bloc = bloc2;
	second_bloc = bloc1;
    }

    //Verify both bloc are adjacent.
    if (first_bloc->start + first_bloc->size != second_bloc->start) {
	return -1;
    }

    // Merge
    first_bloc->size += second_bloc->size;
    first_bloc->next = second_bloc->next;
    free(second_bloc);

    return 1;
}

bloc* search_bloc_by_address(int pBloc){
    bloc *current_bloc = FIRST_BLOC;

    while(current_bloc->next != NULL){
	if (current_bloc->start == pBloc){
	    return current_bloc;
	}
	current_bloc = current_bloc->next;
    }

    if (current_bloc->start == pBloc){
	return current_bloc;
    }

    return NULL;
}

int free_bloc(bloc* bloc_to_free){
    bloc_to_free->is_free = 1;

    // Check if next bloc is free if so merge it
    if(bloc_to_free->next != NULL && bloc_to_free->next->is_free == 1){
	printf("merging bloc to free with next bloc \n");
	merge_bloc(bloc_to_free, bloc_to_free->next);
    }

    // Check if previous bloc is free if so merge it
    if(bloc_to_free->previous != NULL && bloc_to_free->previous->is_free == 1){
	printf("merging bloc to free with previous bloc \n");
	merge_bloc(bloc_to_free, bloc_to_free->previous);
    }
    
    return 1;
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
    FIRST_BLOC->start = (intptr_t) ptr;
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

int mem_small_free(int maxTaillePetit){
    int counter_small_bloc = 0;
    bloc *current_bloc = FIRST_BLOC;
    
    while(current_bloc->next != NULL){
	if (current_bloc->size < maxTaillePetit && current_bloc->is_free){
	    counter_small_bloc++;
	}
	current_bloc = current_bloc->next;
    }

    if (current_bloc->size < maxTaillePetit && current_bloc->is_free){
	counter_small_bloc ++;
    }
    return counter_small_bloc;
}

int mem_est_alloue(int pByte){
    bloc *current_bloc = FIRST_BLOC;
    
    while(current_bloc->next != NULL){
	if (!current_bloc->is_free && pByte >= current_bloc->start && pByte <=  (current_bloc->start + current_bloc->size)){
	    return 1;
	}
	current_bloc = current_bloc->next;
    }

    if (!current_bloc->is_free && pByte >= current_bloc->start && pByte <=  (current_bloc->start + current_bloc->size)){
	return 1;
    }
    return -1;
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
    int biggest_free_bloc = 0;
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
    case first_fit:
	free_bloc = find_first_allocation_fit(size);
	break;

    case best_fit:
	free_bloc = find_best_allocation_fit(size);
	break;

    case worst_fit:
	free_bloc = find_worst_allocation_fit(size);
	break;

    case next_fit:
	free_bloc = find_next_allocation_fit(size);
	break;

    default: 
	printf("ERROR, unknown strategy\n");
	return -1;
    }

    if(free_bloc == NULL){
	printf("No bloc free bloc can fit this size: %d\n", size);
	return -1;
    }
    new_bloc = split_bloc(free_bloc, size);

    if(STRATEGY == next_fit){
	LAST_BLOC_FIT = new_bloc;
    }

    return new_bloc->start;
}

int libermem(int pBloc){
    bloc* bloc_to_free = search_bloc_by_address(pBloc);
    if (bloc_to_free == NULL){
	return -1;
    } else{
	return free_bloc(bloc_to_free);
    }
}

/*
  Return the first free bloc that can be split to fit the future bloc 
*/
bloc * find_first_allocation_fit(int size){bloc* current_bloc = FIRST_BLOC;

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

bloc * find_best_allocation_fit(int size){
    bloc* current_bloc = FIRST_BLOC;
    bloc* current_best_bloc = NULL;
    int additionnal_size = INT_MAX;

    while (current_bloc->next!= NULL) {
	if (current_bloc->is_free && current_bloc->size >= size && (current_bloc->size - size) < additionnal_size){
	    current_best_bloc = current_bloc;
	    additionnal_size = current_bloc->size - size;
	}
	current_bloc = current_bloc->next;
    }
    if (current_bloc->is_free && current_bloc->size >= size && current_bloc->size - size < additionnal_size){
	current_best_bloc = current_bloc;
	additionnal_size = current_bloc->size - size;
    }
    /* printf("current_best_bloc size: %d\n", current_best_bloc->size); */

    return current_best_bloc;
}

bloc * find_worst_allocation_fit(int size) {
    bloc* current_bloc = FIRST_BLOC;
    bloc* current_best_bloc = NULL;
    int additionnal_size = -INT_MAX;

    while (current_bloc->next!= NULL) {
	if (current_bloc->is_free && current_bloc->size >= size && (current_bloc->size - size) > additionnal_size){
	    current_best_bloc = current_bloc;
	    additionnal_size = current_bloc->size - size;
	}
	current_bloc = current_bloc->next;
    }
    if (current_bloc->is_free && current_bloc->size >= size && current_bloc->size - size > additionnal_size){
	current_best_bloc = current_bloc;
	additionnal_size = current_bloc->size - size;
    }
    return current_best_bloc;
}

bloc * find_next_allocation_fit(int size){
    bloc* current_bloc = NULL;

    if(LAST_BLOC_FIT == NULL){
	current_bloc = find_first_allocation_fit(size);
	return current_bloc;
    }
    
    current_bloc = LAST_BLOC_FIT->next;

    if (current_bloc == NULL){
	current_bloc = FIRST_BLOC;
    }

    while (current_bloc != LAST_BLOC_FIT) {
	if (current_bloc->is_free && current_bloc->size >= size){
	    return current_bloc;
	}

	current_bloc = current_bloc->next;
	if (current_bloc == NULL){
	    current_bloc = FIRST_BLOC;
	}
    }
    return NULL;
}
