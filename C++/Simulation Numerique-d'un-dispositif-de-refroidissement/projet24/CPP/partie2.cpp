#include<iostream>
#include<fstream>
#include<cmath>

#include "../HPP/resolution.hpp"

/*
    Dans cette partie, on va creer une classe pour le modele stationnaira.
    On devra alors inclure les classes MatriceTridiagonale, Vecteur, parametres
    et probabablement les foncion de la resolution de systeme lineaire.

    // Dans ce cas stationnaire, on aura besoin de la matrice A, du vecteur F, des solutions
    // exactes et numeriques.
*/
class ModeleStationnaire
{
    private:
        Parametres* M_parametres;
        MatriceTridiagonale *matriceA, *L, *U;
        Vecteur* secondMembreF;
        Vecteur* SolutionNumerique;
    public:
        // Constructeur par défaut
        ModeleStationnaire(Parametres* p)
        :
        M_parametres(p)
        {
            // on initialise le vecteur solution
            int M = M_parametres->M;
            SolutionNumerique = new Vecteur(M + 1);
        }


        // #ifdef DEBUG
        // Methode pour definir la matrice A pour le cas stationnaire
        void A_stationnaire()
        {

            /*
                les parametres de la matrice A:
                on aura besoin de M, S = ly * lz, p = 2(lx + ly), K, hc et h = lx/M
            */
            int M = M_parametres->M;
            double  S = M_parametres->Ly*M_parametres->Lz;
            double p = 2 * (M_parametres->Ly + M_parametres->Ly);
            double hc = M_parametres->hc, h = M_parametres->Lx/M;
            double K = M_parametres->K;
            std::cout << "h = " << h << std::endl;
            std::cout << "S = " << S << std::endl;

            // on cree les diagonales de la matrice A
            double a_A[M-1], b_A[M], c_A[M-1];
            // double * a_A = new double[M-1];
            // double * b_A = new double[M];
            // double * c_A = new double[M-1];


            // la diagonale: les b_i
            for(int i = 1; i < M-1; i++)
            {
                b_A[i] = 2 * K / std::pow(h, 2) + hc * p / S;
            } 
            b_A[0] = K / h;
            b_A[M-1] = K / h;

            // ensuite la diagonale inferieure: les a_i
            for(int i = 0; i < M-2; i++)
            {
                a_A[i] = -K / std::pow(h, 2);
            }
            a_A[M-2] = -K / h;

            // enfin la diagonale superieure: les c_i
            for(int i = 1; i < M; i++)
            {
                c_A[i] = -K / std::pow(h, 2);
            }
            c_A[0] = -K / h;

            // on cree la matrice A
            matriceA = new MatriceTridiagonale(M, a_A, b_A, c_A);

            //affichage de la matrice A
            std::cout << "Matrice A : " << std::endl;
            for (int i = 0; i < M; i++)
            {
                for (int j = 0; j < M; j++)
                {
                    if (j == i)
                        std::cout <<matriceA->getDiag(i) << "\t";
                    else if (i == j - 1)
                        std::cout << matriceA->getDiagSup(i) << "\t";
                    else if (i == j + 1)
                        std::cout << matriceA->getDiagInf(i) << "\t";
                    else
                        std::cout << "0\t";
                }
                std::cout << std::endl;
            }

            // delete
            // delete[] a_A;
            // delete[] b_A;
            // delete[] c_A;

        }

        // #endif
        // #ifdef DEBUG

        // Methode pour definir le vecteur F pour le cas stationnaire
        void F_stationnaire()
        {
            /*
                On recupere les parametres qui interviennent dans le calcul  de F
                on aura besoin de M, S = ly * lz, p = 2(lx + ly), hc, Te, phi_p;
            */
            int M = M_parametres->M;
            double S = M_parametres->Ly * M_parametres->Lz;
            double p = 2 * (M_parametres->Ly + M_parametres->Lz);
            double hc = M_parametres->hc, Te = M_parametres->Te, phi_p = M_parametres->Phi_p;

            // on cree le vecteur 
            double F[M];

            // on initialise les elements de F: les F_i
            for(int i = 1; i < M-1; i++)
            {
                F[i] = (hc * p  * Te) / S;
            }
            F[0] = phi_p;
            F[M-1] = 0;

            // on cree le vecteur F
            secondMembreF = new Vecteur(M, F); // vecteur de  

            //affichage du vecteur F
            std::cout << "Vecteur F : " << std::endl;
            for (int i = 0; i < M; i++)
            {
                std::cout << secondMembreF->getM_V()[i] << std::endl;
            }

        }

