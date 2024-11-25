// Projet.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets.

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace std;
using namespace sf;

RenderWindow window(VideoMode(800, 600), "Projet CPP");

typedef struct Voiture{
	int longueur = 2;
	int vitesse = 3;
	int id = 6;
}Voiture;
