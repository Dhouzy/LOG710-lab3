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

    alloumem(100);
    int virt_start = alloumem(250);
    printf("virt_start %d\n", virt_start);

    int number_allocated_bloc = nblocalloues();
    printf("Nombre de bloc alloue: %d \n", number_allocated_bloc);


    int number_free_bloc = nbloclibres();
    printf("Nombre de bloc free: %d \n", number_free_bloc);

}
