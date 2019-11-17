#include "memory.h"
#include <stdlib.h>
#include <stdio.h>

/*
  Compile program
  gcc program-test.c memory.o 
  TODO make simple MAKEFILE
*/



void test_best_allocation(){
    initmem(1000, best_fit);

    alloumem(300);
    int start1 = alloumem(200);
    alloumem(200);
    alloumem(200);
    int start2 =  alloumem(100);

    printf("start2: %X\n", start2);
    libermem(start1);
    libermem(start2);

    if(mem_est_alloue(start1 +1) == -1){
	printf("ok \n");
    }

    if(mem_est_alloue(start2 +1) == -1){
	printf("ok \n");
    }

    int start3 = alloumem(50);

    if(start2 == start3){
    	printf("start2 = start3 good \n");
    }

    // one more test
    if(mem_est_alloue(start2+1) == 1){
    	printf("good \n");
    }
}

int main(){

    test_best_allocation();

    /* int *ptr = initmem(1000, first_fit); // FIXME address = 0? */

    /* int start1 = alloumem(750); */
    /* int start2 = alloumem(50); */
    /* int start3 = alloumem(150); */

    
    /* libermem(start2); */
    /* libermem(start3); */


    int number_allocated_bloc = nblocalloues();
    printf("Nombre de bloc alloue: %d \n", number_allocated_bloc);

   int number_free_bloc = nbloclibres();
    printf("Nombre de bloc libre: %d \n", number_free_bloc);

    int free_memory = memlibre();
    printf("Memore libre: %d \n", free_memory);

    int biggest_free_bloc = mem_pgrand_libre();
    printf("Plus grans bloc libre est de: %d \n", biggest_free_bloc);

    int counter_small_bloc = mem_small_free(51);
    printf("Nombre de bloc libre plus petit que 51: %d \n", counter_small_bloc);

}
