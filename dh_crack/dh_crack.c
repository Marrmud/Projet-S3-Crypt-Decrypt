#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dh_crack.h"
#include <ctype.h>


int main(int argc, char const *argv[]) {
  int isNotDig = 0; //valeur par defaut pour verifier si le paramètre est un digit ou non (0 = est un digit, 1 = n'est pas un digit)
  int lkey;
  int i;
  int methode = 0;
  char* fileSourceName = NULL; // valeur par defaut verifier plus tard si le file source name est modifié ou non (donc entré ou non)

   for (i = 0; i < argc; i++){
    if ( strcmp(argv[i], "-h") == 0 ){
      printf("\nAide pour '%s':\n",argv[0]);
      printf("\n==============================================\n");
      printf("    > dh_crack -i infile -m c1 -k length\n    > (indispo)dh_crack -i infile -m all -k length -d dicofile\n    > dh_crack -h help\n");
      printf("\n==============================================\n");
      exit(255);
      }
   }
  if (argc != 2 && argc != 7 && argc != 8 && argc != 9){
    printf("ERREUR : nombre d'arguments incorrect \n dh_genkey -h pour l'aide\n");
    exit(255);
  }

    for (i = 0; i < argc; i++){
     /////
    if ( strcmp(argv[i], "-i") == 0 ){
      
      if (argv[i+1] != NULL){
        fileSourceName = malloc ((strlen(argv[i+1]) + 1) * sizeof(char));
        strcpy(fileSourceName, argv[i+1]);
      }
      }
    /////
    else if (strcmp(argv[i], "-m") == 0 ){
      if(strcmp(argv[i+1], "c1")==0){
        if (argv[i+1] != NULL) methode = 1;
      }
    }
    /////
    else if ( strcmp(argv[i], "-k") == 0 ){
      if (argv[i+1] != NULL){
        lkey= atoi(argv[i+1]);
        if(lkey == 0){
          isNotDig = 1;
          printf("ERREUR : clef doit être un nombre\n");
          exit(255);
        }
      }
    } 
  } 
  
  if(methode != 1) {printf("ERREUR : methode incorecte \n"); exit(255);}
  else if (fileSourceName == NULL) {printf("ERREUR : fichier obligatoire \n"); exit(255);}
  else if (methode == 1){
    crack_emondage(fileSourceName, lkey);
  }
  free(fileSourceName);
  return 0;
}