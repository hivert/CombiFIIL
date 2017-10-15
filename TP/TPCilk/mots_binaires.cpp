#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_opadd.h>
#include <x86intrin.h>
#include <array>
#include <iostream>

using namespace std;

#define maxwords 32


/** Tableau cilk pour stockage du resultat **/
array<cilk::reducer_opadd<unsigned long int>, maxwords+1 > res;


/** Calcul du nombre de bits à 1
 * @param un entier non signé sur 32 bits
 * @return le nombre de bits à 1
 **/
int nb1(uint32_t i) {
    return _mm_popcnt_u32(i); // commande processeur
}

/** Génération récursive classique des mots binaires avec comptage des 1
 * @param n entier : la taille du mot souhaitée
 * @param cur entier : la taille du mot courant
 * @param i entier : le mot binaire courant stocké sous forme d'un entier
 **/
void motsbin_classique(int n, int cur, int i) {
    if (cur == n) {
        // Condition d'arret : le mot courant a atteint la taille n
        // On compte le nombre de bits à 1 dans i et on le stocke dans le tableau resultat
        res[nb1(i)]++;
    }
    else{
        // Appels récursifs
        motsbin_classique(n, cur+1, i << 1); // on décale le mot d'un cran à gauche et on ajoute 0
        motsbin_classique(n, cur+1, (i << 1) + 1); // on décale le mot d'un cra à droite et on ajoute 1
    }
}

/** Génération récursive en parrallèle des mots binaires avec comptage des 1
 * @param n entier : la taille du mot souhaitée
 * @param cur entier : la taille du mot courant
 * @param i entier : le mot binaire courant stocké sous forme d'un entier
 **/
void motsbin_parral(int n, int cur, int i) {
    /// A compléter
}

void show_usage(string name) {
    cerr << "Usage: " << name << " [-n <proc_number>] size " << endl;
}

int main(int argc, char *argv[]) {
    int n;
    int nproc = 1;

    /** Récupération des arguments **/
    if (argc != 2 and argc != 4) {
        // Mauvaise utilisation du programme (pas assez d'arguments)
        show_usage(argv[0]);
        return 1;
    }
    if (argc == 4) {
        if (string(argv[1]) != "-n")  {
            // Mauvaise utilisation du programme (mauvais argument)
            show_usage(argv[0]);
            return 1;
        }

        // Récupération du nombre de processeurs à utiliser et configuration de CILK
        string nprocstr = string(argv[2]);
        nproc = atoi(argv[2]);
        if (__cilkrts_set_param("nworkers", nprocstr.c_str()) != __CILKRTS_SET_PARAM_SUCCESS) {
            cerr << "Failed to set the number of Cilk workers" << endl;
        }
    }
    // Récupération du paramètre (taille des mots)
    n = atoi(argv[argc - 1]);
    if (n > maxwords) {
        cerr << "Exceeds max word size" << endl;
        return 1;
    }



    /** Calcul **/
    if (nproc == 1) {
        motsbin_classique(n, 0, 0);
    } else {
        motsbin_parral(n, 0, 0);
    }

    /** Affichage du Résultat **/
    cout << "Result: ";
    for(int i = 0; i <= n; i++){
        cout << res[i].get_value() << " ";
    }
    cout << endl;
    return 0;
}
