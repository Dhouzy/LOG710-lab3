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
    int virt_start = 0;

    virt_start = alloumem(100);
    virt_start = alloumem(200);

    printf("virt_start %d\n", virt_start);

    int number_allocated_bloc = nblocalloues();
    printf("Nombre de bloc alloue: %d \n", number_allocated_bloc);

    int number_free_bloc = nbloclibres();
    printf("Nombre de bloc libre: %d \n", number_free_bloc);

    int free_memory = memlibre();
    printf("Memore libre: %d \n", free_memory);

    int biggest_free_bloc = mem_pgrand_libre();
    printf("Plus grans bloc libre est de: %d \n", biggest_free_bloc);
}
