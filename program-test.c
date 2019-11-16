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
    printf("%p \n", ptr);

    int number_allocated_block = nblocalloues();
    printf("Nombre de bloc alloue: %d \n", number_allocated_block);
}
