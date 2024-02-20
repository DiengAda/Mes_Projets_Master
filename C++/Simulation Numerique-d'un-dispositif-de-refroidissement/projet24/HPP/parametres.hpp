#ifndef PARAMETRES_HPP
#define PARAMETRES_HPP

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

/*
    Je cree ici une class Parametres qui va me permettre de lire un fichier
    de configuration et d'initialiser les paramètres du problème par les valeurs lues
*/
class Parametres
{
    public:
        double Lx, Ly, Lz, Phi_p, hc; 
        int M, Mx, My, Mz, N, Te,TFinal;
        const int rho = 2700, K = 164, Cp = 940;
        bool stationary;
        /*
            le Constructeur permettant de lire un fichier et 
            d'initialiser les paramètres du problème
        */
        Parametres(std::string filename);
        
};

#endif