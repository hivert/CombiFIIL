#include <x86intrin.h>
#include <cassert>
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
 * vecteur de 16 bytes représentant une permutation
 * supporte les commandees vectorielles du processeur
 **/
using perm = uint8_t __attribute__ ((vector_size (16), __may_alias__));


/** type union permettant la conversion entre ar16 et perm **/
using converter = union { ar16 p; perm v8; };

/** Converti une permutation de type vectorielle en un tableau de 16 byte
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
/****** Permutations Variables globales et fonctions de base **********/
/**********************************************************************/

/** Permutation identité **/
const ar16 ar16id = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
const perm permid = perm_ar16(ar16id);

/** Permutation décalée d'un cran à gauche **/
perm decal = perm_ar16({1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,15});


/** Retourne le tableau ar16 correspondant à une transposition
 * @param i un entier inférieur à 16
 * @param j un entier inférieur à 16
 * @return de tableau ar16 de la transposition échangeant les positions i et j
 **/
ar16 transposition(uint64_t i, uint64_t j) {
    assert(i < 16); assert(j < 16);
    ar16 res = ar16id; swap(res[i], res[j]); return res;
}

/** Retourne le tableau d'une permutations aléatoire de taille 16
 **/
ar16 randomar16() {
    ar16 res = ar16id;
    random_shuffle(res.begin(), res.end());
    return res;
}

/***********************************************************************/
/**************** Calcul du nombre de descentes ************************/
/***********************************************************************/


/** Retourne le nombre de descentes d'une permutation, par un algorithme
 * classique. (sans utiliser les commandes avancées du processeur)
 * @param t un tableau ar16 représentant une permutation
 * @return le nombre de position i tel que t[i] > t[i+1]
 **/
int nb_descents_classic(ar16 t) {
    int r = 0;
    for(int i=0; i < 15; i++) {
        if (t[i] > t[i+1]) r+=1;
    }
    return r;
}

/** Retourne le nombre de descentes d'une permutation en utilisant les commandes
 * du processeur sur les données vectorielles
 * @param p une permutation en donnée vectorielle
 * @return le nombre de descente de la permutation
 **/
int nb_descents_optim(perm p) {
    /// Compléter le programme
    return 0;
}

/** Launches tests on random permutations o check the results of nb_descents_optim
 * @param k the number of tests to launch
 **/
void test_nb_descents_optim(int k) {
    converter c;
    for(int i=0; i<k; i++) {
        c.p = randomar16();
        assert(nb_descents_optim(c.v8) == nb_descents_classic(c.p));
    }
    cout << "All tests passed" << endl;
}

/***********************************************************************/
/******************** Fonction principale ******************************/
/***********************************************************************/

int main() {
    // Quelques exemples d'opérations du processeur sur les objets perms
    const ar16 p1_arr = {2,1,7,4,9,15,12,0,5,3,6,8,11,10,14,13};
    perm p1 = perm_ar16(p1_arr);

    const ar16 p2_arr = {5,8,2,15,3,9,4,1,14,6,10,11,7,12,14,13};
    perm p2 = perm_ar16(p2_arr);

    perm r;
    int b;

    // Utilisation de _mm_shuffle_epi8
    cout << "Permuter les valeurs de p1 selon p2" << endl;
    cout << "On permute les valeurs de " << endl << p1 << endl;
    cout << "selon " << endl << p2 << endl;
    cout << "on obtient :" << endl <<  _mm_shuffle_epi8(p1, p2) << endl << endl;

    // Comparaison terme à terme
    cout << "On peut comparer les valeurs des permutations terme à terme" << endl;
    cout << p1 << " < " << endl << p2 << endl;
    cout << (p1 < p2) << endl;
    cout << p1 << " == " << endl << p2 << endl;
    cout << (p1 == p2) << endl;
    cout << endl;

    // masques et comptage de 1
    cout << "Le résultat de cette opération peut être traité grâce à un masque" << endl;
    cout << p1 << " < " << endl << p2 << endl;
    r = (p1 < p2);
    cout << r << endl;
    b = _mm_movemask_epi8(r);
    cout << "Application du masque : on obtient un nombre dont les 1 binaires représentent les positions non nulles : " << hex << unsigned(b) << endl;
    cout << "On compte les 1 avec une opération du processeur" << endl;
    cout << _mm_popcnt_u32(b) << endl;

    // Calcul du nombre de descentes
    // cout << "Nombre de de descentes de " << endl;
    // cout << p1 << endl;
    // cout << "Algorithme classique" << endl;
    // cout << nb_descents_classic(p1_arr) << endl;
    // cout << "Algorithme optimisé processeur" << endl;
    // cout << nb_descents_optim(p1) << endl;

    // cout << "Lancement des tests sur permutations aléatoires" << endl;
    // test_nb_descents_optim(100);

}
