﻿// Projet.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets.

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
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
mutex objets_mutex;

//------------------Fonctions----------------------------------------------------------------

void gestion_couleur();
void gestion_feux(int& cpt, int& etat_feu);
void generation_voitures();
void generation_pietons();
void deplacement();
void free_position(Objet objet);
void gestion_stop(Objet objet, int& etat_feu);

//-----------------Threads-------------------------------------------------------------------------------------------

void thread_gestion_couleur();
void thread_gestion_feux();
void thread_generation_objets();


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
            else if (carte[i][j] == 6) {
                tile.setFillColor(Color::Blue);
            }
            else if (carte[i][j] == 7) {
                tile.setFillColor(Color::Red);
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
                objets.push_back({ 15, 1, 1, 1, 3, 6 });
                carte[0][15] = 6;
                carte[1][15] = 6;
                return;
            }
            break;

        case 1: // Bas
            if (carte[31][16] == 1 && carte[30][16] == 1 && carte[29][16] == 1) {
                objets.push_back({ 16, 30, 0, 1, 3, 6 });
                carte[31][16] = 6;
                carte[30][16] = 6;
                return;
            }
            break;

        case 2: // Droite
            if (carte[17][31] == 1 && carte[17][30] == 1 && carte[17][29] == 1) {
                objets.push_back({ 30, 17, 2, 1, 3, 6 });
                carte[17][31] = 6;
                carte[17][30] = 6;
                return;
            }
            break;

        case 3: // Gauche
            if (carte[19][0] == 1 && carte[19][1] == 1 && carte[19][2] == 1) {
                objets.push_back({ 1, 19, 3, 1, 3, 6 });
                carte[19][0] = 6;
                carte[19][1] = 6;
                return;
            }
            break;
        }
    }
}

void generation_pietons() {
    if (rand() % 100 >= 75) return;

    int direction = rand() % 4;
    for (int i = 0; i < 4; i++) {
        switch ((direction + i) % 4) {
        case 0: // Haut
            if (carte[0][13] == 4 && carte[1][13] == 4) {
                objets.push_back({ 0, 13, 1, 0, 1, 7 });
                carte[0][13] = 7;
                return;
            }
            break;

        case 1: // Bas
            if (carte[31][18] == 4 && carte[30][18] == 4) {
                objets.push_back({ 31, 18, 0, 0, 1, 7 });
                carte[31][18] = 7;
                return;
            }
            break;

        case 2: // Droite
            if (carte[14][31] == 4 && carte[14][30] == 4) {
                objets.push_back({ 14, 31, 2, 0, 1, 7 });
                carte[14][31] = 7;
                return;
            }
            break;

        case 3: // Gauche
            if (carte[22][0] == 4 && carte[22][1] == 1) { //Ce trottoir est un peu spécifique
                objets.push_back({ 22, 0, 3, 0, 1, 7 });
                carte[22][0] = 7;
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
        carte[objet.y][objet.x] = 1;
        carte[objet.y + objet.longueur][objet.x] = 1;
        break;
    case 1: // Bas
        carte[objet.y][objet.x] = 1;
        carte[objet.y - objet.longueur][objet.x] = 1;
        break;
    case 2: // Gauche
        carte[objet.y][objet.x] = 1;
        carte[objet.y][objet.x + objet.longueur] = 1;
        break;
    case 3: // Droite
        carte[objet.y][objet.x] = 1;
        carte[objet.y][objet.x - objet.longueur] = 1;
        break;
    }
}

void gestion_stop(Objet objet, int& etat_feu) {
    switch (objet.direction) {
        case 0: // Haut

        break;
        case 1: // Bas

        break;
        case 2: // Gauche

        break;
        case 3: // Droite

        break;
    }
}

void deplacement() {

    for (size_t i = 0; i < objets.size(); i++) {
        Objet& objet = objets[i];

        // Calcul de la nouvelle position
        int newX = objet.x, newY = objet.y;
        switch (objet.direction) {
        case 0: newY -= objet.vitesse; break; // Haut
        case 1: newY += objet.vitesse; break; // Bas
        case 2: newX -= objet.vitesse; break; // Gauche
        case 3: newX += objet.vitesse; break; // Droite
        }

        // Vérification des limites
        if (newX < 0 || newX > 31 || newY < 0 || newY > 31) {
            free_position(objet);
            objets.erase(objets.begin() + i);
            i--;
            continue;
        }

        // Vérification des collisions
        bool collision = false;
        switch (objet.direction) {
        case 0: // Haut
            if (newY >= 3) {
                for (int i = 0; i < objet.vitesse; i++) {
                    if (!collision) {
                        collision = carte[newY + i][newX] == objet.id;
                    }
                }
            }
            break;
        case 1: // Bas
            if (newY <= 28) {
                for (int i = 0; i < objet.vitesse; i++) {
                    if (!collision) {
                        collision = carte[newY - i][newX] == objet.id;
                    }
                }
            }
            break;
        case 2: // Gauche
            if (newX >= 3) {
                for (int i = 0; i < objet.vitesse; i++) {
                    if (!collision) {
                        collision = carte[newY][newX + i] == objet.id;
                    }
                }
            }
            break;
        case 3: // Droite
            if (newX <= 28) {
                for (int i = 0; i < objet.vitesse; i++) {
                    if (!collision) {
                        collision = carte[newY][newX - i] == objet.id;
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
            objet.y = newY;
        }
        else if (objet.direction == 2 || objet.direction == 3) {
            objet.x = newX;
        }

        // Mise à jour de la carte
        switch (objet.direction) {
        case 0: // Haut
            carte[objet.y][objet.x] = objet.id;
            carte[objet.y + objet.longueur][objet.x] = objet.id;
            break;
        case 1: // Bas
            carte[objet.y][objet.x] = objet.id;
            carte[objet.y - objet.longueur][objet.x] = objet.id;
            break;
        case 2: // Gauche
            carte[objet.y][objet.x] = objet.id;
            carte[objet.y][objet.x + objet.longueur] = objet.id;
            break;
        case 3: // Droite
            carte[objet.y][objet.x] = objet.id;
            carte[objet.y][objet.x - objet.longueur] = objet.id;
            break;
        }
    }
}

//----------------------------Threads (explicités)--------------------------------------------

void thread_gestion_couleur() {
    while (window.isOpen()) {
        objets_mutex.lock();
        gestion_couleur();
        objets_mutex.unlock();
        this_thread::sleep_for(chrono::milliseconds(33)); // 30 FPS
    }
}

void thread_gestion_feux() {
    while (window.isOpen()) {
        gestion_feux(cpt, etat_feu);
        this_thread::sleep_for(chrono::milliseconds(33)); // 30 FPS
    }
}

void thread_generation_objets() {
    while (window.isOpen()) {
        objets_mutex.lock();
        generation_voitures();
        generation_pietons();
        objets_mutex.unlock();
        this_thread::sleep_for(chrono::milliseconds(33));
    }
}
