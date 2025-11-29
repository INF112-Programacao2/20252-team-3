#ifndef DIARIO_ALIMENTAR_H
#define DIARIO_ALIMENTAR_H

#include <vector>
#include <string>
#include <sstream>

#include "alimento.hpp"

class DiarioAlimentar {
    private:
        std::vector<Alimento> _vectorAlimentos;
        std::string lerCampoCSV(std::stringstream& ss);
    public:

        int* buscarAlimentoPeloNome(std::string nome_alimento);
        std::string buscarAlimentoPeloID(int id);
        void adicionarAlimentoAoVetor(Alimento alimento);
        void exibirAlimentos();
};

#endif