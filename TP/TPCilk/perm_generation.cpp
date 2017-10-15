#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_opadd.h>
#include <x86intrin.h>
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
using perm = uint8_t __attribute__ ((vector_size (16)));


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


/** Retourne le nombre de descentes d'une permutation en utilisant les commandes
 * du processeur sur les données vectorielles
 * @param p une permutation en donnée vectorielle
 * @return le nombre de descente de la permutation
 **/
int nb_descents_optim(perm p) {
    /// Complétez la fonction
    return 0;
}

/***********************************************************************/
/************* Génération réursive des permutations ********************/
/***********************************************************************/

/** double tableau 16x16 de toutes les transpositions **/
array<array<perm, 16>, 16> transp;

/** Initialise la varaible globale trans du tableau des transpositions **/
void init_transp() {
    for (int i = 0; i<16; i++) {
        for (int j = 0; j<16; j++) {
            transp[i][j] = perm_ar16(transposition(i,j));
        }
    }
}

/** Applique la transpotion i,j à une permutation
 * @param p une permutation de type vectorielle perm
 * @param i un entier entre 0 et 15
 * @param j un entier entre 0 et 15
 * @return la permutation où les valeurs en position i et j ont été échangées
 **/
perm permuteij(perm p, int i, int j) {
    return _mm_shuffle_epi8(p, transp[i][j]);
}

/** Tableau cilk pour stockage du resultat **/
array<cilk::reducer_opadd<unsigned long int>, 16 > res;

/** Affichage des permutations par Génération récursive
 * @param n la taille des permutations à engendrer
 * @param p la permutation  courante de type perm
 **/
void affiche_all_permutations(int n, perm p) {
    /// Complétez la fonction
}

/** Génération récursive des permutations avec calcul du nombre de descentes
 * @param n la taille des permutations à engendrer
 * @param p la permutation  courante de type perm
 **/
void descentes_all_permutations_classique(int n, perm p) {
    /// Complétez la fonction
}


/** Génération récursive des permutations en parallèle avec calcul du nombre de descentes
 * @param n la taille des permutations à engendrer
 * @param p la permutation  courante de type perm
 **/
void descentes_all_permutations_parall(int n, perm p) {
    /// Complétez la fonction
}

/***********************************************************************/
/******************** Fonction principale ******************************/
/***********************************************************************/

void show_usage(string name) {
    cerr << "Usage: " << name << " [-a] [-n <proc_number>] size " << endl;
}

int main(int argc, char *argv[]) {
    int n;
    int nproc = 1;
    bool affichage = false;

    /** Récupération des arguments **/
    if (argc < 2) {
        // Mauvaise utilisation du programme (pas assez d'arguments)
        show_usage(argv[0]);
        return 1;
    }
    if (argc > 2) {
        if (argc == 4 and string(argv[1]) == "-n")  {
            // Récupération du nombre de processeurs à utiliser et configuration de CLIK
            string nproc = string(argv[2]);
            nproc = atoi(argv[2]);
            if (__cilkrts_set_param("nworkers", nproc.c_str()) != __CILKRTS_SET_PARAM_SUCCESS) {
                cerr << "Failed to set the number of Cilk workers" << endl;
            }
        } else if( argc == 3 and string(argv[1]) == "-a") {
            // Mode affichage
            affichage = true;
        } else {
            // Mauvaise utilisation du programme (mauvais arguments)
            show_usage(argv[0]);
            return 1;
        }
    }
    // Récupération du paramètre (taille des permutations)
    n = atoi(argv[argc - 1]);

    // Initialisation du tableau de transpositions
    init_transp();

    if (affichage) affiche_all_permutations(n, permid);
    else {
        /** Calcul **/
        if (nproc == 1) {
            descentes_all_permutations_classique(n, permid);
        } else {
            descentes_all_permutations_parall(n, permid);
        }

        /** Affichage du Résultat **/
        cout << "Result: ";
        for(int i = 0; i < 16; i++)
            cout << res[i].get_value() << " ";
        cout << endl;

    }

    return 0;

}
