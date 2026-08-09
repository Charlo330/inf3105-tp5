/**
 * algographe.h
 * TP5 -- Algorithmes des graphes
 * UQAM / Departement d'informatique
 * INF3105 - Structures de donnees et algorithmes
 */

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "graphe.h"

// Enleve les retours de chariot ('\r', '\n') a la fin d'une ligne lue dans le fichier.
std::string nettoyerLigne(std::string s) {
    while (!s.empty() && (s[s.size() - 1] == '\r' || s[s.size() - 1] == '\n'))
        s.erase(s.size() - 1);
    return s;
}

Graphe<std::string> ChargerGraphe(const std::string& fichier) {
    Graphe<std::string> oGraphe;

    std::ifstream in(fichier);
    if (!in) {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
        exit(1);
    }

    std::string ligne;
    std::getline(in, ligne);
    int nbSommets = std::stoi(nettoyerLigne(ligne));

    for (int i = 0; i < nbSommets && std::getline(in, ligne); i++)
        oGraphe.ajouterSommet(nettoyerLigne(ligne));

    while (std::getline(in, ligne)) {
        ligne = nettoyerLigne(ligne);
        std::istringstream iss(ligne);
        std::string s1, s2;
        if (iss >> s1 >> s2)
            oGraphe.ajouterAreteNonOrientee(s1, s2);
    }

    return oGraphe;
}

void afficherGraphe(const Graphe<std::string>& graphe) {
    std::cout << "\n--- Graphe ---\n";
    std::map<std::string, Graphe<std::string>::Sommet>::const_iterator it;
    for (it = graphe.sommets.begin(); it != graphe.sommets.end(); ++it) {
        std::cout << std::left << std::setw(10) << it->first << ":";
        size_t i = 0, n = it->second.voisins.size();
        std::set<std::string>::const_iterator jt;
        for (jt = it->second.voisins.begin(); jt != it->second.voisins.end(); ++jt) {
            std::cout << " " << *jt;
            i++;
            if (i != n) std::cout << ",";
        }
        std::cout << "  \n";
    }
}

// Extraction des composantes connexes par recherche en largeur a partir de
// chaque sommet non encore visite (voir section 13.2.1 des notes de cours).
void composantesConnexes(const Graphe<std::string>& graphe) {
    std::cout << "\n--- Communautes detectees ---\n";

    std::map<std::string, int> composante;
    int numero = 0;

    std::map<std::string, Graphe<std::string>::Sommet>::const_iterator it;
    for (it = graphe.sommets.begin(); it != graphe.sommets.end(); ++it) {
        const std::string& depart = it->first;
        if (composante.count(depart)) continue;
        numero++;

        std::queue<std::string> file;
        file.push(depart);
        composante[depart] = numero;
        while (!file.empty()) {
            std::string courant = file.front();
            file.pop();
            std::set<std::string>::const_iterator jt;
            const std::set<std::string>& voisins = graphe.sommets.at(courant).voisins;
            for (jt = voisins.begin(); jt != voisins.end(); ++jt) {
                if (!composante.count(*jt)) {
                    composante[*jt] = numero;
                    file.push(*jt);
                }
            }
        }
    }

    for (int n = 1; n <= numero; n++) {
        std::cout << "Communaute " << n << " : ";
        bool premier = true;
        std::map<std::string, int>::const_iterator jt;
        for (jt = composante.begin(); jt != composante.end(); ++jt) {
            if (jt->second != n) continue;
            if (!premier) std::cout << ", ";
            std::cout << jt->first;
            premier = false;
        }
        std::cout << "\n";
    }
}

