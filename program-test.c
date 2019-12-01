#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*
  Compile program
  gcc program-test.c memory.o 
  TODO make simple MAKEFILE
*/

int RAND_MIN_NUMBER = 50;
int RAND_MAX_NUMBER = 300;

typedef struct Stats {
    char *strategy_name;
    int number_allocated_bloc;
    int number_free_bloc;
    int free_memory;
    int biggest_free_bloc;
    int counter_small_bloc;
}stats;

typedef struct InfoBloc {
    int is_bloc_freed;
    int address;
}infoBloc;

void print_stats();
int read_integer_input();
int read_hex_input();
int get_rand_value();
void run_strategies();
int get_first_used_bloc(infoBloc arrBloc[]);
void print_collected_stats(stats stat);
void run_operations(int *operations, infoBloc *arrBloc);
void collect_stat(stats *stat);

int main(){

    printf("Choisisez la stratégie.\n");
    printf("1. First Fit\n");
    printf("2. Best Fit\n");
    printf("3. Worst Fit\n");
    printf("4. Next Fit\n");
    printf("5. Comparaison des stratégies");
    printf("Entrez un chiffre de 1 à 5: ");

    int strategy_type = -1;
    strategy_type = read_integer_input();

    if(strategy_type < 1 || strategy_type > 5){
	printf("Ooops veuilez choisir une valeur entre 1 et 4 \n");
	return -1;
    }
    if (strategy_type == 5){
	run_strategies();
	return 0;
    }

    int memory_size = -1;
    printf("Entrez la taille de la mémoire à allouer entre 100 et 10000 (un chiffre entier) :");
    memory_size = read_integer_input();
    if (memory_size < 100 || memory_size > 10000){
	printf("Valeur de la taille de la mémoire invalide\n");
	return -1;
    }

    int ptr = -1; 
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
	    printf("Entrez une adresse mémoire (Première adresse de la zone mémoire étant: %X): ", ptr);
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


void run_strategies(){
    srand(time(NULL));

// Generate sequence
// if -1 free a bloc 
    int operations[100], n;

    for(n=0; n<100; n++) {
	operations[n] = get_rand_value();
	/* printf("value at cell %d is %d \n", n, operations[n]); */
    }   

    stats stat[4];
    stat[0].strategy_name = "First Fit";
    stat[1].strategy_name = "Best Fit";
    stat[2].strategy_name = "Worst Fit";
    stat[3].strategy_name = "Next Fit";
    
    infoBloc arrBloc[100];
    for(int l=0; l<100; l++){ arrBloc[l].address = -1; arrBloc[l].is_bloc_freed = 0;}

    // Run operation
    initmem(1000, first_fit);
    run_operations(operations, arrBloc);
    collect_stat(&stat[0]);

    initmem(1000, best_fit);
    run_operations(operations, arrBloc);
    collect_stat(&stat[1]);

    initmem(1000, worst_fit);
    run_operations(operations, arrBloc);
    collect_stat(&stat[2]);

    initmem(1000, next_fit);
    run_operations(operations, arrBloc);
    collect_stat(&stat[3]);

    //Print collected stats
    for(int i=0; i<4; i++){
	print_collected_stats(stat[i]);
    }
}

void collect_stat(stats *stat){
    stat->biggest_free_bloc = mem_pgrand_libre();
    stat->counter_small_bloc = mem_small_free(50);
    stat->free_memory = memlibre();
    stat->number_allocated_bloc = nblocalloues();
    stat->number_free_bloc = nbloclibres();
}

void run_operations(int *operations, infoBloc *arrBloc){
    for(int l=0; l<100; l++){ arrBloc[l].address = -1; arrBloc[l].is_bloc_freed = 0;}

    int temp_address = -1;
    for(int i=0; i<100; i++){
	/* printf("alloue %d\n", operations[i]); */
	if (operations[i] == -1){
	    int bloc_index = get_first_used_bloc(arrBloc);
	    arrBloc[bloc_index].is_bloc_freed = 1;
	    libermem(arrBloc[bloc_index].address);
	} else {
	    temp_address = alloumem(operations[i]);
	}
	arrBloc[i].address = temp_address;
	arrBloc[i].is_bloc_freed = 0;
	temp_address = -1;
    }
}


void print_collected_stats(stats stat){
    printf("=====================================================================\n");
    printf("STRATÉGIE: %s\n", stat.strategy_name);

    printf("Nombre de bloc alloue: %d \n", stat.number_allocated_bloc);

    printf("Nombre de bloc libre: %d \n", stat.number_free_bloc);

    printf("Memore libre: %d \n", stat.free_memory);

    printf("Plus grans bloc libre est de: %d \n", stat.biggest_free_bloc);

    printf("Nombre de bloc libre plus petit que 50: %d\n", stat.counter_small_bloc);
    printf("=====================================================================\n\n\n");
}

int get_first_used_bloc(infoBloc arrBloc[]){
    for(int i=0; i<100; i++){
	if (arrBloc[i].is_bloc_freed == 0 && arrBloc[i].address != -1){
	    arrBloc[i].is_bloc_freed == 1;
	    return i;
	}
    }
    return -1;
}

int get_rand_value(){
    int r = rand() % 5 ;
    if(r == 0){
	return -1;
    }
    r = (rand() % 
	 (RAND_MAX_NUMBER - RAND_MIN_NUMBER + 1)) + RAND_MIN_NUMBER; 
    return r;
}
