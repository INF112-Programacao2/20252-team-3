#include <vector>
#include <iostream>

#include "solido_contavel.hpp"
#include "funcoes_leitura.hpp"

SolidoContavel::SolidoContavel() {}
SolidoContavel::SolidoContavel(int id, std::string categoria, std::string nome, std::string subclasse_tipo):
	Alimento(id, categoria, nome) 
	{
		// Atribuo a subclasse ao objeto
		_subClasse = subclasse_tipo;
    }
	
SolidoContavel::~SolidoContavel() {}

double SolidoContavel::getQuantidadeGramas() {
	if (_unidadeMedida != "") {
		converterQuantidadeParaGramas();
		return _quantidadeGramas;
	}
	else {
		std::cerr << "ERRO Fatal: Nao é possivel converter a quantidade para gramas sem saber a unidade de medida\n";
		std::cerr << "No codigo so deve ser chamado a funcao getQuantidadeGramas() depois de definir a unidade de medida\n";
		exit(0);
	}
}
void SolidoContavel::converterQuantidadeParaGramas() {
	double gramas_unidade;
	
	// Caso especifico - nao tem como saber qual o peso (em gramas) da unidade
	// de um alimento qualquer, é preciso saber qual o peso medio desse alimento
	
	if (_unidadeMedida == "Gramas") // caso o usuario ja tenha colocado em gramas
		return; // nao precisa de conversao
	
	// 1. Leio o peso em gramas de 1 unidade de alimento
	std::cout << "Nao é possivel fazer a conversao direta de " << _unidadeMedida << " para gramas\n";
	while (true) {
		std::cout << "Informe qual o peso medio da unidade de " << _nome << " (valor decimal com ponto): ";
		gramas_unidade = ler_double();
		if (!gramas_unidade)
			continue;
	}
	
	// 2. Aplicar a lógica de conversão
    if (_unidadeMedida == "Unidades") {
        // Conversão: Quantidade * Gramas por Unidade
        _quantidadeGramas =  _quantidade * gramas_unidade;
    
    } else if (_unidadeMedida == "Media Duzia") {
        // Conversão: Quantidade * (6 Unidades/Media Duzia) * Gramas por Unidade
        _quantidadeGramas = _quantidade * 6.0 * gramas_unidade;

    } else if (_unidadeMedida == "Duzia") {
        // Conversão: Quantidade * (12 Unidades/Duzia) * Gramas por Unidade
        _quantidadeGramas =  _quantidade * 12.0 * gramas_unidade;

    } else {
        std::cerr << "ERRO Fatal: Unidade de medida nao reconhecida: " << _unidadeMedida << std::endl;
		exit(0);
    }
}