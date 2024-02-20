#include "../HPP/resolution.hpp"


/*
    Dans ce programm, on teste  les classes MatriceTridiagonale et Vecteur
    ainsi que les autres fonctions (LU, LY, UX etc) qu'on a defnies avant de les utiliser
    pour les donnees du projet.

    Pour cela on definit une matrice A simple de taille 5*5 et un vecteur F de taille 5
*/

int main(int argc, char* argv[])
{
    // fichier de configuration
    std::string filename = "../CFG/simu.cfg";
    if (argc > 1)
        filename = argv[1];

    // testons si les paramètres sont bien lus et initialisés
    Parametres* parametres = new Parametres(filename);
    #ifdef DEBUG
    std::cout << "Lx = " << parametres->Lx << std::endl;
    std::cout << "Ly = " << parametres->Ly << std::endl;
    std::cout << "Lz = " << parametres->Lz << std::endl;
    std::cout << "Phi = " << parametres->Phi_p << std::endl;
    std::cout << "hc = " << parametres->hc << std::endl;
    std::cout << "Te = " << parametres->Te << std::endl;
    std::cout << "stationary = " << parametres->stationary << std::endl;
    std::cout << "TFinal = " << parametres->TFinal << std::endl;
    std::cout << "N = " << parametres->N << std::endl;
    std::cout << "Mx = " << parametres->Mx << std::endl;
    std::cout << "My = " << parametres->My << std::endl;
    std::cout << "Mz = " << parametres->Mz << std::endl;
    std::cout << "rho = " << parametres->rho << std::endl;
    std::cout << "K = " << parametres->K << std::endl;
    std::cout << "Cp = " << parametres->Cp << std::endl;
    std::cout << "M = " << parametres->M << std::endl;
    #endif

    // test de la class matrice A, et la decomposition LU
    int taille = parametres->M;
    double a[taille-1];
    double b[taille];
    double c[taille-1];

   // Initialisation des tableaux a, b, et c
    for (int i = 0; i < taille; i++)
    {
        if (i < taille-1)
        {
            a[i] = 6;
            c[i] = 2;
        }
        b[i] = 4;
    }

    // test de la class matrice A avec le constructeur par defaut   
    #ifdef DEBUG 
    MatriceTridiagonale* A = new MatriceTridiagonale(taille);
    // Initialisation de la matrice A
    for (int i = 0; i < taille+1; i++)
    {
        if (i < taille)
        {
            A->setDiagInf(i, 3);
            A->setDiagSup(i, 4);
        }
        A->setDiag(i, 2);
    }
    #endif

    // avec le constructeur initialisant les diagonales
    MatriceTridiagonale* A = new MatriceTridiagonale(taille, a, b, c);

    // #ifdef DEBUG
    std::cout << "Matrice A : " << std::endl;
    for (int i = 0; i < taille; i++)
    {
        for (int j = 0; j < taille; j++)
        {
            if (j == i)
                std::cout << A->getDiag(i) << "\t";
            else if (i == j - 1)
                std::cout << A->getDiagSup(i) << "\t";
            else if (i == j + 1)
                std::cout << A->getDiagInf(i) << "\t";
            else
                std::cout << "0\t";
        }
        std::cout << std::endl;
    }
    // #endif
    // test de la decomposition LU
    MatriceTridiagonale* L = new MatriceTridiagonale(taille);
    MatriceTridiagonale* U = new MatriceTridiagonale(taille);
    MatriceTridiagonale** FactoLU = DecompositionLU(*A);
    L = FactoLU[0];
    U = FactoLU[1];

    // on affiche les matrices L et U
    // #ifdef DEBUG
    std::cout << "Matrice L : " << std::endl;
    for (int i = 0; i < taille; i++)
    {
        for (int j = 0; j < taille; j++)
        {
            if (j == i)
                std::cout << L->getDiag(i) << "\t";
            else if (i == j - 1)
                std::cout << L->getDiagSup(i) << "\t";
            else if (i == j + 1)
                std::cout << L->getDiagInf(i) << "\t";
            else
                std::cout << "0\t";
        }
        std::cout << std::endl;
    }

    std::cout << "Matrice U : " << std::endl;
    for (int i = 0; i < taille; i++)
    {
        for (int j = 0; j < taille; j++)
        {
            if (j == i)
                std::cout << U->getDiag(i) << "\t";
            else if (i == j - 1)
                std::cout << U->getDiagSup(i) << "\t";
            else if (i == j + 1)
                std::cout << U->getDiagInf(i) << "\t";
            else
                std::cout << "0\t";
        }
        std::cout << std::endl;
    }
    // #endif

    // test de la class vecteur
    #ifdef DEBUG
    double v[5];
    for (int i = 0; i < 5; i++)
        v[i] = i+2;
    Vecteur* V = new Vecteur(5, v);

    std::cout << "Vecteur V : " << std::endl;
    for (int i = 0; i < 5; i++)
        std::cout << V->getM_V()[i] << std::endl;
    #endif

    // on resout maintenant le systeme AX = F en utilisant la decomposition LU et les fonction de remontee etde descente
    int taille_vecteur = taille; // taille du vecteur = taille de la matrice
    Vecteur* Y = new Vecteur(taille_vecteur);
    Vecteur* F = new Vecteur(taille_vecteur);
    Vecteur* X = new Vecteur(taille_vecteur);

    // On considere que F est un vecteur de 1
    double f[taille_vecteur];
    for (int i = 0; i < taille_vecteur; i++)
        f[i] = 1;
    F = new Vecteur(taille_vecteur, f);

    // #ifdef DEBUG
    std::cout << "Vecteur F : " << std::endl;
    for (int i = 0; i < taille_vecteur; i++)
        std::cout << F->getM_V()[i] << std::endl;
    std::cout << "Vecteur Y initialelement nul: " << std::endl;
    for (int i = 0; i < taille_vecteur; i++)
        std::cout << Y->getM_V()[i] << std::endl;
    // #endif


    // test de la fonction ResolutionLyF
    ResolutionLyF(*Y, *L, *F);
    // #ifdef DEBUG
    std::cout << "Vecteur Y trouvé pour LY=F: " << std::endl;
    for (int i = 0; i < taille_vecteur; i++)
        std::cout << Y->getM_V()[i] << std::endl;
    // #endif

    // test de la fonction ResolutionUXY
    ResolutionUXY(*X, *U, *Y);
    #if DEBUG
    std::cout << "Vecteur solution X : " << std::endl;
    for (int i = 0; i < taille_vecteur; i++)
        std::cout << X->getM_V()[i] << std::endl;
    #endif

    // // test de la fonction csv
    // CSV();




    return 0;
}