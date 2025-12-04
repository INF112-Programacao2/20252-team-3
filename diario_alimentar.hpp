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
		std::vector<std::unique_ptr<Alimento>> _vectorAlimentosDoUsuario; 
    public:

		DiarioAlimentar();
		~DiarioAlimentar();

        std::string lerCampoCSV(std::stringstream& ss);
		std::vector<std::unique_ptr<Alimento>> getAlimentos();
		Alimento* getAlimento(const std::string& nome_alimento) const; 
		
        int* buscarAlimentoPeloNome(std::string nome_alimento);
        std::string buscarAlimentoPeloID(int id);
        void adicionarAlimentoAoVetorDoUsuario(Alimento alimento);
		void adicionarAlimentoAoVetorDoUsuario(std::unique_ptr<Alimento> alimentoPtr);
        void exibirAlimento();
		void preencherAlimentoNoVetor();
		void adicionarAlimentoAoDB(std::unique_ptr<Alimento> novo_alimento_ptr);
		bool buscarAlimentoNoDiario(std::string nome_alimento);
		std::string getSubClasse(std::string nome_alimento);
		void exibirInformacoes(std::string nome_alimento);
		void exibirNutrientesNoDiario(const std::string& refeicao_alvo, const std::string& tipo_nutriente_alvo);
		void exibirAlimentosNoDiario(const std::string& refeicao_filtro, const std::string& categoria_filtro);
		void exibirAlimentosNoDiario();
		void removerAlimentoDoDiario(const std::string& nome_alimento);
		void preencherAlimentosNoVetor();
		void exibirAlimentos();

};

#endif