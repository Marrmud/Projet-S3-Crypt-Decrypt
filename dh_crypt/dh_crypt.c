#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "dh_crypt.h"

#define TAILLE 16
#define MAX_BUFFER 100

/*exit(255) est une formalité car pas de traitement d'erreur*/

void help(void){
    /// \brief Print les lignes de Help.
    printf("    > dh_crypt -i infile -o outfile -k key -m xor\n");
    printf("    > dh_crypt -i infile -o outfile -k key -m cbc-crypt vi\n");
    printf("    > dh_crypt -i infile -o outfile -k key -m cbc-uncrypt vi\n");
}

int main(int argc, char * argv[]){
    int i, j; // variable compteur
    char* key = NULL;//Valeur par defaut => si la valeur n'est pas modifié (verifié plus bas)=> renvoie erreur
    char* vi = NULL;
    char* fileSourceName = NULL;
    char* fileDestName = NULL;
    int methode = 0;//idem valeur par defaut

    for(i = 0; i<argc; i++){
        /// \brief Parcours les arguments, et verifie selon les conditions listés ci-dessous.
        //if(argc == 2 && strcmp(argv[1], "-h") != 0) {printf("\nUn seule argument passe\n"); exit(255);}
        if (strcmp(argv[i], "-h") == 0){ // a chaque tour de boucle => si argument = '-h' => exit la boucle
            printf("\nAide pour '%s':\n",argv[0]);
            help();
            exit(255);
        }

        else if(strcmp(argv[i], "-m") == 0){
            if (argv[i+1]== NULL) {printf("\nERREUR : Usage incorecte\n");exit(255);}
            else if(strcmp(argv[i+1], "xor") == 0){
                methode = 1;
            }
            else if(strcmp(argv[i+1], "cbc-crypt") == 0){
                if (argv[i+2]== NULL) {printf("\nERREUR : Usage incorecte\n");exit(255);}
                else{
                    methode = 2;
                    vi = malloc((strlen(argv[i+2]) + 1) * sizeof(char));
                    strcpy(vi, argv[i+2]);
                }
            }
            

            else if(strcmp(argv[i+1], "cbc-uncrypt") == 0){
                if (argv[i+2]== NULL) {printf("\nERREUR : Usage incorecte\n");exit(255);}
                else{
                    methode = 3;
                    vi = malloc((strlen(argv[i+2]) + 1) * sizeof(char));
                    strcpy(vi, argv[i+2]);
                }
            }
        }
        if ((strcmp(argv[i], "-i") == 0)){
            if (argv[i+1]== NULL) {printf("\nERREUR : Usage incorecte\n");exit(255);}
            else{
            fileSourceName = malloc((strlen(argv[i+1]) + 1) * sizeof(char));
            strcpy(fileSourceName, argv[i+1]);
            }
            }
        else if ((strcmp(argv[i], "-o") == 0)){
            if (argv[i+1]== NULL) {printf("\nERREUR : Usage incorecte\n");exit(255);}
            else{
            fileDestName = malloc((strlen(argv[i+1]) + 1) * sizeof(char));
            strcpy(fileDestName, argv[i+1]);
            }
        }
        else if ((strcmp(argv[i], "-k") == 0)){
            if (argv[i+1]== NULL) {printf("\nERREUR : Usage incorecte\n");exit(255);}
            else{
            key = malloc((strlen(argv[i+1]) + 1) * sizeof(char));
            strcpy(key, argv[i+1]);
            }
        }
        //if (strcmp(argv[1],"-i") == 0 && strcmp(argv[2],"-o") == 0  && strcmp(argv[3],"-k") == 0 ) {printf("\nErreur test\n");exit(255);}
    }
    
    for(j = 0; j<argc; j++){
        if(methode != 0 && fileSourceName != NULL && fileDestName != NULL && key != NULL){
           if(methode == 1) {xorCrypt(key,fileSourceName,fileDestName); exit(255);}
           if(methode == 2) {
                if(strlen(vi) == 16) {cbcEncrypt(vi,key,fileSourceName,fileDestName); exit(255);}
                else{
                    printf("\nERREUR : VI invalide (16char)\n");
                    exit(255);
                }
           }
           if(methode == 3) {
               if(strlen(vi) == 16) {cbcDecrypt(vi,key,fileSourceName,fileDestName); exit(255);}
                else{
                    printf("\nERREUR : VI invalide (16char)\n");
                    exit(255);
                }
           }
        }
        else if(methode == 0 || fileSourceName == NULL || fileDestName == NULL || key == NULL || (methode == 3 && strlen(vi) != 16)) { //(methode == 3 && strlen(vi) != 16) reste innutile car verifié plus en haut.
            printf("\nERREUR : Usage Incorecte\n"); exit(255);
        }
    }
    free(vi);
    free(fileDestName);
    free(fileSourceName);
    free(key);
    
    

    return 0;
}