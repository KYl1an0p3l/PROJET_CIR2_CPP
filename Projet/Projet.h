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
    int x;             // Position en X
    int y;             // Position en Y
    int direction;     // Direction : 0 (Haut), 1 (Bas), 2 (Gauche), 3 (Droite)
    int longueur;      // Longueur de l'objet (réelle - 1)
    int vitesse;       // Vitesse de déplacement
    int id;            // Identifiant unique pour le type d'objet
    vector<int> voie; //Stocke la case sur laquelle l'objet vient de se déplacer
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
void generation_bus();
void deplacement();
void free_position(Objet objet);
bool gestion_stop(Objet objet, int& etat_feu);

//-----------------Threads-------------------------------------------------------------------------------------------

//void thread_generation_voiture();
//void thread_generation_pietons();
//void thread_deplacements();


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
            else if (carte[i][j] == 8) {
                tile.setFillColor(Color::Yellow);
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
        etat_feu = 1;
    }
    else if ((cpt % 100) == 0) {
        if (etat_feu == 0) {
            feu.setFillColor(Color::Red);
            feu2.setFillColor(Color::Green);
            etat_feu = 1;
            carte[14][13] = 4;carte[14][18] = 4;carte[22][18] = 4;carte[22][13] = 4; //On reset les coins des troittoirs pour éviter que des piétons s'y bloquent 
        }
        else if (etat_feu == 1) {
            feu.setFillColor(Color::Red);
            feu2.setFillColor(Color(255, 128, 0));
            cpt += 49;
            etat_feu = 2;
            carte[14][13] = 4;carte[14][18] = 4;carte[22][18] = 4;carte[22][13] = 4; //On reset les coins des troittoirs pour éviter que des piétons s'y bloquent 
        }
        else if (etat_feu == 2) {
            feu.setFillColor(Color::Green);
            feu2.setFillColor(Color::Red);
            etat_feu = 3;
            carte[14][13] = 4;carte[14][18] = 4;carte[22][18] = 4;carte[22][13] = 4; //On reset les coins des troittoirs pour éviter que des piétons s'y bloquent 
        }
        else if (etat_feu == 3) {
            feu.setFillColor(Color(255, 128, 0));
            feu2.setFillColor(Color::Red);
            cpt += 49;
            etat_feu = 0;
            carte[14][13] = 4;carte[14][18] = 4;carte[22][18] = 4;carte[22][13] = 4; //On reset les coins des troittoirs pour éviter que des piétons s'y bloquent 
        }
    }
    else {
        if (etat_feu == 1) {
            feu.setFillColor(Color::Red);
            feu2.setFillColor(Color::Green);
        }
        else if (etat_feu == 2) {
            feu.setFillColor(Color::Red);
            feu2.setFillColor(Color(255, 128, 0));
            cpt += 49; //Pour que l'on ne passe que 2 itérations en orange
        }
        else if (etat_feu == 3) {
            feu.setFillColor(Color::Green);
            feu2.setFillColor(Color::Red);
        }
        else if (etat_feu == 0) {
            feu.setFillColor(Color(255, 128, 0));
            feu2.setFillColor(Color::Red);
            cpt += 49; //Pour que l'on ne passe que 2 itérations en orange
        }
    }
    cpt++;//On passe 1 itération
    window.draw(feu);
    window.draw(feu2);

}

