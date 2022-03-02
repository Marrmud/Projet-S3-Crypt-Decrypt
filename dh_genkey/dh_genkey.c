#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "dh_prime.h"



 
int main(int argc, char* argv[])
{ int option;
  int i=0;
  while (i<argc){ //même utilisation que la boucle for des autres "dh_" => Regarder d'abord tout les arguments (si'il y'en a) => si qulconque argument = -h => afficher aide => quitter 
    if (strcmp(argv[i], "-h")==0)
    {
      printf("\nAide pour '%s':\n",argv[0]);
      printf("\n==============================================\n");
      printf("    > dh_genkey : mode interactif avec menu\n    > dh_genkey -o out_file : génère une clef partagée et la sauve dans out_file\n    > dh_genkey -h help\n");
      printf("\n==============================================\n");
      exit(255);
    }
    i++;
  }
  while ((option = getopt(argc, argv, "o:")) != -1) {
    switch (option) 
    {
      case 'o' :
        if (optarg == NULL) { // NULL => Argument inexistant (explicite dans le printf)
          printf("ERREUR : Arguments manquants\n");
          exit (255);
        }
        generated_shared_key_dh(MIN_PRIME, random_long(MIN_PRIME, MAX_PRIME), optarg);
        return 0;
      case '?':
        printf("ERREUR : Usage Incorecte\n"); //'?' => Argument inconue (switch)
        exit(255);  
    }
  }
  long key= generate_shared_key(MIN_PRIME, random_long(MIN_PRIME, MAX_PRIME));
  return 0;
}