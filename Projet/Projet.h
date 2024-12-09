// Projet.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets.

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "map.h"
using namespace std;
using namespace sf;

//----------------------Structures-------------------------------------------------------------

typedef struct Objet {
    int x;
    int y;
    int direction;
    int longueur; //longueur = longueur réelle -1
    int vitesse;
    int id;
}Objet;

//---------------------Constantes-----------------------------------------------------------

#define TILE_SIZE 20
int etat_feu = 0;
int cpt = 0;
RenderWindow window(VideoMode(800, 640), "Projet CPP");
vector<Objet> objets;

//------------------Fonctions----------------------------------------------------------------

void gestion_couleur();
void gestion_feux(int& cpt, int& etat_feu);
void generation_voitures();
void deplacement();
void free_position(Objet objet);

//-------------Fonctions (explicités)---------------------------------------------------------------------------

void gestion_couleur() {
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
}

void gestion_feux(int& cpt, int& etat_feu) {
    RectangleShape feu(Vector2f(TILE_SIZE, TILE_SIZE));
    RectangleShape feu2(Vector2f(TILE_SIZE, TILE_SIZE));
    feu.setPosition(0, 0);
    feu2.setPosition(0, 20);
    if (cpt == 0) {
        feu.setFillColor(Color::Red);
        feu2.setFillColor(Color::Green);
        etat_feu = 0;
    }
    else if ((cpt % 1000) == 0) {
        if (etat_feu == 0) {
            feu.setFillColor(Color::Red);
            feu2.setFillColor(Color::Green);
            etat_feu = 1;
        }
        else if (etat_feu == 1) {
            feu.setFillColor(Color::Red);
            feu2.setFillColor(Color(255, 128, 0));
            etat_feu = 2;
        }
        else if (etat_feu == 2) {
            feu.setFillColor(Color::Green);
            feu2.setFillColor(Color::Red);
            etat_feu = 3;
        }
        else if (etat_feu == 3) {
            feu.setFillColor(Color(255, 128, 0));
            feu2.setFillColor(Color::Red);
            etat_feu = 0;
        }
    }
    else {
        if (etat_feu == 0) {
            feu.setFillColor(Color::Red);
            feu2.setFillColor(Color::Green);
        }
        else if (etat_feu == 1) {
            feu.setFillColor(Color::Red);
            feu2.setFillColor(Color(255, 128, 0));
            cpt += 99; //Pour que l'on ne passe que 10 itérations en orange
        }
        else if (etat_feu == 2) {
            feu.setFillColor(Color::Green);
            feu2.setFillColor(Color::Red);
        }
        else if (etat_feu == 3) {
            feu.setFillColor(Color(255, 128, 0));
            feu2.setFillColor(Color::Red);
            cpt += 99; //Pour que l'on ne passe que 10 itérations en orange
        }
    }
    cpt++;
    window.draw(feu);
    window.draw(feu2);
    
}

void generation_voitures() {
    if (rand() % 100 >= 75) return;

    int direction = rand() % 4;
    for (int i = 0; i < 4; i++) {
        switch ((direction + i) % 4) {
            case 0: // Haut
                if (carte[0][15] == 1 && carte[1][15] == 1 && carte[2][15] == 1) {
                    objets.push_back({0, 15, 1, 1, 3, 6});
                    carte[0][15] = 6;
                    carte[1][15] = 6;
                    return;
                }
            break;

            case 1: // Bas
                if (carte[31][16] == 1 && carte[30][16] == 1 && carte[29][16] == 1) {
                    objets.push_back({31, 16, 0, 1, 3, 6});
                    carte[31][16] = 6;
                    carte[30][16] = 6;
                    return;
                }
            break;

            case 2: // Gauche
                if (carte[17][31] == 1 && carte[17][30] == 1 && carte[17][29] == 1) {
                    objets.push_back({17, 31, 2, 1, 3, 6});
                    carte[17][31] = 6;
                    carte[17][30] = 6;
                    return;
                }
            break;

            case 3: // Droite
                if (carte[19][0] == 1 && carte[19][1] == 1 && carte[19][2] == 1) {
                    objets.push_back({19, 0, 3, 1, 3, 6});
                    carte[19][0] = 6;
                    carte[19][1] = 6;
                    return;
                }
            break;
        }
    }
}

void free_position(Objet objet) {
    // Libérer les anciennes positions sur la carte
    switch (objet.direction) {
        case 0: // Haut
            carte[objet.x][objet.y] = 1;
        carte[objet.x - objet.longueur][objet.y] = 1;
        break;
        case 1: // Bas
            carte[objet.x][objet.y] = 1;
        carte[objet.x + objet.longueur][objet.y] = 1;
        break;
        case 2: // Gauche
            carte[objet.x][objet.y] = 1;
        carte[objet.x][objet.y - objet.longueur] = 1;
        break;
        case 3: // Droite
            carte[objet.x][objet.y] = 1;
        carte[objet.x][objet.y + objet.longueur] = 1;
        break;
    }
}

void deplacement() {

    for (size_t i = 0; i < objets.size(); i++ ) {
        Objet& objet = objets[i];

        // Calcul de la nouvelle position
        int newX = objet.x, newY = objet.y;
        switch (objet.direction) {
            case 0: newX -= objet.vitesse; break; // Haut
            case 1: newX += objet.vitesse; break; // Bas
            case 2: newY -= objet.vitesse; break; // Gauche
            case 3: newY += objet.vitesse; break; // Droite
        }

        // Vérification des limites
        if (newX <= 0 || newX >= 31 || newY <= 0 || newY >= 31) {
            free_position(objet);
            objets.erase(objets.begin() + i);
            i--;
            continue;
        }

        // Vérification des collisions
        bool collision = false;
        switch (objet.direction) {
            case 0: // Haut
                if (newX >= 3) {
                    for (int i = 0; i < objet.vitesse; i++) {
                        if (!collision) {
                            collision = carte[newX - i][newY] == objet.id;
                        }
                    }
                }
            break;
            case 1: // Bas
                if (newX <= 28) {
                    for (int i = 0; i < objet.vitesse; i++) {
                        if (!collision) {
                            collision = carte[newX + i][newY] == objet.id;
                        }
                    }
                }
            break;
            case 2: // Gauche
                if (newY >= 3) {
                    for (int i = 0; i < objet.vitesse; i++) {
                        if (!collision) {
                            collision = carte[newX][newY - i] == objet.id;
                        }
                    }
                }
            break;
            case 3: // Droite
                if (newY <= 28) {
                    for (int i = 0; i < objet.vitesse; i++) {
                        if (!collision) {
                            collision = carte[newX][newY + i] == objet.id;
                        }
                    }
                }
            break;
        }


        if (collision) {
            continue;
        }


        free_position(objet);



        // Mise à jour de la position
        if (objet.direction == 0 || objet.direction == 1) {
            objet.x = newX;
        } else if (objet.direction == 2 || objet.direction == 3) {
            objet.y = newY;
        }

        // Mise à jour de la carte
        switch (objet.direction) {
            case 0: // Haut
                carte[objet.x][objet.y] = objet.id;
                carte[objet.x - 1][objet.y] = objet.id;
                break;
            case 1: // Bas
                carte[objet.x][objet.y] = objet.id;
                carte[objet.x + 1][objet.y] = objet.id;
                break;
            case 2: // Gauche
                carte[objet.x][objet.y] = objet.id;
                carte[objet.x][objet.y - 1] = objet.id;
                break;
            case 3: // Droite
                carte[objet.x][objet.y] = objet.id;
                carte[objet.x][objet.y + 1] = objet.id;
                break;
        }
    }
}
