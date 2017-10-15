#include <chrono>
#include <x86intrin.h>
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <array>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace std::chrono;

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


/*** Resultat des calculs ***/
using Statistic = array<uint64_t, 17>;
std::ostream & operator<<(std::ostream & stream, Statistic const &term) {
  stream << "[" << unsigned(term[0]);
  for (unsigned i=1; i < 17; i++) stream << "," << unsigned(term[i]);
  stream << "]";
  return stream;
}

/** Calcul et affiche le temps de calcul d'une fonction
 * @param func la fonction à executer
 * @param reftime le temps de référence
 * @return le temps d'exécution
 **/
template <int ncycles(ar16 p)> double timef(vector<ar16> &v,
					    double reftime) {
  high_resolution_clock::time_point tstart, tfin;
  Statistic stat = {};
  uint_fast64_t sz = v.size();

  tstart = high_resolution_clock::now();
  for (uint_fast64_t i=0; i < sz; i++)
    stat[ncycles(v[i])]++;
  tfin = high_resolution_clock::now();

  auto tm = duration_cast<duration<double>>(tfin - tstart);
  cout << stat << endl;
  cout << "time = " << tm.count() << "s";
  if (reftime != 0) cout << ", speedup = " << reftime/tm.count();
  cout << endl;
  return tm.count();
}

/**********************************************************************/
/******************** Calcul du nombre de Cycles **********************/
/**********************************************************************/

int nb_cycles_ref(ar16 p) {
  ar16 v {};
  int i, j, c = 0;
  for (i = 0; i < 16; i++) {
    if (v[i] == 0) {
      for (j=i; v[j] == 0; j = p[j]) v[j] = 1;
      c++;
    }
  }
  return c;
}

int nb_cycles_unroll(ar16 arp) {
  perm p = perm_ar16(arp);
  perm x0, x1 = permid;

  x0 = _mm_min_epi8(x1, _mm_shuffle_epi8(x1, p));
  p = _mm_shuffle_epi8(p, p);
  x1 = _mm_min_epi8(x0, _mm_shuffle_epi8(x0, p));
  p = _mm_shuffle_epi8(p, p);
  x0 = _mm_min_epi8(x1, _mm_shuffle_epi8(x1, p));
  p = _mm_shuffle_epi8(p, p);
  x1 = _mm_min_epi8(x0, _mm_shuffle_epi8(x0, p));
  perm res = (permid == x1);
  return _mm_popcnt_u32(_mm_movemask_epi8(res));
}


/**********************************************************************/
/****************************** main **********************************/
/**********************************************************************/

int main() {
  double sp_ref;

  auto vrand = rand_perms(10000000);

  cout << "RefType  : " << endl;
  sp_ref = timef<nb_cycles_ref>(vrand, 0.);
  cout << "UnrollTyp: " << endl;
  timef<nb_cycles_unroll>(vrand, sp_ref);

  return EXIT_SUCCESS;
}
