#ifndef DIARIO_ALIMENTAR_H
#define DIARIO_ALIMENTAR_H

#include <vector>
#include <string>
#include <sstream>

class DiarioAlimentar {
    private:
        std::vector<AlimentoAB> _vectorAlimentos;
        std::string lerCampoCSV(std::stringstream& ss);
    public:

        int* buscarAlimentoPeloNome(std::string nome_alimento);
        std::string buscarAlimentoPeloID(int id);
        void adicionarAlimentoAoVetor(AlimentoAB alimento);
        void exibirAlimentos()
};

#endif