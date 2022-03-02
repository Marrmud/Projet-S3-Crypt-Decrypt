#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/// /!\ Le cbcCrypt et cbcDecrypt marchent parfaitement avec des exemples "direct" : fichier text, cependant ne passe pas les test des images (.png, autre)/!\ 


void cbcEncrypt(char* vi, char* key, char* fileSourceName, char* fileDestName){
    /// \brief On profite de l'opérateur '^' pour implementer la logique de la fonction xor.
  /// \param[in] key La clé pour crypter / decrypter.
  /// \param[in] vi Vecteur initiateur permetant de chiffrer le premier bloc.
  /// \param[in] fileSourceName Nom du fichier source.
  /// \param[in] fileDestName Nom du fichier destination.
  /// \returns Resultat dans un fichier du message chiffré en CBC.
    FILE * fileSource = fopen(fileSourceName, "r");
    FILE * fileDest = fopen(fileDestName, "w");
    /// 'if(fileSource == NULL || fileDest == NULL)' => Verifie l'existance du fichier.
    /// NB : fileDest est ouvert en 'w' => créer un fichier si'il n'existe pas
    if(fileSource == NULL || fileDest == NULL){
        printf("Erreur fopen !\n");
        exit(255);
    }
    int i = 0;
    int v = 1;
    char c,outputCrypt,output;

    while((c = fgetc(fileSource)) != EOF || i!=0){
        if(c == -1) c = ' ';
        output = c ^ vi[i];
        outputCrypt = output ^ key[v];
        vi[i] = outputCrypt;
        fputc(outputCrypt,fileDest); //NB : j'utilise fputc au lieu de fprintf comme sur le Xor aucune raison valable, les deux donnent le même résultat
        i++;
        v++;
        if(key[v] == '\0') v=0;
        if (i == 16) {i = 0; v = 1; }//Plus tard j'ai compris que faire key[v%16] => revenais automatiquement au debut de la clé, le but de ce if est donc de boucler autant de fois que nécessair sur la clé pour crypter
    }
    fclose(fileSource);
    fclose(fileDest);
}

void cbcDecrypt(char* vi, char* key, char* fileSourceName, char* fileDestName){
    /// \brief On profite de l'opérateur '^' pour implementer la logique de la fonction xor.
  /// \param[in] key La clé pour crypter / decrypter.
  /// \param[in] vi Vecteur initiateur permetant de dechiffrer le premier bloc.
  /// \param[in] fileSourceName Nom du fichier source.
  /// \param[in] fileDestName Nom du fichier destination.
  /// \returns Resultat dans un fichier du message dechiffré en CBC.
    FILE * fileSource = fopen(fileSourceName, "r");
    FILE * fileDest = fopen(fileDestName, "w");
    /// 'if(fileSource == NULL || fileDest == NULL)' => Verifie l'existance du fichier.
    /// NB : fileDest est ouvert en 'w' => créer un fichier si'il n'existe pas
    if(fileSource == NULL || fileDest == NULL){
        printf("Erreur fopen !\n");
        exit(255);
    }
    int i = 0;
    int v = 1;
    char c,outputDecrypt,output;

    while((c = fgetc(fileSource)) != EOF){
        output = c ^ key[v];
        outputDecrypt = output ^ vi[i];
        vi[i] = c;
        fputc(outputDecrypt,fileDest);
        i++;
        v++;
        if(key[v] == '\0') v=0;
        if (i == 16) {i = 0; v = 1; }
    }
    fclose(fileSource);
    fclose(fileDest);
}

