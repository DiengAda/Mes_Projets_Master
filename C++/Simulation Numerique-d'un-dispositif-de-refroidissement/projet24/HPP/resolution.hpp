#ifndef RESOLUTION_HPP
#define RESOLUTION_HPP

#include <iostream>
#include <iostream>
#include "Matrice.hpp"
#include "Vecteur.hpp"
#include "parametres.hpp"

/*
    cette fonction permettra d'effectuer la decomposition d'une matrice
    tridiaognale en un produit de LU par la mehtode de Gauss
*/



MatriceTridiagonale** DecompositionLU(const MatriceTridiagonale A);

/*
    Fonction pour la resolution de LY = F
    c'est la descente
*/
void ResolutionLyF(Vecteur& Y, const MatriceTridiagonale& L, const Vecteur& F);

/*
    Fonction pour la resolution de UX = Y
    c'est la remontée
*/

void ResolutionUXY(Vecteur& X, const MatriceTridiagonale& U, const Vecteur& Y);

void CSV();



#endif