void generation_voitures() {
    if (rand() % 100 >= 75) return;
    printf("\nTest_gen_v");

    int direction = rand() % 4;
    for (int i = 0; i < 4; i++) {
        switch ((direction + i) % 4) {
        case 0: // Bas
            if (carte[0][15] == 1 && carte[1][15] == 1 && carte[2][15] == 1) {
                objets.push_back({ 15, 1, 1, 1, 3, 6, vector<int>{1, 1} });
                carte[0][15] = 6;
                carte[1][15] = 6;
                return;
            }
            break;

        case 1: // Haut
            if (carte[31][16] == 1 && carte[30][16] == 1 && carte[29][16] == 1) {
                objets.push_back({ 16, 30, 0, 1, 3, 6, vector<int>{1, 1} });
                carte[31][16] = 6;
                carte[30][16] = 6;
                return;
            }
            break;

        case 2: // Gauche
            if (carte[17][31] == 1 && carte[17][30] == 1 && carte[17][29] == 1) {
                objets.push_back({ 30, 17, 2, 1, 3, 6, vector<int>{1, 1} });
                carte[17][31] = 6;
                carte[17][30] = 6;
                return;
            }
            break;

        case 3: // Droite
            if (carte[19][0] == 1 && carte[19][1] == 1 && carte[19][2] == 1) {
                objets.push_back({ 1, 19, 3, 1, 3, 6, vector<int>{1, 1} });
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
    printf("\nTest_gen_p");

    int direction = rand() % 4;
    for (int i = 0; i < 4; i++) {
        switch ((direction + i) % 4) {
        case 0: // Bas
            if (carte[0][13] == 4 && carte[1][13] == 4) {
                objets.push_back({ 13, 0, 1, 0, 1, 7, vector<int>{4} });
                carte[0][13] = 7;
                return;
            }
            break;

        case 1: // Haut
            if (carte[31][18] == 4 && carte[30][18] == 4) {
                objets.push_back({ 18, 31, 0, 0, 1, 7, vector<int>{4} });
                carte[31][18] = 7;
                return;
            }
            break;

        case 2: // Gauche
            if (carte[14][31] == 4 && carte[14][30] == 4) {
                objets.push_back({ 31, 14, 2, 0, 1, 7, vector<int>{4} });
                carte[14][31] = 7;
                return;
            }
            break;

        case 3: // Droite
            if (carte[22][0] == 4 && carte[22][1] == 1) { //Ce trottoir est un peu spécifique
                objets.push_back({ 0, 22, 3, 0, 1, 7, vector<int>{4} });
                carte[22][0] = 7;
                return;
            }
            break;
        }
    }
}

void generation_bus() {
    if (rand() % 100 >= 50) return;
    printf("\nTest_gen_b");

    int direction = rand() % 4;
    for (int i = 0; i < 4; i++) {
        switch ((direction + i) % 4) {
        case 2: // Gauche
            if (carte[16][31] == 2 && carte[16][30] == 2 && carte[16][29] == 2 && carte[16][28] == 2) {
                objets.push_back({ 29, 16, 2, 2, 3, 8, vector<int>{2, 2, 2} });
                return;
            }
            break;

        case 3: // Droite
            if (carte[20][0] == 2 && carte[20][1] == 1 && carte[20][2] == 2 && carte[20][3] == 2) {
                objets.push_back({ 2, 20, 3, 2, 3, 8, vector<int>{2, 1, 2} }); //La voie à cet endroit est particulière
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
        for (int i = 0; i <= objet.longueur; i++) {
            carte[objet.y + i][objet.x] = objet.voie[i];
        }
        break;
    case 1: // Bas
        for (int i = 0; i <= objet.longueur; i++) {
            carte[objet.y - i][objet.x] = objet.voie[i];
        }
        break;
    case 2: // Gauche
        for (int i = 0; i <= objet.longueur; i++) {
            carte[objet.y][objet.x + i] = objet.voie[i];
        }
        break;
    case 3: // Droite
        for (int i = 0; i <= objet.longueur; i++) {
            carte[objet.y][objet.x - i] = objet.voie[i];
        }
        break;
    }
}

// Vérification obligation d'arret
bool gestion_stop(Objet objet, int& etat_feu) {
    bool stop = false;
    if (objet.id == 6 || objet.id == 8) {
        switch (objet.direction) {
        case 0: // Haut
            if (etat_feu == 3) { break; } // Feu vert
            if (objet.y < 21 && etat_feu == 0) { break; } // Freinage trop dangereux ou feu dépassé
            stop = true;
            break;
        case 1: // Bas
            if (etat_feu == 3) { break; } // Feu vert
            if (objet.y > 11 && etat_feu == 0) { break; } // Freinage trop dangereux ou feu dépassé
            stop = true;
            break;
        case 2: // Gauche
            if (etat_feu == 1) { break; } // Feu vert
            if (objet.x < 21 && etat_feu == 2) { break; } // Freinage trop dangereux ou feu dépassé
            stop = true;
            break;
        case 3: // Droite
            if (etat_feu == 1) { break; } // Feu vert
            if (objet.x > 11 && etat_feu == 2) { break; } // Freinage trop dangereux ou feu dépassé
            stop = true;
            break;
        }
    }
    else if (objet.id == 7) {
        switch (objet.direction) {
        case 0: // Haut
            if (etat_feu == 3 && carte[22][17] != 7 && carte[22][16] != 7) { break; } //Feu vert
            stop = true;
            break;
        case 1: // Bas
            if (etat_feu == 3) { break; } // Feu vert
            else { stop = true; }
            break;
        case 2: // Gauche
            if (etat_feu == 1) { break; } // Feu vert
            else { stop = true; }
            break;
        case 3: // Droite
            if (etat_feu == 1) { break; } // Feu vert
            else { stop = true; }
            break;
        }
    }
    return stop;
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
                        for (int k = 6; k < 10; k++) {
                            if (collision) {
                                break;
                            }
                            else {
                                if (objet.id == 7) {
                                    collision = carte[newY - 2][newX] == objet.id;
                                    collision = carte[newY - 1][newX] == objet.id;
                                }
                                else {
                                    collision = carte[newY - i][newX] == k;
                                }
                            }
                        }
                    }
                }
            }
            break;
        case 1: // Bas
            if (newY <= 28) {
                for (int i = 0; i < objet.vitesse; i++) {
                    if (!collision) {
                        for (int k = 6; k < 10; k++) {
                            if (collision) {
                                break;
                            }
                            else {
                                if (objet.id == 7) {
                                    collision = carte[newY + 2][newX] == objet.id;
                                    collision = carte[newY + 1][newX] == objet.id;
                                }
                                else {
                                    collision = carte[newY + i][newX] == k;
                                }
                            }
                        }
                    }
                }
            }
            break;
        case 2: // Gauche
            if (newX >= 3) {
                for (int i = 0; i < objet.vitesse; i++) {
                    if (!collision) {
                        for (int k = 6; k < 10; k++) {
                            if (collision) {
                                break;
                            }
                            else {
                                if (objet.id == 7) {
                                    collision = carte[newY][newX - 2] == objet.id;
                                    collision = carte[newY][newX - 1] == objet.id;
                                }
                                else {
                                    collision = carte[newY][newX - i] == k;
                                }
                            }
                        }
                    }
                }
            }
            break;
        case 3: // Droite
            if (newX <= 28) {
                for (int i = 0; i < objet.vitesse; i++) {
                    if (!collision) {
                        for (int k = 6; k < 10; k++) {
                            if (collision) {
                                break;
                            }
                            else {
                                if (objet.id == 7) {
                                     collision = carte[newY][newX + 2] == objet.id;
                                     collision = carte[newY][newX + 1] == objet.id;
                                }
                                else {
                                    collision = carte[newY][newX + i] == k;
                                }
                            }
                        }
                    }
                }
            }
            break;
        }

        // Si collision pas de déplacement
        if (collision) {
            continue;
        }

        // Vérification couleur feu
        bool stop = gestion_stop(objet, etat_feu);

        if (stop) {
            // Calcul de la distance
            if (objet.id == 6 || objet.id == 8) {
                switch (objet.direction) {
                case 0: //Haut
                    if (objet.y == 25 || objet.y == 26) { newY++; }
                    else if (objet.y == 24) { newY += 2; }
                    else if (objet.y == 23) { continue; }
                    break;
                case 1: //Bas
                    if (objet.y == 11 || objet.y == 10) { newY--; }
                    else if (objet.y == 12) { newY -= 2; }
                    else if (objet.y == 13) { continue; }
                    break;
                case 2: //Gauche
                    if (objet.x == 21 || objet.x == 22) { newX++; }
                    else if (objet.x == 20) { newX += 2; }
                    else if (objet.x == 19) { continue; }
                    break;
                case 3: //Droite
                    if (objet.x == 10 || objet.x == 9) { newX--; }
                    else if (objet.x == 11) { newX -= 2; }
                    else if (objet.x == 12) { continue; }
                    break;
                }
            }
            else if (objet.id == 7) {
                switch (objet.direction) {
                case 0: //Haut
                    if (objet.y == 23) { continue; }
                    break;
                case 1: //Bas
                    if (objet.y == 13) { continue; }
                    break;
                case 2: //Gauche
                    if (objet.x == 19) { continue; }
                    break;
                case 3: //Droite
                    if (objet.x == 12) { continue; }
                    break;
                }


            }
        }


        // Libère l'ancienne position
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
            for (int i = 0; i <= objet.longueur; i++) {
                objet.voie[i] = carte[objet.y + i][objet.x];
                carte[objet.y + i][objet.x] = objet.id;
            }
            break;
        case 1: // Bas
            for (int i = 0; i <= objet.longueur; i++) {
                objet.voie[i] = carte[objet.y - i][objet.x];
                carte[objet.y - i][objet.x] = objet.id;
            }
            break;
        case 2: // Gauche
            for (int i = 0; i <= objet.longueur; i++) {
                objet.voie[i] = carte[objet.y][objet.x + i];
                carte[objet.y][objet.x + i] = objet.id;
            }
            break;
        case 3: // Droite
            for (int i = 0; i <= objet.longueur; i++) {
                objet.voie[i] = carte[objet.y][objet.x - i];
                carte[objet.y][objet.x - i] = objet.id;
            }
            break;
        }
    }
}

//----------------------------Threads (explicités)--------------------------------------------

//void thread_generation_voiture() {
//        //objets_mutex.lock();
//        generation_voitures();
//        //objets_mutex.unlock();
//        
//        this_thread::sleep_for(chrono::milliseconds(33));
//}
//
//void thread_generation_pietons() {
//        //objets_mutex.lock();
//        generation_pietons();
//        //objets_mutex.unlock();
//        
//        this_thread::sleep_for(chrono::milliseconds(33));
//}
//
//void thread_deplacements() {
//        //objets_mutex.lock();
//        deplacement();
//        //objets_mutex.unlock();
//        
//        this_thread::sleep_for(chrono::milliseconds(33));
//}
