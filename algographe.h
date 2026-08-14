/**
 * Charles-Antoine Lanthier
 * LANC70040208
 * 
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

Graphe<std::string> ChargerGraphe(const std::string &fichier)
{
    Graphe<std::string> oGraphe;

    std::ifstream in(fichier);
    if (!in)
    {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
        exit(1);
    }

    std::string ligne;
    std::getline(in, ligne);
    int nbSommets = std::stoi(ligne);

    for (int i = 0; i < nbSommets && std::getline(in, ligne); i++)
        oGraphe.ajouterSommet(ligne);

    while (std::getline(in, ligne))
    {
        std::istringstream iss(ligne);
        std::string s1, s2;
        if (iss >> s1 >> s2)
            oGraphe.ajouterAreteNonOrientee(s1, s2);
    }

    return oGraphe;
}

void afficherGraphe(const Graphe<std::string> &graphe)
{
    std::cout << "\n--- Graphe ---\n";

    // on itere sur tous les sommets
    std::map<std::string, Graphe<std::string>::Sommet>::const_iterator it_sommet;
    for (it_sommet = graphe.sommets.begin(); it_sommet != graphe.sommets.end(); ++it_sommet)
    {
        std::cout << it_sommet->first << " :";

        // on itere sur les voisins du sommet
        size_t i = 0, n = it_sommet->second.voisins.size();
        std::set<std::string>::const_iterator it_voisin;
        for (it_voisin = it_sommet->second.voisins.begin(); it_voisin != it_sommet->second.voisins.end(); ++it_voisin)
        {
            std::cout << " " << *it_voisin;
            i++;
            if (i != n)
                std::cout << ",";
        }
        std::cout << "\n";
    }
}

void composantesConnexes(const Graphe<std::string> &graphe)
{
    std::cout << "\n--- Communautes detectees ---\n";

    std::map<std::string, int> composante;
    int id_composante = 0;

    // on itere sur les sommets
    for (std::map<std::string, Graphe<std::string>::Sommet>::const_iterator it_sommet = graphe.sommets.begin(); it_sommet != graphe.sommets.end(); ++it_sommet)
    {
        const std::string &depart = it_sommet->first;
        if (!composante.count(depart))
        {
            id_composante++;

            std::queue<std::string> file;
            file.push(depart);
            composante[depart] = id_composante;

            while (!file.empty())
            {
                std::string courant = file.front();
                file.pop();

                // on itere sur les voisins du sommet
                const std::set<std::string> &voisins = graphe.sommets.at(courant).voisins;
                for (std::set<std::string>::const_iterator it_voisin = voisins.begin(); it_voisin != voisins.end(); ++it_voisin)
                {
                    if (!composante.count(*it_voisin))
                    {
                        composante[*it_voisin] = id_composante;
                        file.push(*it_voisin);
                    }
                }
            }
        }
    }

    for (int n = 1; n <= id_composante; n++)
    {
        std::cout << "Communaute " << n << " : ";
        bool premiere_composante = true;

        for (std::map<std::string, int>::const_iterator it = composante.begin(); it != composante.end(); ++it)
        {
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

void plusCourtChemin(const Graphe<std::string> &graphe, const std::string &debut, const std::string &fin)
{
    std::cout << "\n--- Plus court chemin de " << debut << " a " << fin << " ---\n";

    if (!graphe.sommets.count(debut) || !graphe.sommets.count(fin))
    {
        std::cout << "Sommet inexistant.\n";
        return;
    }

    graphe.reinitVisite();
    std::map<std::string, std::string> parent;
    std::queue<std::string> file;
    graphe.sommets.at(debut).visite = true;
    file.push(debut);
    bool trouve = (debut == fin);

    while (!file.empty() && !trouve)
    {
        std::string courant = file.front();
        file.pop();

        const std::set<std::string> &voisins = graphe.sommets.at(courant).voisins;
        for (std::set<std::string>::const_iterator it_voisin = voisins.begin(); it_voisin != voisins.end(); ++it_voisin)
        {
            if (!graphe.sommets.at(*it_voisin).visite)
            {
                graphe.sommets.at(*it_voisin).visite = true;
                parent[*it_voisin] = courant;
                if (*it_voisin == fin)
                {
                    trouve = true;
                    break;
                }
                file.push(*it_voisin);
            }
        }
    }

    if (!trouve)
    {
        std::cout << "Aucun chemin trouve.\n";
        return;
    }

    std::vector<std::string> chemain;
    std::string courant = fin;
    while (courant != debut)
    {
        chemain.push_back(courant);
        courant = parent[courant];
    }
    chemain.push_back(debut);
    std::reverse(chemain.begin(), chemain.end());

    for (size_t i = 0; i < chemain.size(); i++)
    {
        std::cout << chemain[i];
        if (i + 1 != chemain.size())
            std::cout << " -> ";
    }
    std::cout << " (distance : " << chemain.size() - 1 << ")\n";
}

void pointsArticulation(const Graphe<std::string> &graphe)
{
    std::cout << "\n--- Points d'articulation ---\n";

    for (std::map<std::string, Graphe<std::string>::Sommet>::const_iterator it_sommet = graphe.sommets.begin(); it_sommet != graphe.sommets.end(); ++it_sommet)
    {
        const std::string &sommet = it_sommet->first;
        const std::set<std::string> &voisins = it_sommet->second.voisins;

        if (voisins.size() >= 2)
        {

            graphe.reinitVisite();
            graphe.sommets.at(sommet).visite = true;

            int nbVisite = 0;

            for (std::set<std::string>::const_iterator it_voisin = voisins.begin(); it_voisin != voisins.end(); ++it_voisin)
            {
                if (!graphe.sommets.at(*it_voisin).visite)
                {
                    nbVisite++;
                    std::queue<std::string> file;

                    graphe.sommets.at(*it_voisin).visite = true;
                    file.push(*it_voisin);

                    while (!file.empty())
                    {
                        std::string courant = file.front();
                        file.pop();

                        const std::set<std::string> &voisinsCourant = graphe.sommets.at(courant).voisins;

                        for (std::set<std::string>::const_iterator it_voisin_voisin = voisinsCourant.begin();
                             it_voisin_voisin != voisinsCourant.end(); ++it_voisin_voisin)
                        {
                            if (!graphe.sommets.at(*it_voisin_voisin).visite)
                            {
                                graphe.sommets.at(*it_voisin_voisin).visite = true;
                                file.push(*it_voisin_voisin);
                            }
                        }
                    }
                }
            }

            if (nbVisite > 1)
                std::cout << sommet << "\n";
        }
    }
}