        // #endif


        #ifdef DEBUG
        // Methode pour la resolution numerique du modele stationnaire
        void ResolutionNumerique()
        {
            int M = M_parametres->M;

            // On initialise L et U et un vecteur Y
            MatriceTridiagonale** TabLU = DecompositionLU(*matriceA);
            L = TabLU[0];
            U = TabLU[1];

            Vecteur* Y = new Vecteur(M+1);

            // On resoud LY = F
            ResolutionLyF(*Y, *L, *secondMembreF);
            std::cout << "Vecteur Y : " << std::endl;
            for (int i = 0; i < M; i++)
            {
                std::cout << Y->getM_V()[i] << std::endl;
            }

            // On resoud UX = Y
            ResolutionUXY(*SolutionNumerique, *U, *Y);
            std::cout << "Vecteur X : " << std::endl;
            for (int i = 0; i < M; i++)
            {
                std::cout << SolutionNumerique->getM_V()[i] << std::endl;
            }

            // on libere la memoire
            // delete[] TabLU;
            // delete [] L;
            // delete [] U;
            // delete [] Y;

        }
        #endif


        // methode pour la solution exacte du modele stationnaire T_exact
        Vecteur* Solution_T_exact()
        {
            // On recupere les parametres qui interviennent
            int M = M_parametres->M;
            double Lx = M_parametres->Lx, Ly = M_parametres->Ly, Lz = M_parametres->Lz;
            double K = M_parametres->K, hc = M_parametres->hc, Te = M_parametres->Te, phi = M_parametres->Phi_p;
            double h = Lx/M, S = Ly * Lz, p = 2 * (Ly + Lz);

            // on cree le vecteur solution
            double T_exact[M+1];

            // on initialise les elements de T_exact: les T_exact_i
            double a = (hc * p)/(K * S);
            for(int i = 0; i < M+1; i++)
            {
                T_exact[i] = Te + (phi * std::cosh(std::sqrt(a)*Lx) * std::cosh(std::sqrt(a)*(Lx - i*h))) / 
                                  (K * std::sqrt(a) * std::sinh(std::sqrt(a)*Lx) * std::cosh(std::sqrt(a)*Lx));
            } 

            // on cree le vecteur solution et y stocke les elements de T_exact et on le retourne
            Vecteur* solution = new Vecteur(M+1, T_exact);

            return solution;
        }

        // methode ecrire dans un fichier CSV
        void EcrireCSV()
        {
            int M = M_parametres->M;
            double h = M_parametres->Lx / M;
            Vecteur* T = Solution_T_exact();
            

            // On crée le fichier
            std::ostringstream osrtr;
            osrtr << "../CSV/solutions.csv"; // on donne un nom au fichier pour Lx donné
            std::ofstream ofile(osrtr.str());
            
            if (ofile)
            {
                ofile << "x, sol1, sol2\n";

                // Écrire les données dans le fichier CSV
                for(int i=0; i<M+1; i++)
                {
                    ofile << i*h << ", " << 1 << ", " << (*T)[i] << "\n";
                }
            }
            ofile.close();
            std::cout << "Fichier CSV créé avec succes" << std::endl;
            delete T;
        }



};

int main()
{
    // on cree les parametres
    Parametres* parametres = new Parametres("../CFG/simu.cfg");
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

    // on cree le modele stationnaire
    ModeleStationnaire* modele = new ModeleStationnaire(parametres);


    #ifdef DEBUG
    // on initialise la matrice A
    modele->A_stationnaire();
    #endif

    //on affiche la matrice A

    // on initialise le vecteur F
    modele->F_stationnaire();

    #ifdef DEBUG
    // on resoud le modele stationnaire
    modele->ResolutionNumerique();
    #endif

    #ifdef DEBUG
    // on affiche la solution exacte
    std::cout << "Solution exacte : " << std::endl;
    Vecteur* solution_exacte = modele->Solution_T_exact();
    for (int i = 0; i < parametres->M; i++)
    {
        std::cout << solution_exacte->getM_V()[i] << std::endl;
    }
    #endif


    // on ecrire dans un fichier CSV
    modele->EcrireCSV();


    return 0;
}