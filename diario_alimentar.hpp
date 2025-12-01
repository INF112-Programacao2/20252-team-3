#ifndef DIARIO_ALIMENTAR_H
#define DIARIO_ALIMENTAR_H

#include <vector>
#include <string>
#include <sstream>
#include <memory>

#include "alimento.hpp"

class DiarioAlimentar {
    private:
        std::vector<std::unique_ptr<Alimento>> _vectorAlimentos; 
        std::string lerCampoCSV(std::stringstream& ss);
    public:

        int* buscarAlimentoPeloNome(std::string nome_alimento);
        std::string buscarAlimentoPeloID(int id);
        void adicionarAlimentoAoVetor(Alimento alimento);
        void exibirAlimentos();
		void preencherAlimentosNoVetor();
		bool adicionarAlimentoAoDB(Alimento novo_alimento);
		bool buscarAlimentoNoDiario(std::string nome_alimento);
		std::string getSubClasse(std::string nome_alimento);
		void exibirInformacoes(std::string nome_alimento);
};

#endif