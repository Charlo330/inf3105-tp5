/**
 * algographe.h
 * TP4 -- Algorithme des graphes
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <string>
#include "graphe.h"

Graphe<std::string> ChargerGraphe(const std::string& fichier) {
    // Implémentation de la generation du graphe à partir du fichier
    Graphe<std::string> oGraphe;

    std::ifstream in(fichier);
    if (!in) {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
        exit(1);
    }
    //....

    return oGraphe;
}

void afficherGraphe(const Graphe<std::string>& graphe) {
    // Implémentation de l'affichage du graphe
    std::cout << "\n--- Graphe ---\n";
    // ...
}

void composantesConnexes(const Graphe<std::string>& graphe) {
    // Implémentation de l'algorithme pour trouver les composantes connexes (communautes isolées)
    std::cout << "\n--- Communautes detectees ---\n";
    // ...
}

void plusCourtChemin(const Graphe<std::string>& graphe, const std::string& debut, const std::string& fin) {
    // Implémentation de l'algorithme pour trouver le plus court chemin entre deux sommets
    std::cout << "\n--- Plus court chemin de " << debut << " a " << fin << " ---\n";
    // ...
}

void pointsArticulation(const Graphe<std::string>& graphe) {
    // Implémentation de l'algorithme pour trouver les points d'articulation
    std::cout << "\n--- Points d'articulation ---\n";
    // ...
}
