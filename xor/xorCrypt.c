#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef unsigned char byte; ///Non utilisé

void xorCrypt(char* key, char* fileSourceName, char* fileDestName){
    /// \brief On profite de l'opérateur '^' pour créer la fonction xor.
  /// \param[in] key La clé pour crypter / decrypter.
  /// \param[in] fileSourceName Nom du fichier source.
  /// \param[in] fileDestName Nom du fichier destination.
  /// \returns Resultat dans un fichier du message chiffré / déchiffré.
    FILE * fileSource = fopen(fileSourceName,"r");
    FILE * fileDest = fopen(fileDestName,"w");
    //Verifie l'existance du fichier.   NB : fileDest est ouvert en 'w' => créer un fichier si'il n'existe pas
    if(fileSource == NULL || fileDest == NULL){
        printf("Erreur fopen !\n");
        exit(255);
    }
    int output,c; //output = resultat du caractère c^key
    int i = 0;

    while ((c = fgetc(fileSource))!= EOF){
        output = c ^ key[i];
        fprintf(fileDest,"%c",output); //fputc marche tout autant
        i++;
        if (key[i] == '\0') i=0;//revient au debut de la clé si besoin (si len(clé)<len(msg))
    }
    fclose(fileSource);
    fclose(fileDest);

}