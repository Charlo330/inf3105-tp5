/**
 * algographe.h
 * TP5 -- Algorithmes des graphes
 * UQAM / Departement d'informatique
 * INF3105 - Structures de donnees et algorithmes
 */

#include <algorithm>
#include <fstream>
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
    // on itere sur tous les sommets
    std::map<std::string, Graphe<std::string>::Sommet>::const_iterator it_sommet;
    for (it_sommet = graphe.sommets.begin(); it_sommet != graphe.sommets.end(); ++it_sommet) {
        std::cout << it_sommet->first << " :";
        // on itere sur les voisins du sommet
        size_t i = 0, n = it_sommet->second.voisins.size();
        std::set<std::string>::const_iterator it_voisin;
        for (it_voisin = it_sommet->second.voisins.begin(); it_voisin != it_sommet->second.voisins.end(); ++it_voisin) {
            std::cout << " " << *it_voisin;
            i++;
            if (i != n)
                std::cout << ",";
        }
        std::cout << "\n";
    }
}

void composantesConnexes(const Graphe<std::string>& graphe) {
    std::cout << "\n--- Communautes detectees ---\n";

    std::map<std::string, int> composante;
    int numero = 0;

    // on itere sur les sommets
    std::map<std::string, Graphe<std::string>::Sommet>::const_iterator it;
    for (it = graphe.sommets.begin(); it != graphe.sommets.end(); ++it) {
        const std::string& depart = it->first;
        if (composante.count(depart))
            continue;
        numero++;

        std::queue<std::string> file;
        file.push(depart);
        composante[depart] = numero;

        while (!file.empty()) {
            std::string courant = file.front();
            file.pop();
            
            // on itere sur les voisins du sommet
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
        bool premiere_composante = true;

        std::map<std::string, int>::const_iterator it;
        for (it = composante.begin(); it != composante.end(); ++it) {
            if (it->second != n) 
                continue;
            if (!premiere_composante) 
                std::cout << ", ";
            std::cout << it->first;
            premiere_composante = false;
        }

        std::cout << "\n";
    }
}

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
        std::set<std::string>::const_iterator it;
        const std::set<std::string>& voisins = graphe.sommets.at(courant).voisins;
        for (it = voisins.begin(); it != voisins.end(); ++it) {
            if (!graphe.sommets.at(*it).visite) {
                graphe.sommets.at(*it).visite = true;
                parent[*it] = courant;
                if (*it == fin) { trouve = true; break; }
                file.push(*it);
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

void pointsArticulation(const Graphe<std::string>& graphe) {
    std::cout << "\n--- Points d'articulation ---\n";

    std::map<std::string, Graphe<std::string>::Sommet>::const_iterator it;
    for (it = graphe.sommets.begin(); it != graphe.sommets.end(); ++it) {
        const std::string& sommet = it->first;
        const std::set<std::string>& voisins = it->second.voisins;

        // Un sommet avec 0 ou 1 voisin ne peut relier personne : inutile de le tester.
        if (voisins.size() < 2) continue;

        graphe.reinitVisite();
        graphe.sommets.at(sommet).visite = true; // on "enleve" le sommet

        int nbMorceaux = 0;
        std::set<std::string>::const_iterator jt;
        for (jt = voisins.begin(); jt != voisins.end(); ++jt) {
            if (graphe.sommets.at(*jt).visite) continue;

            nbMorceaux++;

            std::queue<std::string> file;
            graphe.sommets.at(*jt).visite = true;
            file.push(*jt);
            while (!file.empty()) {
                std::string courant = file.front();
                file.pop();
                std::set<std::string>::const_iterator kt;
                const std::set<std::string>& voisinsCourant = graphe.sommets.at(courant).voisins;
                for (kt = voisinsCourant.begin(); kt != voisinsCourant.end(); ++kt) {
                    if (!graphe.sommets.at(*kt).visite) {
                        graphe.sommets.at(*kt).visite = true;
                        file.push(*kt);
                    }
                }
            }
        }

        if (nbMorceaux > 1)
            std::cout << sommet << "\n";
    }
}
