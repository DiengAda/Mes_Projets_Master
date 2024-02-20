#include<iostream>
#include "../HPP/Vecteur.hpp"



/*
    cette classe vecteur est créée pour initialiser et stocker des vecteurs de taille M
    dans les differents cas du probleme(stationnaire et instationnaire)
*/

//un constructeur par défaut
Vecteur::Vecteur(int taille)
:
M(taille), M_V(new double[taille])
{
    for(int i = 0; i < taille; i++)
        M_V[i] = 0;
}

//un constructeur avec un tableau de taille M
Vecteur::Vecteur(int taille, double* v)
:
M(taille), M_V(new double[taille])
{
    for(int i = 0; i < taille; i++)
        M_V[i] = v[i];
}

