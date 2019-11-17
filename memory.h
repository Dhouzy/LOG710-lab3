/* #ifndef _MEMORY_H_ */
/* #define _MEMORY_H_ */

/* https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_C_libraries.html */

enum strategy{first_fit, best_fit, worst_fit, next_fit};


// Return success or error
extern int * initmem(int size, enum strategy strategy);

// Return start location of the block (pBloc)
extern int alloumem(int size);

extern int libermem(int pBloc);

// Retourne le nombre de blocs de mémoire libres 
extern int nbloclibres();

// Cette fonction retourne le nombre de blocs alloués présentement
extern int nblocalloues();

// Combien de mémoire libre (non allouée) au total.
extern int memlibre();

// Taille du plus grand bloc libre.
extern int mem_pgrand_libre();

// Combien de petits blocs (plus petit que maxTaillePetit) non alloués y a-t-il présentement en mémoire?
extern int mem_small_free(int maxTaillePetit);

// Est-ce qu’un octet (byte) particulier est alloué
extern int mem_est_alloue(int pByte);

