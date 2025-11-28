#include "receita.hpp"

void adicionarReceita() {
	std::string nome_ingrediente;
	int quantidade;
	std::string medida;
	
	std::cout << "\nCriando nova receita.........\n";
	
	int i = 1;
	while (true) {
		std::cout << "Ingrediente " << i << std::endl;
		while (true) {
			std::cout << "Nome: ";
			nome_ingrediente = ler_nome();
			if (nome_ingrediente.empty())
				continue;
			break;
		}
		// Verifico se esse alimento existe
		buscarAlimento(nome_ingrediente, 
		while (true) {
			std::cout << "Unidade de medida: ";
			medida = ler_nome();
			if (medida.empty())
				continue;
			break;
		}
		while (true) {
			std::cout << "Quantidade: ";
			quantidade = ler_inteiro();
			if (!quantidade)
				continue;
			break;
		}
		i++;
		
	}
	
	
}

// void editarReceita() {}