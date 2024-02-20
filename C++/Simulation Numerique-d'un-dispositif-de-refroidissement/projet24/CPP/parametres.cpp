#include "../HPP/parametres.hpp"


Parametres::Parametres(std::string filename)
{
    std::ifstream file(filename);
    if (file)
    {
        std::string chaine_de_caractere;
        file >> chaine_de_caractere >> Lx;
        file >> chaine_de_caractere >> Ly;
        file >> chaine_de_caractere >> Lz;
        file >> chaine_de_caractere >> M;
        file >> chaine_de_caractere >> Phi_p;
        file >> chaine_de_caractere >> hc;
        file >> chaine_de_caractere >> Te;
        file >> chaine_de_caractere >> stationary;
        file >> chaine_de_caractere >> TFinal;
        file >> chaine_de_caractere >> N;
        file >> chaine_de_caractere >> Mx;
        file >> chaine_de_caractere >> My;
        file >> chaine_de_caractere >> Mz;
    }
}
    