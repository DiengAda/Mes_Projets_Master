#ifndef VECTEUR_HPP
#define VECTEUR_HPP


#include<iostream>
// #include "../HPP/Vecteur.hpp"


/*
    cette classe vecteur est créée pour initialiser et stocker des vecteurs de taille M
    dans les differents cas du probleme(stationnaire et instationnaire)
*/

class Vecteur
{
    private:
        int M; // taille du vecteur
        double* M_V; // tableau de taille M


    public:
        //un constructeur par défaut
        Vecteur(int taille);

        //un constructeur avec un tableau de taille M
        Vecteur(int taille, double* v);

        // accesseurs
        int getM() const
        { 
            return M;
        }
        
        double* getM_V() const
        {       
            return M_V;
        }

        // On surcharge l'operateur [] pour pouvoir acceder aux elements du vecteur
        double& operator[](int i)
        {
            return M_V[i];
        }

        double const & operator[](int i) const
        {
            return M_V[i];
        }

        // destructeur
        ~Vecteur()
        {
            delete[] M_V;
        }

};

#endif