#include <vector>
#include <iostream>

#include "liquido.hpp"
#include "funcoes_leitura.hpp"

Liquido::Liquido() {}
Liquido::Liquido(int id, std::string categoria, std::string nome,std::string subclasse_tipo):
	Alimento(id, categoria, nome) 
	{
		// Atribuo a subclasse ao objeto
		_subClasse = subclasse_tipo;
    }
	
Liquido::~Liquido() {}


double Liquido::getQuantidadeGramas() {
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
void Liquido::converterQuantidadeParaGramas() {
	// As unidades padrão de volume (usadas no Brasil):
	// Xícara (chá) = 240 mL
	// Copo Americano = 190 mL
	// Colher de Sopa = 15 mL
	// Colher de Sobremesa = 10 mL
	// Colher de Chá = 5 mL
	// Litro = 1000 mL
	
	// Caso especifico - consigo converter as unidades informados pelo usuario
	// para mL, mas preciso da densidade para converter para gramas
	
	if (_unidadeMedida == "Gramas") // caso o usuario ja tenha colocado em gramas
		return; // nao precisa de conversao

    // 1. Definições e Variáveis Locais
    double volume_total_mL = 0.0;
    double densidade_g_por_mL = 0.0;
    
    // 2. Conversão de Volume para Mililitros (mL)

    if (_unidadeMedida == "Xicara") {
        volume_total_mL = _quantidade * 240.0; // Padrão: 240 mL
    } else if (_unidadeMedida == "Meia Xicara") {
        volume_total_mL = _quantidade * 120.0; // 240 / 2 = 120 mL
    } else if (_unidadeMedida == "Quarto Xicara") {
        volume_total_mL = _quantidade * 60.0;  // 240 / 4 = 60 mL
    } else if (_unidadeMedida == "Copo Americano") {
        volume_total_mL = _quantidade * 190.0; // Padrão: 190 mL
    } else if (_unidadeMedida == "Colher de Sopa") {
        volume_total_mL = _quantidade * 15.0;  // Padrão: 15 mL
    } else if (_unidadeMedida == "Colher de Sobremesa") {
        volume_total_mL = _quantidade * 10.0;  // Padrão: 10 mL
    } else if (_unidadeMedida == "Colher de Chá") {
        volume_total_mL = _quantidade * 5.0;   // Padrão: 5 mL
    } else if (_unidadeMedida == "Litro") {
        volume_total_mL = _quantidade * 1000.0; // 1 L = 1000 mL
    } else if (_unidadeMedida == "Mililitros") {
        volume_total_mL = _quantidade;        // Já está em mL
    } else {
        std::cerr << "ERRO Fatal: Unidade de medida nao reconhecida: " << _unidadeMedida << std::endl;
		exit(0);
    }
    
    // 3. Obter a Densidade do Líquido 
    // A densidade (g/mL) é necessária para converter Volume para Massa (Gramas).
    std::cout << "\nNao é possivel converter volume (mL) para massa (gramas) sem saber a densidade do liquido.\n";
    while (true) {
        std::cout << "Informe a densidade de " << _nome << " (em g/mL, valor decimal com ponto): ";
        densidade_g_por_mL = ler_double(); 
        if (!densidade_g_por_mL)
            break; 
    }

    // 4. Conversao final para gramas
    _quantidadeGramas =  volume_total_mL * densidade_g_por_mL; 
}