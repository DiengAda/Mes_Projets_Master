#include<iostream>
#include "../HPP/resolution.hpp"
// #include "../HPP/Matrice.hpp"
// #include "../HPP/Vecteur.hpp"
// #include "../HPP/parametres.hpp"


MatriceTridiagonale** DecompositionLU(const MatriceTridiagonale A)
{
    // on recupere les coefficients a, b et c de A
    int M = A.getM();
    double* a = A.getDiagInf();
    double* b = A.getDiag();
    double* c = A.getDiagSup();

    // on cree deux tableaux pour stocker les coefficients a*, b* et c* de L et U
    double b_etoile_L[M];
    double c_etoile_L[M-1];
    double a_etoile_L[M-1];
    double b_etoile_U[M];
    double c_etoile_U[M-1];
    double a_etoile_U[M-1]; // inutile: c'est le diagonale inf de A

    // initialisation de b*_O et c*_O
    b_etoile_L[0] = b[0];
    c_etoile_U[0] = c[0]/b_etoile_L[0];

    // on calcul  b*_k et c*_k pour k = 1, 2, ..., M-1
    for(int k = 1; k < M-1; k++)
    {
        b_etoile_L[k] = b[k] - a[k-1] * c_etoile_U[k-1];
        c_etoile_U[k] = c[k]/b_etoile_L[k];
    }
    // on calcul ensuite le dernier coefficient de la diagonale
    b_etoile_L[M-1] = b[M-1] - a[M-1] * c_etoile_U[M-2];

    // les diagonales manquantes de L et U
    for(int i = 0; i < M-1; i++)
    {
        a_etoile_U[i] = 0;
        b_etoile_U[i] = 1; //des 1 sur la diagonale de U
        a_etoile_L[i] = a[i+1]; // attention ici
        c_etoile_L[i] = 0;
    }
    b_etoile_U[M-1] = 1;
        
        
    // on peut creer un tableau de pointeurs de taille 2 pour stocker les matrices L et U
    MatriceTridiagonale** TabLU = new MatriceTridiagonale*[2];
    TabLU[0] = new MatriceTridiagonale(M, a_etoile_L, b_etoile_L, c_etoile_L); // L
    TabLU[1] = new MatriceTridiagonale(M, a_etoile_U, b_etoile_U, c_etoile_U); // U

    //on libere la memoire
    // delete[] a_etoile_L;
    // delete[] b_etoile_L;
    // delete[] c_etoile_L;
    // delete[] a_etoile_U;
    // delete[] b_etoile_U;
    // delete[] c_etoile_U;

    return TabLU;
}

/*
    Fonction pour la resolution de LY = F
    c'est la descente
*/

void ResolutionLyF(Vecteur& Y, const MatriceTridiagonale& L, const Vecteur& F)
{
    int M = L.getM();
    double* y = Y.getM_V();
    double* f = F.getM_V();
    double* b = L.getDiag();
    double* c = L.getDiagSup();
    double* a = L.getDiagInf();

    // initialisation de y_O
    y[0] = f[0]/b[0];
    // Y[0] = F[0]/L.getDiag(0); // attention avec cette synt, on se sera oblige de surcharger l'operateur [] pour la vecteur

    // on calcul y_k pour k = 1, 2, ..., M-1
    for(int k = 1; k < M; k++)
    {
        y[k] = (f[k] - a[k-1] * y[k-1])/b[k];
        // Y[k] = (F[k] - L.getDiagInf(k-1) * Y[k-1])/L.getDiag(k);
    } 
}

/*
    Fonction pour la resolution de UX = Y
    c'est la remontée
*/

void ResolutionUXY(Vecteur& X, const MatriceTridiagonale& U, const Vecteur& Y)
{

    int M = U.getM();
    double* x = X.getM_V();
    double* y = Y.getM_V();
    double* b = U.getDiag();
    double* c = U.getDiagSup();
    double* a = U.getDiagInf();


    // calcul de x_k pour k = M-2, M-3, ..., 0
    for(int k = M-2; k >= 0; k--)
    {
        x[k] = (y[k] - c[k] * x[k+1]);
    }
    // initialisation de x_M
    x[M-1] = y[M-1];
}