// Plus court chemin par recherche en largeur (Algorithme 4 des notes de cours) :
// dans un graphe sans poids, la recherche en largeur trouve le chemin le plus
// court en nombre d'aretes.
void plusCourtChemin(const Graphe<std::string>& graphe, const std::string& debut, const std::string& fin) {
    std::cout << "\n--- Plus court chemin de " << debut << " a " << fin << " ---\n";

    if (!graphe.sommets.count(debut) || !graphe.sommets.count(fin)) {
        std::cout << "Sommet inexistant.\n";
        return;
    }

    graphe.reinitVisite();
    std::map<std::string, std::string> parent;
    std::queue<std::string> file;
    graphe.sommets.at(debut).visite = true;
    file.push(debut);
    bool trouve = (debut == fin);

    while (!file.empty() && !trouve) {
        std::string courant = file.front();
        file.pop();
        std::set<std::string>::const_iterator jt;
        const std::set<std::string>& voisins = graphe.sommets.at(courant).voisins;
        for (jt = voisins.begin(); jt != voisins.end(); ++jt) {
            if (!graphe.sommets.at(*jt).visite) {
                graphe.sommets.at(*jt).visite = true;
                parent[*jt] = courant;
                if (*jt == fin) { trouve = true; break; }
                file.push(*jt);
            }
        }
    }

    if (!trouve) {
        std::cout << "Aucun chemin trouve.\n";
        return;
    }

    std::vector<std::string> chemin;
    std::string courant = fin;
    while (courant != debut) {
        chemin.push_back(courant);
        courant = parent[courant];
    }
    chemin.push_back(debut);
    std::reverse(chemin.begin(), chemin.end());

    for (size_t i = 0; i < chemin.size(); i++) {
        std::cout << chemin[i];
        if (i + 1 != chemin.size()) std::cout << " -> ";
    }
    std::cout << " (distance : " << chemin.size() - 1 << ")\n";
}

// Recherche en profondeur pour trouver les points d'articulation. Pour chaque
// sommet, decouverte[] donne son ordre de visite et bas[] donne le plus petit
// ordre de decouverte atteignable en remontant au plus une arete de retour.
// Un sommet u (non racine) est un point d'articulation si un de ses enfants v
// dans l'arbre de recherche ne peut pas atteindre un ancetre de u sans repasser
// par u, c'est-a-dire si bas[v] >= decouverte[u]. La racine est un point
// d'articulation si elle a plus d'un enfant dans l'arbre de recherche.
void dfsArticulation(const Graphe<std::string>& graphe, const std::string& u, const std::string& parent,
                      std::map<std::string, int>& decouverte, std::map<std::string, int>& bas,
                      int& compteur, std::set<std::string>& articulation) {
    decouverte[u] = compteur;
    bas[u] = compteur;
    compteur++;
    int enfants = 0;

    std::set<std::string>::const_iterator it;
    const std::set<std::string>& voisins = graphe.sommets.at(u).voisins;
    for (it = voisins.begin(); it != voisins.end(); ++it) {
        const std::string& v = *it;
        if (v == parent) continue;
        if (!decouverte.count(v)) {
            enfants++;
            dfsArticulation(graphe, v, u, decouverte, bas, compteur, articulation);
            bas[u] = std::min(bas[u], bas[v]);
            if (!parent.empty() && bas[v] >= decouverte[u])
                articulation.insert(u);
        } else {
            bas[u] = std::min(bas[u], decouverte[v]);
        }
    }

    if (parent.empty() && enfants > 1)
        articulation.insert(u);
}

void pointsArticulation(const Graphe<std::string>& graphe) {
    std::cout << "\n--- Points d'articulation ---\n";

    std::map<std::string, int> decouverte, bas;
    std::set<std::string> articulation;
    int compteur = 0;

    std::map<std::string, Graphe<std::string>::Sommet>::const_iterator it;
    for (it = graphe.sommets.begin(); it != graphe.sommets.end(); ++it) {
        if (!decouverte.count(it->first))
            dfsArticulation(graphe, it->first, "", decouverte, bas, compteur, articulation);
    }

    std::set<std::string>::const_iterator jt;
    for (jt = articulation.begin(); jt != articulation.end(); ++jt)
        std::cout << *jt << "\n";
}
