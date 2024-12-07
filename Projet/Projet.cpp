﻿// Projet.cpp : définit le point d'entrée de l'application.
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
        deplacement_voitures();

        window.display();
        this_thread::sleep_for(std::chrono::seconds(1));
    }

    window.close();
    return 0;
}