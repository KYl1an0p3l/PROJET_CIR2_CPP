// Projet.cpp : définit le point d'entrée de l'application.
//

#include "Projet.h"
#include "map.h"
#define TILE_SIZE 20

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

        for (int i = 0; i < 32;i++) {
            for (int j = 0; j < 32;j++) {
                RectangleShape tile(Vector2f(TILE_SIZE, TILE_SIZE));
                tile.setPosition(j * TILE_SIZE, i * TILE_SIZE);
                if (carte[i][j] == 0) {
                    tile.setFillColor(Color(128, 128, 128));
                }
                else if (carte[i][j] == 1) {
                    tile.setFillColor(Color::Black);
                }
                else if (carte[i][j] == 2) {
                    tile.setFillColor(Color(150, 105, 180));
                }
                else if (carte[i][j] == 3) {
                    tile.setFillColor(Color::Green);
                }
                else if (carte[i][j] == 4) {
                    tile.setFillColor(Color(200, 200, 200));
                }

                window.draw(tile);
            }
        }

        window.display();
    }

    window.close();
    return 0;
}