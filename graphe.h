/*  INF3105 - Structures de données et algorithmes
    Lab 11 -- Graphes / Représentations et algorithmes de base
    http://cria2.uqam.ca/INF3105/lab11/
*/
#ifndef GRAPHE_H
#define GRAPHE_H

#include <map>
#include <set>

// Le type S est le symbole pour identifier un sommet
template<class S>
class Graphe {
 public:
  // Interface public pour créer le graphe.
  void ajouterSommet(const S& s);
  void ajouterAreteOrientee(const S& s1, const S& s2);
  void ajouterAreteNonOrientee(const S& s1, const S& s2);

  // Interface public pour les requêtes de base.
  // Amélioration possible: retourner le résultat au lieu de faire l'affichage à l'interne.
  void parcoursRechercheProfondeur(const S& s) const;
  void parcoursRechercheLargueur(const S& s) const;
  void extraireComposantesConnexes() const;

  struct Sommet {
    // À compléter.
    std::set<S> voisins;  // ensemble des sommets accessibles via les arêtes
                          // sortantes du sommet. Cela est légèrement différent
                          // de la page 128 des notes de cours. C'est voulu, car
                          // ici les arêtes ne sont pas étiquetées par un poids
                          // (ex: distance). Pour attacher une étiquette, il
                          // suffirait de modifier pour : map<S, A> sortants;
  };
  std::map<S, Sommet> sommets;  // identification --> sommet
};

template<class S>
void Graphe<S>::ajouterSommet(const S& s) {
  // À compléter.
}

template<class S>
void Graphe<S>::ajouterAreteNonOrientee(const S& s1, const S& s2) {
  // À compléter.
  // Doit ajouter les 2 arêtes orientées : s1->s2 et s2->s1;
}

template<class S>
void Graphe<S>::ajouterAreteOrientee(const S& s1, const S& s2) {
  // À compléter.
  // Doit uniquement ajouter s1->s2.
}

template<class S>
void Graphe<S>::parcoursRechercheProfondeur(const S& s) const {
  // À compléter.
  // Vous devrez implémenter des marqueurs «visité» sur les sommets.
}

template<class S>
void Graphe<S>::parcoursRechercheLargueur(const S& s) const {
  // À compléter.
  // Vous devrez implémenter des marqueurs «visité» sur les sommets.
  // Il est recommendé d'utiliser la file std::queue de la bibliothèque standard
  // de C++. Voir : http://en.cppreference.com/w/cpp/container/queue
}
template<class S>
void Graphe<S>::extraireComposantesConnexes() const {
  // À compléter.
  // Ne faites que l'extraction simple en premier (plus simple que l'extraction
  // de composantes *fortement* connexes).
}

#endif
