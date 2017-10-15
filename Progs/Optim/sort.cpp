#include <chrono>
#include <x86intrin.h>
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <array>
#include <iostream>
#include <iomanip>

using namespace std;

/**********************************************************************/
/************** Défnitions des types et convertisseurs ****************/
/**********************************************************************/

/** type tableau classique
 * tableau de 16 entiers positifs sur 8 bits
 * permet l'accès indicé
 **/
using ar16 = array<uint8_t, 16>;


/** Variable vectorielle
 * vecteur de 16 byte représentant une permutation
 * supporte les commandees vectorielles du processeur
 **/
using perm = uint8_t __attribute__ ((vector_size (16), __may_alias__));


/** type union permettant la conversion entre ar16 et perm **/
using converter = union { ar16 p; perm v8; };

/** Converti une permutation de type vectorielle en un tableau de 16 bytes
 * @param v, une permutation de type vectorielle (perm)
 * @return le tableau de 16 bytes (ar16) correspondant
 **/
ar16 ar16_perm(perm v) {
    converter c;
    c.v8 = v;
    return c.p;
}

/** Converti un tableau de 16 bytes en une permutation de type vectoriel
 * @param p, un tableau de 16 byte
 * @return la permutation équivalente de type vectoriel
 **/
perm perm_ar16(ar16 p) {
    converter c;
    c.p = p;
    return c.v8;
}

/**********************************************************************/
/***************** Fonctions d'affichages *****************************/
/**********************************************************************/


/** Affichage ar16
 * Définition de l'opérateur d'affichage << pour le type ar16
 **/
ostream & operator<<(ostream & stream, ar16 const &ar) {
    stream << "[" << setw(2) << hex << unsigned(ar[0]);
    for (unsigned i=1; i < 16; ++i)
        stream << "," << setw(2) << hex << unsigned(ar[i]) << dec;
    stream << "]";
    return stream;
}

/** Affichage perm
 * Définition de l'opérateur d'affichage << pour le type perm
 **/
ostream & operator<<(ostream & stream, perm const &p) {
    stream << ar16_perm(p);
    return stream;
}

/**********************************************************************/
/*********** Permutations Variables globales **************************/
/**********************************************************************/

/** Permutation identité **/
const ar16 ar16id = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
const perm permid = perm_ar16(ar16id);


/**********************************************************************/
/************************ Utilitaires *********************************/
/**********************************************************************/


/** permute aléatoirement le tableau ar16id
 * @return le tableau de 16 bytes (ar16) correspondant
 **/
ar16 random_ar16() {
    ar16 res = ar16id;
    random_shuffle(res.begin(), res.end());
    return res;
}

/** Construit un vecteurs d'ar16 au hasard
 * @param sz le nombre d'élements
 * @return le vecteur correspondant
 **/
std::vector<ar16> rand_perms(int sz) {
  std::vector<ar16> res(sz);
  std::srand(std::time(0));
  for (int i = 0; i < sz; i++) res[i] = random_ar16();
  return res;
}

/** Calcul et affiche le temps de calcul d'une fonction
 * @param func la fonction à executer
 * @param reftime le temps de référence
 * @return le temps d'exécution
 **/
template<typename Func>
double timethat(Func fun, double reftime = 0) {
  std::chrono::high_resolution_clock::time_point tstart, tfin;

  tstart = std::chrono::high_resolution_clock::now();
  fun();
  tfin = std::chrono::high_resolution_clock::now();

  auto tm = std::chrono::duration_cast<
    std::chrono::duration<double>>(tfin - tstart);
  std::cout << "time = " << std::setprecision(3) << tm.count() << "s";
  if (reftime != 0) std::cout << ", speedup = " << reftime/tm.count();
  std::cout << std::endl;
  return tm.count();
}

/**********************************************************************/
/******************** Algorithmes de tris *****************************/
/**********************************************************************/

/** Tri un tableau stocké dans une permutation
 * @param a la permutation à trier
 * @return la permutation triée
 ***
 * Algorithme: tri par insertion
 **/
ar16 insertion_sort (ar16 a){
  for (int i = 0; i < 16; i++)
    for (int j = i; j > 0 && a[j] < a[j-1]; j--)
      std::swap(a[j], a[j-1]);
  return a;
}

/** Tri un tableau stocké dans une permutation
 * @param a la permutation à trier
 * @return la permutation triée
 ***
 * Algorithme: tri par base
 **/
ar16 radix_sort(ar16 a) {
  ar16 stat = {}, res;
  for (int i = 0; i < 16; i++) stat[a[i]]++;
  int c = 0;
  for (int i = 0; i < 16; i++)
    for (int j = 0; j < stat[i]; j++) res[c++]=i;
  return res;
}

// Sorting network Knuth AoCP3 Fig. 51 p 229.
static const array<perm, 9> rounds =
 //      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15
    {{ { 1, 0, 3, 2, 5, 4, 7, 6, 9, 8,11,10,13,12,15,14},
       { 2, 3, 0, 1, 6, 7, 4, 5,10,11, 8, 9,14,15,12,13},
       { 4, 5, 6, 7, 0, 1, 2, 3,12,13,14,15, 8, 9,10,11},
       { 8, 9,10,11,12,13,14,15, 0, 1, 2, 3, 4, 5, 6, 7},
       { 0, 2, 1,12, 8,10, 9,11, 4, 6, 5, 7, 3,14,13,15},
       { 0, 4, 8,10, 1, 9,12,13, 2, 5, 3,14, 6, 7,11,15},
       { 0, 1, 4, 5, 2, 3, 8, 9, 6, 7,12,13,10,11,14,15},
       { 0, 1, 2, 6, 4, 8, 3,10, 5,12, 7,11, 9,13,14,15},
       { 0, 1, 2, 4, 3, 6, 5, 8, 7,10, 9,12,11,13,14,15}
       }};

/** Tri un tableau stocké dans une permutation
 * @param a la permutation à trier
 * @return la permutation triée
 ***
 * Algorithme: réseau de tri
 **/
perm sort(perm a) {
  for (perm round : rounds) {
    perm minab, maxab, mask;
    perm b = _mm_shuffle_epi8(a, round);
    mask = _mm_cmplt_epi8(round, permid);
    minab = _mm_min_epi8(a, b);
    maxab = _mm_max_epi8(a, b);
    a = _mm_blendv_epi8(minab, maxab, mask);
  }
  return a;
}


/**********************************************************************/
/****************************** main **********************************/
/**********************************************************************/

int main() {
  double sp_ref;

  auto vrand = rand_perms(10000000);

  cout << "Std lib: ";
  sp_ref = timethat([vrand]() {
      for (ar16 v : vrand) {
	std::sort (v.begin(), v.end());
	assert(v == ar16id);
      }
    });
  cout << "Fast    : ";
  timethat([vrand]() {
      for (ar16 v : vrand) assert(ar16_perm(sort(perm_ar16(v))) == ar16id);
  }, sp_ref);
  cout << "Insert  : ";
  timethat([vrand]() {
      for (ar16 v : vrand) assert(insertion_sort(v) == ar16id);
    }, sp_ref);
  cout << "Radix16 : ";
  timethat([vrand]() {
      for (ar16 v : vrand) assert(radix_sort(v) == ar16id);
    }, sp_ref);

  return EXIT_SUCCESS;
}
