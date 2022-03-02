#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "dh_prime.h"

long random_long(long min,long max){
/// \brief genere un uint aleatoire entre min et max
/// \param[in] min et max des uint
/// \return n : min≤n≤max
    ///srand(time(NULL)); => Ajouter sleep(1) pour random ?(toujours pas trouvé la solution) 
    return ((rand() % (max-min+1)) + min); // fonction modifié manuellement car soucis sur Windows (génère nombres négatifs)
}

long puissance_mod_n (long a, long e, long n) {
  /// /brief puissance modulaire, calcule a^e mod n
  /// a*a peut depasser la capacite d'un long
  /// https://www.labri.fr/perso/betrema/deug/poly/exp-rapide.html
  /// vu au S1 en Python
  long p;
  for (p = 1; e > 0; e = e / 2) {
    if (e % 2 != 0)
      p = (p * a) % n;
    a = (a * a) % n;
  }
  return p;
}

int test_prime (long n) {
/// \brief test de primarite, crible d'Erathostene
/// \returns 1 le nombre est premier, 0 sinon
  long d;

  if (n % 2 == 0)
    return (n == 2);
  for (d = 3; d * d <= n; d = d + 2)
    if (n % d == 0)
      return 0;
  return 1;
}

int rabin (long a, long n) {
  /// \brief test de Rabin sur la pimarite d'un entier
  /// \brief c'est un test statistique. Il est plus rapide que le precedent.
  /// \param[in] a : on met 2, ça marche
  /// \param[in] n : le nombre a tester
  /// \returns 1 il est premier, 0 sinon
  long p, e, m;
  int i, k;

  e = m = n - 1;
  for (k = 0; e % 2 == 0; k++)
    e = e / 2;

  p = puissance_mod_n (a, e, n);
  if (p == 1) return 1;

  for (i = 0; i < k; i++) {
    if (p == m) return 1;
    if (p == 1) return 0;
    p = (p * p) % n;
  }

  return 0;
}

long generePremierRabin(long min,long max,int *cpt){
  /// \brief fournit un nombre premier entre min et max.
  /// La primarite est verifiee avec le test de rabin
  /// \param[in] min et max
  /// \param[out] cpt : le nombre d'essais pour trouver le nombre
  /// \returns un nombre premier p min≤p≤max
  long num;
  *cpt=1;
  int a=2;
  do{
    num = random_long(min,max);
  } while (num%2!=1);

  while (!rabin(a,num) && num<max){
    (*cpt)++;
    num=num+2;
  }
  return num;
}

long genPrimeSophieGermain(long min,long max,int *cpt){
  /// \brief fournit un nombre premier de Sophie Germain verifie avec le test de rabin
  /// \param[in] min et max
  /// \param[out] cpt : le nombre d'essais pour trouver le nombre
  /// \returns un nombre premier p min≤p≤max && p = 2*q+1 avec q premier
  long num;
  *cpt=1;
  int a=2;
  do{
    num = random_long(min,max);
  } while (num%2!=1);

  while ((!rabin(a,num) || !rabin(a,2*num+1)) && num<max){
    (*cpt)++;
    num=num+2;
  }
  return 2*num+1;
}

long seek_generator(long start,long p){
  /// \brief recherche d'un generateur du groupe (corps) cyclique Z/pZ avec p premier
  long q = (p-1)/2; /// \note p est un premier de Sophie Germain ie p =2q+1 avec q premier

  /// \note si p = \prod(i=1,k,{p_i}^{n_i}) alors g est un generateur si \forall i 1..k, g^p_i != 1 (mod p)
  /// comme p = 2q+1 et q premier, k = 2 et p_i = 2 et q.
  /// il suffit que g^2 et g^q soient tous les deux differents de 1 (mod p)
  while ((puissance_mod_n(start, 2, p) == 1 || puissance_mod_n(start,q,p) == 1) && start < p-1){
    start++;
  }
  if (start == p-1) return -1;
  return start;
}

