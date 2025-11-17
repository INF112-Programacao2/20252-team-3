#include <iostream>
#include <string>
#include <exception>

#include "usuario.hpp"
#include "funcoes_leitura.hpp"

int main() {

// 0) DECLARACOES
	int resposta_int;
	bool iniciar_programa = false;
	Usuario usuario;  // Objeto usuario (inicialmente vazio)	
	
// 1) Menu inicial	
	std::cout << "\n===DIARIO ALIMENTAR===\n";
	while (true) {
		std::cout << "\nMenu inicial................\n";
		std::cout << "Iniciar o programa (digite 1)\n";
		std::cout << "Fechar o programa (digite 2)\n";
		std::cout << "Desenvolvedores (digite 3)\n";
		std::cout << "Referencias (digite 4)\n";
		resposta_int = ler_num_no_intervalo(1, 4);
		if (!resposta_int) // se le_inteiro retornar 0
			continue; // reinicio a iteracao
		switch (resposta_int) {
			case 1:
				std::cout << "Iniciando o programa...\n";
				iniciar_programa = true;
				break;
			case 2:
				std::cout << "Fechando o programa...\n";
				// IMPLEMENTAR.............
				break;
			case 3:
				std::cout << "Desenvolvedores do programa...\n";
				// IMPLEMENTAR.............
				break;
			case 4:
				std::cout << "Referencias do programa...\n";
				// IMPLEMENTAR.............
				break;
		}
		if (iniciar_programa)
			break;
	}
	
// 2) Encontrando diretorio para o usuario
	usuario.lerCaminhoDiretorioDiario();
	
// 3) Cadastro do usuario 
	usuario.cadastrarUsuario();
	
// 4) Menu principal
	while (true) {
		std::cout << "\nMenu principal................\n";
		std::cout << "Adicionar alimentos (digite 1)\n";
		std::cout << "Ver alimentos adicionados (digite 2)\n";
		std::cout << "Editar alimentos adicionados (digite 3)\n";
		std::cout << "Ver status da Meta Calorica/Nutrientes (digite 4)\n";
		std::cout << "Gerar historico de consumo diario (digite 5)\n";
		std::cout << "Ver perfil pessoal (digite 6)\n";
		std::cout << "Editar perfil pessoal (digite 7)\n";
		std::cout << "Salvar e fechar programa (digite 8)\n";
		std::cout << "Desenvolvedores (digite 9)\n";
		std::cout << "Referencias (digite 10)\n";
		resposta_int = ler_num_no_intervalo(1, 10);
		if (!resposta_int) // se le_inteiro retornar 0
			continue; // reinicio a iteracao
	switch (resposta_int) {
			case 1:
				std::cout << "Adicionar alimentos (digite 1)\n";
				// usuario.lerAlimento();
				break;
			case 2:
				std::cout << "Ver alimentos adicionados (digite 2)\n";
				// usuario.verAlimentosAdicionados();
				break;
			case 3:
				std::cout << "Editar alimentos adicionados (digite 3)\n";
				// usuario.editarAlimentosAdicionados();
				break;
			case 4:
				std::cout << "Ver status da Meta Calorica/Nutrientes (digite 4)\n";
				// usuario.verNutrientesCalculados();
				break;
			case 5:
				std::cout << "Gerar historico de consumo diario (digite 5)\n";
				// usuario.gerarHistoricoDeConsumo();
				break;
			case 6:
				std::cout << "Ver perfil pessoal (digite 6)\n";
				// usuario.exibirPerfil();
				break;
			case 7:
				std::cout << "Editar perfil pessoal (digite 7)\n";
				// usuario.editarPerfil();
				break;
			case 8:
				std::cout << "Salvar e fechar programa (digite 8)\n";
				break;
			case 9:
				std::cout <<  "Desenvolvedores (digite 9)\n";
				break;
			case 10:
				std::cout << "Referencias (digite 10)\n";
				break;
			
		}
	
	}
	
	//CRIANDO O OBJETO USUARIO
	//Usuario usuario;
	return 0;
}