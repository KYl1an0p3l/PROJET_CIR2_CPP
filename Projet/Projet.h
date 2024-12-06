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

//---------------------Constantes-----------------------------------------------------------

#define TILE_SIZE 20
int etat_feu = 0;
int cpt = 0;
RenderWindow window(VideoMode(800, 640), "Projet CPP");

//----------------------Structures-------------------------------------------------------------

typedef struct Voiture {
    int x;
    int y;
    int direction;
    int longueur = 2;
    int vitesse = 3;
    int id = 6;
}Voiture;

//------------------Fonctions----------------------------------------------------------------

void gestion_couleur();
void gestion_feux(int& cpt, int& etat_feu);
void generation_voitures();
void deplacement_voitures();
void free_placement(Voiture voiture);

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

// Déclaration du vecteur pour stocker les voitures
vector<Voiture> voitures;

void generation_voitures() {
    if (rand() % 100 >= 75) return;

    int direction = rand() % 4;
    for (int i = 0; i < 4; i++) {
        switch ((direction + i) % 4) {
            case 0: // Haut
                if (carte[0][15] == 1 && carte[1][15] == 1 && carte[2][15] == 1) {
                    voitures.push_back({0, 15, 1, 2, 3});
                    carte[0][15] = 6;
                    carte[1][15] = 6;
                    return;
                }
            break;

            case 1: // Bas
                if (carte[31][16] == 1 && carte[30][16] == 1 && carte[29][16] == 1) {
                    voitures.push_back({31, 16, 0, 2, 3});
                    carte[31][16] = 6;
                    carte[30][16] = 6;
                    return;
                }
            break;

            case 2: // Gauche
                if (carte[17][31] == 1 && carte[17][30] == 1 && carte[17][29] == 1) {
                    voitures.push_back({17, 31, 2, 2, 3});
                    carte[17][31] = 6;
                    carte[17][30] = 6;
                    return;
                }
            break;

            case 3: // Droite
                if (carte[19][0] == 1 && carte[19][1] == 1 && carte[19][2] == 1) {
                    voitures.push_back({19, 0, 3, 2, 3});
                    carte[19][0] = 6;
                    carte[19][1] = 6;
                    return;
                }
            break;
        }
    }
}

void free_placement(Voiture voiture) {
    // Libérer les anciennes positions sur la carte
    switch (voiture.direction) {
        case 0: // Haut
            carte[voiture.x][voiture.y] = 1;
        carte[voiture.x - 1][voiture.y] = 1;
        break;
        case 1: // Bas
            carte[voiture.x][voiture.y] = 1;
        carte[voiture.x + 1][voiture.y] = 1;
        break;
        case 2: // Gauche
            carte[voiture.x][voiture.y] = 1;
        carte[voiture.x][voiture.y - 1] = 1;
        break;
        case 3: // Droite
            carte[voiture.x][voiture.y] = 1;
        carte[voiture.x][voiture.y + 1] = 1;
        break;
    }
}

void deplacement_voitures() {

    for (size_t i = 0; i < voitures.size(); i++ ) {
        Voiture& voiture = voitures[i];

        // Calcul de la nouvelle position
        int newX = voiture.x, newY = voiture.y;
        switch (voiture.direction) {
            case 0: newX -= voiture.vitesse; break; // Haut
            case 1: newX += voiture.vitesse; break; // Bas
            case 2: newY -= voiture.vitesse; break; // Gauche
            case 3: newY += voiture.vitesse; break; // Droite
        }

        // Vérification des limites
        if (newX <= 0 || newX >= 31 || newY <= 0 || newY >= 31) {
            free_placement(voiture);
            voitures.erase(voitures.begin() + i);
            i--;
            continue;
        }

        // Vérification des collisions
        bool collision = false;
        switch (voiture.direction) {
            case 0: // Haut
                if (newX >= 3) {
                    collision = carte[newX][newY] == voiture.id || carte[newX - 1][newY] == voiture.id || carte[newX - 2][newY] == voiture.id || carte[newX - 3][newY] == voiture.id;
                }
            break;
            case 1: // Bas
                if (newX <= 28) {
                    collision = carte[newX][newY] == voiture.id || carte[newX + 1][newY] == voiture.id || carte[newX + 2][newY] == voiture.id || carte[newX + 3][newY] == voiture.id;
                }
            break;
            case 2: // Gauche
                if (newY >= 3) {
                    collision = carte[newX][newY] == voiture.id || carte[newX][newY - 1] == voiture.id || carte[newX][newY - 2] == voiture.id || carte[newX][newY - 3] == voiture.id;
                }
            break;
            case 3: // Droite
                if (newY <= 28) {
                    collision = carte[newX][newY] == voiture.id || carte[newX][newY + 1] == voiture.id || carte[newX][newY + 2] == voiture.id || carte[newX][newY + 3] == voiture.id;
                }
            break;
        }


        if (collision) {
            continue;
        }


        free_placement(voiture);



        // Mise à jour de la position
        if (voiture.direction == 0 || voiture.direction == 1) {
            voiture.x = newX;
        } else if (voiture.direction == 2 || voiture.direction == 3) {
            voiture.y = newY;
        }

        // Mise à jour de la carte
        switch (voiture.direction) {
            case 0: // Haut
                carte[voiture.x][voiture.y] = voiture.id;
                carte[voiture.x - 1][voiture.y] = voiture.id;
                break;
            case 1: // Bas
                carte[voiture.x][voiture.y] = voiture.id;
                carte[voiture.x + 1][voiture.y] = voiture.id;
                break;
            case 2: // Gauche
                carte[voiture.x][voiture.y] = voiture.id;
                carte[voiture.x][voiture.y - 1] = voiture.id;
                break;
            case 3: // Droite
                carte[voiture.x][voiture.y] = voiture.id;
                carte[voiture.x][voiture.y + 1] = voiture.id;
                break;
        }
    }
}
