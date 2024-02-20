#ifndef MATRICE_HPP
#define MATRICE_HPP

#include <iostream>

/*
    Je pense creer cette classe pour initialiser  et stocker des matrices de type tridiagonale
    dans les differents cas du probleme(stationnaire et instationnaire)
*/

class MatriceTridiagonale
{
    private:
        int M; // taille de la matrice
        double* DiagInf;
        double* Diag; 
        double* DiagSup;

    public:
        MatriceTridiagonale(int taille);
        MatriceTridiagonale(int taille, double* diagInf, double* diag, double* diagSup);



        // Accesseurs
        int getM() const { return M; }
        double* getDiagInf() const { return DiagInf; }
        double* getDiag() const { return Diag; }
        double* getDiagSup() const { return DiagSup; }

        double getDiagInf(int i) const { return DiagInf[i]; }
        double getDiag(int i) const { return Diag[i]; }
        double getDiagSup(int i) const { return DiagSup[i]; }

        //mutateurs
        void setDiagInf(int i, double val) { DiagInf[i] = val; }
        void setDiag(int i, double val) { Diag[i] = val; }
        void setDiagSup(int i, double val) { DiagSup[i] = val; }
        

        // Destructeur
        ~MatriceTridiagonale()
        {
            delete[] DiagInf;
            delete[] Diag;
            delete[] DiagSup;
        }


};

#endif
