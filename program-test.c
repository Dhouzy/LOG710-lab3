#include "memory.h"
#include <stdlib.h>
#include <stdio.h>

/*
  Compile program
  gcc program-test.c memory.o 
  TODO make simple MAKEFILE
*/

int main(){
    int *ptr = initmem(1000, first_fit);

    int start1 = alloumem(750);
    int start2 = alloumem(50);
    int start3 = alloumem(150);

   /* int number_free_bloc_test1 = nbloclibres(); */
   /*  printf("Nombre de bloc libre: %d \n", number_free_bloc_test1); */
    
    libermem(start2);
    /* libermem(start2); */


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
