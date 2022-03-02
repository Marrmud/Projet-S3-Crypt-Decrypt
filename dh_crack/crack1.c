#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dh_crack.h"

#define MAX_BUFFER_SIZE 10000
#define valeurMax 10

void crack_emondage(char* fileSourceName, int lenKey){
    /// \brief Fonction pour cracker utilisant un émondage des caractères possible pour la clef.
    /// \param[in] lenKey Longueur de la clé (exemple 3).
    /// \param[in] fileSourceName Nom du fichier dans lequel le message à cracker se trouve.
    /// \returns Resultat d'un tableau de valeurs de clés possibles
    char key[lenKey][valeurMax];
    int nonValid = 0;
    /*==================BOUCLE allouer char specifique au lieu de random (Debug pour test_crack)==================*/
    for(int m = 0; m<lenKey; m++){
        for(int n = 0; n<valeurMax; n++){
            key[m][n] = 'a';
        }
    }
    /*==================================================================================================*/
    //////////////////////////////////////////////////////
    FILE * fp = fopen(fileSourceName, "r");
    char msg[MAX_BUFFER_SIZE];
    fgets(msg, MAX_BUFFER_SIZE, fp);
    if(fp == NULL){printf("Erreur fopen\n"); exit(255);}
    //////////////////////////////////////////////////////
    int lenMsg = strlen(msg);
  /*Premier char de la clef != de 0*/
    for (int a = 0; a<lenKey; a++){ //a = lignes, b = colonnes
        int b = 0;
        if (a == 0){
            for (char car = '1' ; car<='9'; car++){
                int x = 0;
                nonValid = 0;
                while (a+lenKey*x < lenMsg){
                    if (!isalnum(msg[a+lenKey*x]^car) && !isspace(msg[a+lenKey*x]^car) && !ispunct(msg[a+lenKey*x]^car)){
                        nonValid = 1;
                        break;
                    }
                    x++;
                }
                if (nonValid == 0){key[a][b] = car;b++;} 
            }
        }

        else if (a != 0){
         for (char car = '0' ; car<='9'; car++){
            int x = 0;
            nonValid = 0;
            while (a+lenKey*x < lenMsg){
                if (!isalnum(msg[a+lenKey*x]^car) && !isspace(msg[a+lenKey*x]^car) && !ispunct(msg[a+lenKey*x]^car)){
                    nonValid = 1;
                    break;
                }
                x++;
                
            }
            if (nonValid == 0){key[a][b] = car;b++;}
        }
        }
    }
    for(int a = 0; a<lenKey; a++){
        printf("[");
        for(int b = 0; b<valeurMax; b++)
            if (isdigit(key[a][b])) printf("%c,",key[a][b]);
        
        printf("]");
        printf("\n");
    }
    fclose(fp);
    printf("\n\nEverything is under control my friend\n"); //POUR LE TEST
}