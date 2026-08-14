/**
 * Charles-Antoine Lanthier
 * LANC70040208
 */
/*  INF3105 - Structures de données et algorithmes
    Lab 9 Partie 1 -- Graphes / Représentations et algorithmes de base
*/
#ifndef GRAPHE_H
#define GRAPHE_H

#include <cassert>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>

// Le type S est le symbole pour identifier un sommet
template <class S>
class Graphe {
  public:
    void ajouterSommet(const S& s);
    void ajouterAreteOrientee(const S& s1, const S& s2);
    void ajouterAreteNonOrientee(const S& s1, const S& s2);

    void parcoursRechercheProfondeur(const S& s) const;
    void parcoursRechercheProfondeurIter(const S& s) const;
    void parcoursRechercheLargueur(const S& s) const;
    void extraireComposantesConnexes() const;
    void reinitVisite() const;

    struct Sommet {
        mutable bool visite = false;
        std::set<S> voisins;
    };
    std::map<S, Sommet> sommets;

  private:
    void parcoursProfondeur(const S& s) const;
    void parcoursLargueur(const S& s) const;
};

template <class S>
void Graphe<S>::ajouterSommet(const S& s) {
    assert(sommets.count(s) == 0);
    sommets[s];
}

template <class S>
void Graphe<S>::ajouterAreteOrientee(const S& s1, const S& s2) {
    assert(sommets.count(s1) == 1 && sommets.count(s2) == 1);
    sommets[s1].voisins.insert(s2);
}

template <class S>
void Graphe<S>::ajouterAreteNonOrientee(const S& s1, const S& s2) {
    ajouterAreteOrientee(s1, s2);
    ajouterAreteOrientee(s2, s1);
}

template <class S>
void Graphe<S>::reinitVisite() const {
    typename std::map<S, Sommet>::const_iterator it;
    for (it = sommets.begin(); it != sommets.end(); ++it)
        it->second.visite = false;
}

template <class S>
void Graphe<S>::parcoursProfondeur(const S& s) const {
    const Sommet& sommet = sommets.at(s);
    if (sommet.visite) return;

    sommet.visite = true;
    std::cout << s << " ";

    typename std::set<S>::const_iterator it;
    for (it = sommet.voisins.begin(); it != sommet.voisins.end(); ++it)
        parcoursProfondeur(*it);
}

template <class S>
void Graphe<S>::parcoursRechercheProfondeur(const S& s) const {
    reinitVisite();
    parcoursProfondeur(s);
    std::cout << std::endl;
}

template <class S>
void Graphe<S>::parcoursRechercheProfondeurIter(const S& s) const {
    reinitVisite();
    std::stack<S> pile;
    pile.push(s);

    while (!pile.empty()) {
        S courant = pile.top();
        pile.pop();
        const Sommet& sommet = sommets.at(courant);
        if (sommet.visite) continue;

        sommet.visite = true;
        std::cout << courant << " ";

        typename std::set<S>::const_iterator it;
        for (it = sommet.voisins.begin(); it != sommet.voisins.end(); ++it)
            pile.push(*it);
    }
    std::cout << std::endl;
}

template <class S>
void Graphe<S>::parcoursLargueur(const S& s) const {
    std::queue<S> file;
    sommets.at(s).visite = true;
    file.push(s);

    while (!file.empty()) {
        S courant = file.front();
        file.pop();
        std::cout << courant << " ";

        typename std::set<S>::const_iterator it;
        for (it = sommets.at(courant).voisins.begin(); it != sommets.at(courant).voisins.end(); ++it) {
            if (!sommets.at(*it).visite) {
                sommets.at(*it).visite = true;
                file.push(*it);
            }
        }
    }
}

template <class S>
void Graphe<S>::parcoursRechercheLargueur(const S& s) const {
    reinitVisite();
    parcoursLargueur(s);
    std::cout << std::endl;
}

template <class S>
void Graphe<S>::extraireComposantesConnexes() const {
    reinitVisite();

    typename std::map<S, Sommet>::const_iterator it;
    for (it = sommets.begin(); it != sommets.end(); ++it) {
        if (it->second.visite) continue;
        parcoursLargueur(it->first);
        std::cout << std::endl;
    }
}

#endif
