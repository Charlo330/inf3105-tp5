/**
 * main.cpp
 * TP4 -- Analyse d'un Réseau Social (Algorithme des graphes)
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes
 */


#include "algographe.h"


int main(int argc, char* argv[]) {

    std::string inputFileName;
    std::cout << ("Le nom du programme est: %s", argv[0]);

    if (argc == 1) {
        std::cout << ("\nAucun argument en parametres (utilise input.txt pour lire) ");
        inputFileName = std::string("input.txt");

    }
    else if (argc >= 2) {
        std::cout << ("\nNombre d'arguments passes: %d", argc);
        std::cout << ("\n----La commande passee est ");
        for (int i = 0; i < argc; i++)
            std::cout << ("\nargv[%d]: %s", i, argv[i]);
        inputFileName = std::string(argv[1]);
    }
    else
    {
        std::cout << ("\nCe programme prend un parametre dont un nom de fichier pour lire les donnees du graphe ");
    }

    std::cout << std::endl;

    int choix;
    std::string debut, fin;
    Graphe<std::string> graphe = ChargerGraphe(inputFileName);

    do {
        std::cout << "\n--- MENU ---\n";
        std::cout << "1. Afficher graphe\n";
        std::cout << "2. Afficher les composantes connexes (communautes isolees)\n";
        std::cout << "3. Afficher les plus court chemin entre deux sommets\n";
        std::cout << "4. Afficher les points de articulation\n";
        std::cout << "0. Quitter\n";
        std::cout << "Choix : ";
        std::cin >> choix;
        std::cin.ignore();

        switch (choix) {
        case 1:
			afficherGraphe(graphe);
            break;
        case 2:
			composantesConnexes(graphe);
            break;
        case 3:
            std::cout << "Debut : "; 
            getline(std::cin, debut);
            std::cout << "Fin : ";
            getline(std::cin, fin);
			plusCourtChemin(graphe, debut, fin);
            break;
        case 4:
			pointsArticulation(graphe);
            break;
        case 0:
            std::cout << "Fin du programme.\n";
            break;
        default:
            std::cout << "Choix invalide.\n";
        }
    } while (choix != 0);

    return 0;
}
