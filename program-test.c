#include "memory.h"
#include <stdlib.h>
#include <stdio.h>

/*
  Compile program
  gcc program-test.c memory.o 
  TODO make simple MAKEFILE
*/

void print_stats();
int read_integer_input();
int read_hex_input();

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
void test_next_allocation(){
    initmem(1000, next_fit);

    int start0 = alloumem(100);
    alloumem(95);
    alloumem(5);
    alloumem(100);
    int start1 = alloumem(200);
    alloumem(200);
    int start2 = alloumem(200);
    int start3 = alloumem(100);


    libermem(start0);
    libermem(start2);

    int new_start0 = alloumem(50);

    if (new_start0 == start0){
	printf("good1\n");
    }

    libermem(new_start0);
    int new_start1 = alloumem(45); // FIXME  we should point on the merged one --HERE the last bloc fit is merged. Should we still point on the merged one or the next?

    if (new_start1 == start0){
	printf("good2\n");
    }
    
}
int main(){

    /* test_best_allocation(); */
    /* test_next_allocation(); */

    /* int *ptr1 = initmem(1000, first_fit); */

    /* sleep(1); */
    /* int start1 = alloumem(750); */
    /* int res = 3; */
    /* res = mem_est_alloue(start1+751); */
    /* printf("%d\n", res); */

    /* int start2 = alloumem(50); */
    /* int start3 = alloumem(150); */

    
    /* libermem(start2); */
    /* libermem(start3); */

    
    /* return 0; */

    printf("Choisisez la stratégie.\n");
    printf("1. First Fit\n");
    printf("2. Best Fit\n");
    printf("3. Worst Fit\n");
    printf("4. Next Fit\n");
    printf("Entrez un chiffre de 1 à 4: ");

    int strategy_type = -1;
    strategy_type = read_integer_input();

    if(strategy_type < 1 || strategy_type > 4){
	printf("Ooops veuilez choisir une valeur entre 1 et 4 \n");
	return -1;
    }

    int memory_size = -1;
    printf("Entrez la taille de la mémoire à allouer entre 100 et 10000 (un chiffre entier) :");
    memory_size = read_integer_input();
    if (memory_size < 100 || memory_size > 10000){
	printf("Valeur de la taille de la mémoire invalide\n");
	return -1;
    }

    int *ptr = NULL; 
    if(strategy_type == 1){
	ptr = initmem(memory_size, first_fit);
    }else if(strategy_type == 2){
	ptr = initmem(memory_size, best_fit);
    }else if(strategy_type == 3){
	ptr = initmem(memory_size, worst_fit);
    }else if(strategy_type == 4){
	ptr = initmem(memory_size, next_fit);
    }


    int user_action = -1;
    while(user_action != 5){
	user_action = -1;
	printf("1. Alouer un bloc mémoire\n");
	printf("2. Suprimer un bloc\n");
	printf("3. Vérifier si un octet est alloué\n");
	printf("4. Afficher les statistiques\n");
	printf("5. Quitter\n");
	user_action = read_integer_input();

	if(user_action == 1){
	    int bloc_size = -1;
	    printf("Taille du bloc à allouer: "); 
	    bloc_size = read_integer_input();
	    if(bloc_size != -1){
		int start_bloc = alloumem(bloc_size);
		if(start_bloc != -1){
		    printf("Bloc créer avec succes\n");
		    printf("> Addresse mémoire du bloc: %X\n", start_bloc);
		}
	    }else {
		printf("Valeur incorect\n");
		continue;
	    }
	    
	} else if(user_action == 2){
	    int input_address = -1;
	    printf("Addresse mémoire du bloc à libéré:\n");
	    input_address = read_hex_input();
	    if(input_address != -1){
		int res = libermem(input_address);
		if(res == -1){
		    printf("Aucun bloc à libérer à cette adresse\n"); 
		}else{
		    printf("Bloc libéré avec succès\n");
		}
	    }else{
		printf("Valeur incorect\n");
	    }
	} else if(user_action == 3){
	    int input_address = -1;
	    printf("Entrez une adresse mémoire (Première adresse de la zone mémoire étant: %X): ", &ptr);
	    input_address = read_hex_input();
	    if(input_address != -1){
		int res = mem_est_alloue(input_address);
		if(res == 1){
		    printf("Bloc mémoire est alloué.\n");
		}else{
		    printf("Bloc mémoire n'est pas alloué.\n");
		}
	    }else{
		printf("Valeur incorect\n");
	    }
	    

	} else if(user_action == 4){
	    print_stats();    
	}
    }
}

int read_integer_input(){

    int value = -1;
    char line[100];
    fgets(line, sizeof(line), stdin);
    sscanf (line, "%d", &value); 

    return value;
}

int read_hex_input(){
    int value = -1;
    char line[100];
    fgets(line, sizeof(line), stdin);
    sscanf (line, "%X", &value);

    return value;
}


void print_stats(){
    system("@cls||clear");
    int number_allocated_bloc = nblocalloues();
    printf("Nombre de bloc alloue: %d \n", number_allocated_bloc);

    int number_free_bloc = nbloclibres();
    printf("Nombre de bloc libre: %d \n", number_free_bloc);

    int free_memory = memlibre();
    printf("Memore libre: %d \n", free_memory);

    int biggest_free_bloc = mem_pgrand_libre();
    printf("Plus grans bloc libre est de: %d \n", biggest_free_bloc);

    int counter_small_bloc = mem_small_free(50);
    printf("Nombre de bloc libre plus petit que 50: %d \n\n\n\n", counter_small_bloc);
}
