// Projet.cpp : définit le point d'entrée de l'application.
//

#include "Projet.h"

bool running = true; // Contrôle si les threads doivent continuer à tourner
mutex objets_mutex; // Définit l'instance globale du mutex
mutex feu_mutex;    // Définit l'instance globale du mutex


// Fonction de gestion des déplacements (thread)
void thread_deplacement() { //Ce thread gère le comportement de TOUS les acteurs encore présents sur la carte
    while (running) {
        {
            lock_guard<mutex> lock(objets_mutex); // Protéger l'accès à `objets`
            deplacement();
        }
        this_thread::sleep_for(chrono::milliseconds(100)); // On ajute à 100 pour donner un peu le temps de voir ce qu'il se passe, sachant que plus on attend, plus d'objets ont le temps de spawn
    }
}

// Fonction de gestion des feux (thread)
void thread_gestion_feux() {
    while (running) {
        {
            lock_guard<mutex> lock(feu_mutex); // Protéger l'accès à `etat_feu` et `cpt`
            gestion_feux(cpt, etat_feu);
        }
        this_thread::sleep_for(chrono::milliseconds(33)); // Pour avoir environ 30 fps
    }
}

int main()
{
    window.setFramerateLimit(30);

    // Lancer les threads
    thread deplacement_thread(thread_deplacement);
    thread feux_thread(thread_gestion_feux);

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                running = false; // Arrêter les threads
            }
        }



        window.clear(Color::White);

        
        generation_voitures();
        generation_pietons();
        generation_bus();
        generation_velos();
        {
            lock_guard<mutex> lock(objets_mutex); // Protéger les accès aux objets pour le rendu
            gestion_couleur();
            dessin_feux(etat_feu);
        }

        window.display();
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    // Attendre la fin des threads
    if (deplacement_thread.joinable()) deplacement_thread.join();
    if (feux_thread.joinable()) feux_thread.join();

    window.close();
    return 0;
}