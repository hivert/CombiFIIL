#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cstdint>
#include <iostream>
#include <string>

using namespace std;

int f() { return 1; }

int rec(int prof, int trg) {
  if (prof == trg) return f();
  else {
    int x = cilk_spawn rec(prof+1, trg);
    int y = cilk_spawn rec(prof+1, trg);
    cilk_sync;
    return x + y;
  }
}

void show_usage(string name) {
    cerr << "Usage: " << name << " [-n <proc_number>] size " << endl;
}

int main(int argc, char *argv[]) {
  int n;

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
    if (__cilkrts_set_param("nworkers", nprocstr.c_str()) != __CILKRTS_SET_PARAM_SUCCESS) {
      cerr << "Failed to set the number of Cilk workers" << endl;
    }
  }
  // Récupération du paramètre (taille des mots)
  n = atoi(argv[argc - 1]);

  std::cout << rec(0, n) << std::endl;
  return 1;
}
