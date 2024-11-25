// Projet.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets.

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "map.h"
using namespace std;
using namespace sf;

//---------------------Constantes-----------------------------------------------------------

#define TILE_SIZE 20
RenderWindow window(VideoMode(800, 600), "Projet CPP");

//----------------------Structures-------------------------------------------------------------

typedef struct Voiture {
	int longueur = 2;
	int vitesse = 3;
	int id = 6;
}Voiture;

//------------------Fonctions----------------------------------------------------------------

void gestion_couleur();
void gestion_feu(int cpt);

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

void gestion_feu(int cpt) {
    bool hasChanged = false;
    RectangleShape feu(Vector2f(TILE_SIZE, TILE_SIZE));
    feu.setPosition(0, 640);
    if (cpt == 0) {
        feu.setFillColor(Color::Red);
    }
    else if (cpt == 20) {
        cpt = 0;
        if (feu.getFillColor() == Color(255, 128, 0) && !hasChanged) {
            feu.setFillColor(Color::Red);
            hasChanged = true;
        }
        else if (feu.getFillColor() == Color::Green && !hasChanged) {
            feu.setFillColor(Color(255, 128, 0));
            hasChanged = true;
        }
        else if (feu.getFillColor() == Color::Red && !hasChanged) {
            feu.setFillColor(Color::Green);
            hasChanged = true;
        }
    }
    cpt++;
    
}