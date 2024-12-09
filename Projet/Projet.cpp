// Projet.cpp : définit le point d'entrée de l'application.
//

#include "Projet.h"

int main()
{

    window.setFramerateLimit(30);

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }



        window.clear(Color::White);

        gestion_couleur();
        gestion_feux(cpt, etat_feu);
        cpt++; //On passe 1 itération
        generation_voitures();
        objets_mutex.lock();
        deplacement();
        objets_mutex.unlock();

        window.display();
        this_thread::sleep_for(chrono::seconds(1));
    }

    jthread t_gestion_couleur(thread_gestion_couleur);
    jthread t_gestion_feux(thread_gestion_feux);
    jthread t_generation_objets(thread_generation_objets);

    window.close();
    return 0;
}