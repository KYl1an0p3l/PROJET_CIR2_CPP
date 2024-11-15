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

        window.clear(Color::Black);

        window.display();
    }

    window.close();
    return 0;
}