long xchange_shared_key(long generateur, long premier){
/// \brief similateur d'echange de clefs de Diffie-Hellman par reseau
/// \param[in] premier : un nombre premier, generateur : un generateur du groupe Z/premierZ
/// \returns la clef commune generee
  //srand(time(NULL));
  long a = random_long(1 , premier-1);
  printf ("Alice et Bob choisissent un nombre premier %ld et une base %ld\n", premier, generateur);
  printf (">Alice choisi un nombre 'a' (secret) au hasard: %ld\n", a);
  long A = puissance_mod_n(generateur, a, premier);
  printf ( "Alice communique a Bob la valeur \"A\": %ld (Valeur de calcul avec 'a')\n", A);
  printf ( "\nAlice et Bob connaissent a present les valeur \"A\", le nombre premier et la base\n");
  printf("------------------------------------------------------------------------\n");
  //srand(time(NULL));
  long b = random_long(1 , premier-1);
  printf (">Bob choisi un nombre b (secret) au hasard: %ld\n", b);
  long B = puissance_mod_n(generateur, b, premier);
  printf ( "Bob communique a Alice la valeur \"B\": %ld (Valeur de calcul avec 'b')\n", B);
  printf("------------------------------------------------------------------------\n");
  printf ( "\nAlice et Bob connaissent a present les valeur \"A\", \"B\", le nombre premier et la base\n");
  
  printf("\n====================================================================================\n");
  printf("Eve connait (publique) : les deux codes echange A = %ld et B = %ld  entre Alice et Bob", A,B);
  printf("\n====================================================================================\n");

  long key_bob = puissance_mod_n(A,b,premier);
  long key_alice = puissance_mod_n(B,a,premier);

  printf("En calculant la cle secrete,\n  > Alice trouve %ld et Bob trouve %ld\n", key_alice, key_bob);
  

  return key_alice;

}

long generate_shared_key(long min,long max){
  /// \brief calcule un nombre premier p de Sophie Germain et un generateur du groupe p/Zp.
  /// appelle le simulateur d'echange de clef partagee.
  /// \returns la clef partagee
  int cpt;
  printf("\nGeneration de la clef partagee\n");
  long premier = genPrimeSophieGermain(min,max,&cpt);
  printf("\nPremier (Sophie Germain) = %ld\n",premier);
  long generateur = seek_generator(3,premier); // exemple 100
  long ordre = puissance_mod_n (generateur, premier-1, premier); // generateur^{premier -1} (mod premier)
  printf("\nGenerateur : %ld Ordre : %ld\n",generateur,ordre);
  printf("------------------------------------------------------------------------\n");
  assert (generateur != -1);
  return xchange_shared_key(generateur, premier);
}

long int_pow(long a, long e) {
  /// \brief puissance russe
  /// \param[in] : a l'entier et e l'exposant
  /// \returns : a^e
  long p;

  for (p = 1; e > 0; e = e / 2) {
    if (e % 2 != 0)
      p = (p * a);
    a = (a * a);
  }
  return p;
}

int nb_digit_base10(long n){
  /// \brief compte le nombre de chiffres d'un nombre entier de type long
  /// \returns le nombre de chiffres calcules.
  int cpt = 0;
  while (n!=0){
    n = n/10;
    cpt++;
  }
  return cpt;
}

long generated_shared_key_dh(long min, long max, char*fichier)
{
  FILE * fichier_dest=fopen(fichier, "w");
  if (fichier_dest== NULL){printf("Erreur fopen\n");exit (255);}
  int cpt;
  long premier = genPrimeSophieGermain(min,max,&cpt);
  fprintf(fichier_dest , "%ld\n" ,premier);
  long generateur = seek_generator(3,premier);
  long ordre = puissance_mod_n (generateur, premier-1, premier);
  fprintf(fichier_dest , "%ld" ,generateur);
  assert (generateur != -1);
  return xchange_shared_key(generateur, premier);
}
