#include "../HPP/Matrice.hpp"




// Constructeur par défaut
MatriceTridiagonale::MatriceTridiagonale(int taille)
:
M(taille), DiagInf(new double[taille-1]), Diag(new double[taille]), DiagSup(new double[taille-1])
{
    // Initialisation à 0
    for(int i = 0; i < taille-1; i++)
    {
        DiagInf[i] = 0;
        Diag[i] = 0;
        DiagSup[i] = 0;
    }
    Diag[taille-1] = 0;
};

    // Constructeur avec initialisation des diagonales
MatriceTridiagonale::MatriceTridiagonale(int taille, double* diagInf, double* diag, double* diagSup)
:
M(taille), DiagInf(new double[taille-1]), Diag(new double[taille]), DiagSup(new double[taille-1])
{
    // Initialisation avec les valeurs fournies
    for(int i = 0; i < M-1; i++)
    {
        DiagSup[i] = diagSup[i];
    }
    for(int i = 0; i < M; i++)
    {
        Diag[i] = diag[i];
    }
    for(int i = 1; i < M; i++)
    {
        DiagInf[i] = diagInf[i-1];
    }
};

// class MatriceTridiagonale
// {
// private:
//     int M; // taille de la matrice
//     double* DiagInf;
//     double* Diag; 
//     double* DiagSup; 

// public:
//     // Constructeur par défaut
//     MatriceTridiagonale(int taille)
//     :
//     M(taille), DiagInf(new double[taille-1]), Diag(new double[taille]), DiagSup(new double[taille-1])
//     {
//         // Initialisation à 0
//         for(int i = 0; i < taille-1; i++)
//         {
//             DiagInf[i] = 0;
//             Diag[i] = 0;
//             DiagSup[i] = 0;
//         }
//         Diag[taille-1] = 0;
//     }

//     // Constructeur avec initialisation des diagonales
//     MatriceTridiagonale(int taille, double* diagInf, double* diag, double* diagSup)
//     :
//     M(taille), DiagInf(new double[taille-1]), Diag(new double[taille]), DiagSup(new double[taille-1])
//     {
//         // Initialisation avec les valeurs fournies
//         for(int i = 0; i < M-1; i++)
//         {
//             DiagSup[i] = diagSup[i];
//         }
//         for(int i = 0; i < M; i++)
//         {
//             Diag[i] = diag[i];
//         }
//         for(int i = 1; i < M; i++)
//         {
//             DiagInf[i] = diagInf[i-1];
//         }
//     }

// };

