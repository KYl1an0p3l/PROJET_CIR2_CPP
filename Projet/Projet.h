// Projet.h : fichier Include pour les fichiers Include système standard,
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
extern bool running; // Contrôle l'exécution des threads
extern mutex objets_mutex; // Mutex pour protéger les objets
extern mutex feu_mutex;    // Mutex pour protéger l'état des feux




//------------------Fonctions----------------------------------------------------------------

void gestion_couleur();
void gestion_feux(int& cpt, int& etat_feu);
void generation_voitures();
void generation_pietons();
void generation_bus();
void generation_velos();
void deplacement();
void free_position(Objet objet);
bool gestion_stop(Objet objet, int& etat_feu);
void gestion_rotation(Objet& objet);
void dessin_feux(int etat_feux);

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
            else if (carte[i][j] == 9) {
                tile.setFillColor(Color::Cyan);
            }
            window.draw(tile);
        }
    }
}

void dessin_feux(int etat_feux) {
    RectangleShape feu(Vector2f(TILE_SIZE, TILE_SIZE));
    RectangleShape feu2(Vector2f(TILE_SIZE, TILE_SIZE));
    feu.setPosition(220, 240);
    feu2.setPosition(400, 240);
    if (etat_feux == 1) {
        feu.setFillColor(Color::Red);
        feu2.setFillColor(Color::Green);
    }
    else if (etat_feux == 2) {
        feu.setFillColor(Color::Red);
        feu2.setFillColor(Color(255, 128, 0));
    }
    else if (etat_feux == 3) {
        feu.setFillColor(Color::Green);
        feu2.setFillColor(Color::Red);
    }
    else if (etat_feux == 0) {
        feu.setFillColor(Color(255, 128, 0));
        feu2.setFillColor(Color::Red);
    }
    window.draw(feu);
    feu.setPosition(400, 480);
    window.draw(feu);
    window.draw(feu2);
    feu2.setPosition(220, 480);
    window.draw(feu2);
}

void gestion_feux(int& cpt, int& etat_feu) {
    if (cpt == 0) {
        etat_feu = 1;
    }
    else if ((cpt % 100) == 0) {
        if (etat_feu == 0) {
            etat_feu = 1;
            carte[14][13] = 4;carte[14][18] = 4;carte[22][18] = 4;carte[22][13] = 4; //On reset les coins des troittoirs pour éviter que des piétons s'y bloquent 
        }
        else if (etat_feu == 1) {
            cpt += 49;
            etat_feu = 2;
            carte[14][13] = 4;carte[14][18] = 4;carte[22][18] = 4;carte[22][13] = 4; //On reset les coins des troittoirs pour éviter que des piétons s'y bloquent 
        }
        else if (etat_feu == 2) {
            etat_feu = 3;
            carte[14][13] = 4;carte[14][18] = 4;carte[22][18] = 4;carte[22][13] = 4; //On reset les coins des troittoirs pour éviter que des piétons s'y bloquent 
        }
        else if (etat_feu == 3) {
            cpt += 49;
            etat_feu = 0;
            carte[14][13] = 4;carte[14][18] = 4;carte[22][18] = 4;carte[22][13] = 4; //On reset les coins des troittoirs pour éviter que des piétons s'y bloquent 
        }
    }
    else {
        if (etat_feu == 2) {
            cpt += 49; //Pour que l'on ne passe que 2 itérations en orange
        }
        else if (etat_feu == 0) {
            cpt += 49; //Pour que l'on ne passe que 2 itérations en orange
        }
    }
    cpt++;//On passe 1 itération
}

