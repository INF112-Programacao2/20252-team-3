#include <iostream>
#include <string>
#include <exception>

#include "usuario.hpp"
#include "funcoes_leitura.hpp"

int main() {

// 0) DECLARACOES
	int resposta_int; // variavel para leitura do numero digitado na tela
	Usuario usuario;  // Objeto usuario (inicialmente vazio)	
	
// 1) Menu inicial	
	std::cout << "\n===DIARIO ALIMENTAR===\n";
	while (true) {
		bool iniciar_programa = false;
		std::cout << "\nMenu inicial................\n";
		std::cout << "Iniciar o programa (digite 1)\n";
		std::cout << "Fechar o programa (digite 2)\n";
		resposta_int = ler_num_no_intervalo(1, 2);
		if (!resposta_int) // se le_inteiro retornar 0
			continue; // reinicio a iteracao
		switch (resposta_int) {
			case 1:
				std::cout << "Iniciando o programa...\n";
				// Decido qual o diretorio em que as informacoes do usuario serao salvas
				if (!(usuario.lerCaminhoDiretorioDiario())) // se usuario parar a excecucao, volta para menu
					continue; // em caso de erro, volta para o loop while
				// Faço o cadastro do Usuario (preencher informacoes pessoais e nutricionais
				if (!(usuario.cadastrarUsuario())) 
					continue; // em caso de erro, volta para o loop while
				iniciar_programa = true;
				break;
			case 2:
				std::cout << "Fechando o programa...\n";
				exit(0)
				break;
			
		}
		if (iniciar_programa)
			break;
	}
	
	
// 2) Menu principal
	while (true) {
		std::cout << "\nMenu principal................\n";
		std::cout << "Abrir Diario Alimentar (digite 1)\n";
		std::cout << "Editar perfil pessoal (digite 2)\n";
		std::cout << "Desenvolvedores (digite 3)\n";
		std::cout << "Salvar e fechar programa (digite 4)\n";
		resposta_int = ler_num_no_intervalo(1, 4);
		if (!resposta_int) // se funcao retornar 0, reinicie o loop
			continue;
		switch (resposta_int) {
			case 1:
				while (true) {
					bool parar_loop_interno = false;
					std::cout << "\nMenu do Diario Alimentar\n";
					std::cout << "Adicionar alimentos (digite 1)\n";
					std::cout << "Ver alimentos adicionados (digite 2)\n";
					std::cout << "Editar alimentos adicionados (digite 3)\n";
					std::cout << "Ver status da Meta Calorica/Nutrientes (digite 4)\n";
					std::cout << "Gerar historico de consumo diario (digite 5)\n";
					std::cout << "Voltar para o menu principal (digite 6)\n";
					resposta_int = ler_num_no_intervalo(1, 6);
					if (!resposta_int)
						continue;
					switch (resposta_int) {
						case 1:
							std::cout << "Adicionar alimentos (digite 1)\n";
							// usuario.adicionarAlimentoAoDiario();
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
							std::cout << "Voltar para o menu principal (digite 6)\n";
							parar_loop_interno = true;
							break;
					}
					if (parar_loop_interno)
						break;
				}
				break;
			case 2:
				std::cout << "\nMenu do Perfil Pessoal\n";
				while (true) {
					bool parar_loop_interno = false;
					std::cout << "Ver perfil pessoal (digite 1)\n";
					std::cout << "Editar perfil pessoal (digite 2)\n";
					std::cout << "Voltar para o menu principal (digite 3)\n";
					resposta_int = ler_num_no_intervalo(1, 6);
					if (!resposta_int)
						continue;
					switch (resposta_int) {
						case 1:
							std::cout << "Ver perfil pessoal (digite 6)\n";
							// usuario.exibirPerfil();
							break;
						case 2:
							std::cout << "Editar perfil pessoal (digite 7)\n";
							// usuario.editarPerfil();
							break;
						case 3:
							std::cout << "Voltar para o menu principal (digite 6)\n";
							parar_loop_interno = true;
							break;
						
					}
					if (parar_loop_interno)
						break;
				}
				break;
			case 3:
				std::cout << "Desenvolvedores do programa...\n";
				std::cout << "Anthony de Paula Cândido\n";
				std::cout << "Gustavo Freitas Santos\n";
				std::cout << "João Pedro Castro da Rocha\n";
				std::cout << "Leandro Henrique Costa de Sousa\n";
				std::cout << "Pedro Cremonezi Fiorini \n";
				break;
			case 4:
				std::cout << "Salvar e fechar programa (digite 4)\n";
				std::cout << "Fechando o programa...\n";
				exit(0)
		}
	}
	return 0;
}