void generation_voitures() {
    if (rand() % 100 >= 10) return;
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
            if (rand() % 2) {
                if (carte[31][1] == 1 && carte[30][1] == 1 && carte[29][1] == 1) {
                    objets.push_back({ 1, 30, 0, 1, 3, 6, vector<int>{1, 1} });
                    carte[31][1] = 6;
                    carte[30][1] = 6;
                    return;
                }
            }
            else {
                if (carte[31][16] == 1 && carte[30][16] == 1 && carte[29][16] == 1) {
                    objets.push_back({ 16, 30, 0, 1, 3, 6, vector<int>{1, 1} });
                    carte[31][16] = 6;
                    carte[30][16] = 6;
                    return;
                }
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
    if (rand() % 100 >= 5) return;
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
            if (rand() % 3 == 0) {
                if (carte[31][18] == 4 && carte[30][18] == 4) {
                    objets.push_back({ 18, 31, 0, 0, 1, 7, vector<int>{4} });
                    return;
                }
            }
            if (rand() % 3 == 1) {
                if (carte[31][3] == 4 && carte[30][3] == 4) {
                    objets.push_back({ 3, 31, 0, 0, 1, 7, vector<int>{4} });
                    return;
                }
            }
            if (rand() % 3 == 2) {
                if (carte[31][31] == 4 && carte[30][31] == 4) {
                    objets.push_back({ 31, 31, 0, 0, 1, 7, vector<int>{4} });
                    return;
                }
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
    if (rand() % 100 >= 3) return;
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

void generation_velos() {
    if (rand() % 100 >= 5) return;

    int direction = rand() % 4;
    for (int i = 0; i < 4; i++) {
        switch ((direction + i) % 4) {
        case 0: // Bas
            if (carte[0][12] == 3 && carte[1][12] == 3) {
                objets.push_back({ 12, 0, 1, 0, 2, 9, vector<int>{3} });
                return;
            }
            break;

        case 1: // Haut
            if (rand() % 2 == 0) {
                if (carte[31][2] == 3 && carte[30][2] == 3) {
                    objets.push_back({ 2, 31, 0, 0, 2, 9, vector<int>{3} });
                    return;
                }
            }
            else {
                if (carte[31][17] == 3 && carte[30][17] == 3) {
                    objets.push_back({ 17, 31, 0, 0, 2, 9, vector<int>{3} });
                    return;
                }
            }
            break;

        case 2: // Gauche
            if (carte[15][31] == 3 && carte[15][30] == 3) {
                objets.push_back({ 31, 15, 2, 0, 2, 9, vector<int>{3} });
                return;
            }
            break;

        case 3: // Droite
            if (carte[21][0] == 3 && carte[21][1] == 1) {
                objets.push_back({ 0, 21, 3, 0, 2, 9, vector<int>{3} });
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
    if (objet.id == 6 || objet.id == 8 || objet.id == 9) {
        switch (objet.direction) {
        case 0: // Haut
            if (etat_feu == 3) { break; } // Feu vert
            stop = true;
            break;
        case 1: // Bas
            if (etat_feu == 3) { break; } // Feu vert
            stop = true;
            break;
        case 2: // Gauche
            if (etat_feu == 1) { break; } // Feu vert
            stop = true;
            break;
        case 3: // Droite
            if (etat_feu == 1) { break; } // Feu vert
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

void gestion_rotation(Objet& objet) {
    bool permission = false;
    if (objet.id == 6) {
        if (objet.direction == 0) { //Haut
            if ((objet.y == 19 || objet.y == 20) && (objet.x != 1) && rand() % 5 == 0) {
                objet.x++; //Dans ces conditions ce sera toujours tourné vers la droite
                objet.y = 19;
                objet.direction = 3;
                carte[19][16] = 1; carte[20][16] = 1; carte[21][16] = 1; //On enlève l'ancienne position de la voiture
            }
            else if ((objet.y == 23) && (objet.x == 1)) {
                for (int l = 0; l < 4; l++) { //S'il n'y a pas déjà une voiture à cet endroit
                    if (carte[19][l] == 1) {
                        permission = true;
                    }
                    else {
                        permission = false;
                        break;
                    }
                }
                if (permission) {
                    objet.x++; //Dans ces conditions ce sera toujours tourné vers la droite
                    objet.y = 19;
                    objet.direction = 3;
                    carte[23][1] = 1; carte[24][1] = 1; carte[25][1] = 1; //On enlève l'ancienne position de la voiture
                }

            }
        }
        else if (objet.direction == 1) { //Bas
            if ((objet.y == 16 || objet.y == 17) && rand() % 5 == 0) {
                objet.x--; //Dans ces conditions ce sera toujours tourné vers la droite
                objet.y = 17;
                objet.direction = 2;
                carte[15][15] = 1; carte[16][15] = 1; carte[17][15] = 1; //On enlève l'ancienne position de la voiture
            }
        }
        else if (objet.direction == 3) { //Droite
            if ((objet.x == 14 || objet.x == 15) && rand() % 5 == 0) {
                objet.y++; //Dans ces conditions ce sera toujours tourné vers la droite
                objet.x = 15;
                objet.direction = 1;
                carte[19][13] = 1; carte[19][14] = 1; carte[19][15] = 1; //On enlève l'ancienne position de la voiture
            }
            else if ((objet.x == 29 || objet.x == 30) && rand() % 5 == 0) {
                objet.y++; //Dans ces conditions ce sera toujours tourné vers la droite
                objet.x = 30;
                objet.direction = 1;
                carte[19][28] = 1; carte[19][29] = 1; carte[19][30] = 1; //On enlève l'ancienne position de la voiture
            }
        }
        else if (objet.direction == 2) { //Gauche
            if ((objet.x == 16 || objet.x == 17) && rand() % 5 == 0) {
                objet.y--; //Dans ces conditions ce sera toujours tourné vers la droite
                objet.x = 16;
                objet.direction = 0;
                carte[17][16] = 1; carte[17][17] = 1; carte[17][18] = 1; //On enlève l'ancienne position de la voiture
            }
        }
    }
    else if (objet.id == 7) {
        if (objet.direction == 0) { //Haut
            if ((objet.y == 22 && objet.x == 18) && rand() % 5 == 0 ) {
                objet.direction = 3;
            }
            else if ((objet.y == 22 && objet.x == 3)) {
                objet.direction = 3;
            }
            else if ((objet.y == 22 && objet.x == 31)) {
                objet.direction = 3;
            }
        }
        else if (objet.direction == 1) { //Bas
            if ((objet.y == 14 && objet.x == 13) && rand() % 5 == 0) {
                objet.direction = 2;
            }
        }
        else if (objet.direction == 2) { //Gauche
            if ((objet.y == 14 && objet.x == 18) && rand() % 5 == 0) {
                objet.direction = 0;
            }
        }
        else if (objet.direction == 3) { //Droite
            if ((objet.y == 22 && objet.x == 0) && rand() % 5 == 0) {
                objet.direction = 1;
            }
            else if ((objet.y == 22 && objet.x == 13) && rand() % 5 == 0) {
                objet.direction = 1;
            }
            else if ((objet.y == 22 && objet.x == 28) && rand() % 5 == 0) {
                objet.direction = 1;
            }
        }
    }
    else if (objet.id == 9) {
        if (objet.direction == 0) { //Haut
            if ((objet.y == 21 && objet.x == 17) && rand() % 5 == 0) {
                objet.direction = 3;
            }
            else if ((objet.y == 21 && objet.x == 2)) {
                objet.direction = 3;
            }
            else if ((objet.y == 22 && objet.x == 2)) {
                objet.direction = 3;
                objet.y--;
                carte[22][2] = 3;
            }
            else if ((objet.y == 21 && objet.x == 30)) {
                objet.direction = 3;
            }
        }
        else if (objet.direction == 1) { //Bas
            if ((objet.y == 15 && objet.x == 14) && rand() % 5 == 0) {
                objet.direction = 2;
            }
        }
        else if (objet.direction == 2) { //Gauche
            if ((objet.y == 15 && objet.x == 17) && rand() % 5 == 0) {
                objet.direction = 0;
            }
        }
        else if (objet.direction == 3) { //Droite
            if ((objet.y == 21 && objet.x == 1) && rand() % 5 == 0) {
                objet.direction = 1;
            }
            else if ((objet.y == 21 && objet.x == 14) && rand() % 5 == 0) {
                objet.direction = 1;
            }
            else if ((objet.y == 21 && objet.x == 29) && rand() % 5 == 0) {
                objet.direction = 1;
            }
        }
    }
}

void deplacement() {

    for (size_t i = 0; i < objets.size(); i++) {
        Objet& objet = objets[i];

        //On regarde si l'objet tourne
        gestion_rotation(objet);

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
                for (int i = -1; i < objet.vitesse; i++) {
                    if (!collision) {
                        for (int k = 6; k < 10; k++) {
                            if (collision) {
                                break;
                            }
                            else {
                                if (objet.id == 7) {
                                    collision = carte[newY - 1][newX] == k;
                                }
                                else if (objet.id == 9) {
                                    collision = carte[newY - i][newX] == k;
                                    if (collision) { break; }
                                }
                                else {
                                    collision = carte[newY - i][newX] == k;
                                    if (collision) { break; }
                                    collision = carte[newY - i][newX + 1] == 7; //S'il y a un piéton, on le laisse passer
                                    if (collision) { break; }
                                    collision = carte[newY - i][newX - 1] == 7;
                                    if (collision) { break; }
                                    collision = carte[newY - i + 1][newX - 1] == 7;
                                    if (collision) { break; }
                                    collision = carte[newY - i + 1][newX + 1] == 7;
                                }
                            }
                        }
                    }
                }
            }
            break;
        case 1: // Bas
            if (newY <= 28) {
                for (int i = -1; i < objet.vitesse; i++) {
                    if (!collision) {
                        for (int k = 6; k < 10; k++) {
                            if (collision) {
                                break;
                            }
                            else {
                                if (objet.id == 7) {
                                    collision = carte[newY + 1][newX] == k;
                                }
                                else if (objet.id == 9) {
                                    collision = carte[newY + i][newX] == k;
                                    if (collision) { break; }
                                }
                                else {
                                    collision = carte[newY + i][newX] == k;
                                    if (collision) { break; }
                                    collision = carte[newY + i][newX + 1] == 7; //S'il y a un piéton, on le laisse passer
                                    if (collision) { break; }
                                    collision = carte[newY + i][newX - 1] == 7;
                                    if (collision) { break; }
                                    collision = carte[newY + i - 1][newX - 1] == 7;
                                    if (collision) { break; }
                                    collision = carte[newY + i - 1][newX + 1] == 7;
                                }
                            }
                        }
                    }
                }
            }
            break;
        case 2: // Gauche
            if (newX >= 3) {
                for (int i = -1; i < objet.vitesse; i++) {
                    if (!collision) {
                        for (int k = 6; k < 10; k++) {
                            if (collision) {
                                break;
                            }
                            else {
                                if (objet.id == 7) {
                                    collision = carte[newY][newX - 1] == k;
                                }
                                else if (objet.id == 9) {
                                    collision = carte[newY][newX - i] == k;
                                    if (collision) { break; }
                                }
                                else {
                                    collision = carte[newY][newX - i] == k;
                                    if (collision) { break; }
                                    collision = carte[newY + 1][newX - i] == 7; //S'il y a un piéton, on le laisse passer
                                    if (collision) { break; }
                                    collision = carte[newY - 1][newX - i] == 7;
                                    if (collision) { break; }
                                    collision = carte[newY - 1][newX - i + 1] == 7;
                                    if (collision) { break; }
                                    collision = carte[newY + 1][newX - i + 1] == 7;
                                    if (collision) { break; }
                                    collision = carte[newY - 1][newX - i + 2] == 7;
                                    if (collision) { break; }
                                    collision = carte[newY + 1][newX - i + 2] == 7;
                                }
                            }
                        }
                    }
                }
            }
            break;
        case 3: // Droite
            if (newX <= 28) {
                for (int i = -1; i < objet.vitesse; i++) {
                    if (!collision) {
                        for (int k = 6; k < 10; k++) {
                            if (collision) {
                                break;
                            }
                            else {
                                if (objet.id == 7) {
                                     collision = carte[newY][newX + 1] == k;
                                }

                                else if (objet.id == 9) {
                                    collision = carte[newY][newX + i] == k;
                                    if (collision) { break; }
                                }
                                else {
                                    collision = carte[newY][newX + i] == k;
                                    if (collision) { break; }
                                    collision = carte[newY + 1][newX + i] == 7; //S'il y a un piéton, on le laisse passer
                                    if (collision) { break; }
                                    collision = carte[newY - 1][newX + i] == 7;
                                    if (collision) { break; }
                                    collision = carte[newY - 1][newX + i - 1] == 7;
                                    if (collision) { break; }
                                    collision = carte[newY + 1][newX + i - 1] == 7;
                                    if (collision) { break; }
                                    collision = carte[newY - 1][newX + i - 2] == 7;
                                    if (collision) { break; }
                                    collision = carte[newY + 1][newX + i - 2] == 7;
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

        if (stop || objet.x == 1) {
            // Calcul de la distance
            if (objet.id == 6 || objet.id == 8 || objet.id == 9) {
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
