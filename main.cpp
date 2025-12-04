#include <iostream>
#include <string>
#include <stdexcept>

#include "usuario.hpp"
#include "funcoes_leitura.hpp"

int main() {

// 0) DECLARACOES
	int resposta_int; // variavel para leitura do numero digitado na tela
	Usuario usuario;  // Objeto usuario (inicializado com um construtor vazio)	
	
// 1) Menu inicial	
	std::cout << "\n===DIARIO ALIMENTAR===\n";
	while (true) {
		bool iniciar_programa = false;
		std::cout << "\nMenu inicial................\n";
		std::cout << "1. Iniciar o programa\n";
		std::cout << "2. Fechar o programa\n";
		std::cout << "Digite sua opcao (1-2): ";
		resposta_int = ler_num_no_intervalo(1, 2);
		if (!resposta_int) // se le_inteiro retornar 0
			continue; // reinicio a iteracao
		switch (resposta_int) {
			case 1:
				std::cout << "Iniciando o programa...\n";
				// Decido qual o diretorio em que as informacoes do usuario serao salvas
				if (!(usuario.lerCaminhoDiretorioDiario())) // se o usuario parar a excecucao da funcao, volta para menu
					continue; 
				// Faço o cadastro do Usuario (preencher informacoes pessoais e nutricionais
				if (!(usuario.cadastrarUsuario())) // se o usuario parar a excecucao da funcao, volta para menu
					continue; 
				iniciar_programa = true;
				break;
			case 2:
				std::cout << "Fechando o programa...\n";
				exit(0);
				break;
			
		}
		if (iniciar_programa)
			break;
	}
	
	
// 2) Menu principal
	while (true) {
		std::cout << "\nMenu principal................\n";
		std::cout << "1. Abrir Diario Alimentar\n";
		std::cout << "2. Editar perfil pessoal\n";
		std::cout << "3. Desenvolvedores\n";
		std::cout << "4. Salvar e fechar programa\n";
		std::cout << "Digite sua opcao (1-4): ";
		resposta_int = ler_num_no_intervalo(1, 4);
		if (!resposta_int) // se funcao retornar 0, reinicie o loop
			continue;
		switch (resposta_int) {
			case 1:
				while (true) {
					bool parar_loop_interno = false;
					std::cout << "\nMenu do Diario Alimentar\n";
					std::cout << "1. Adicionar alimentos\n";
					std::cout << "2. Ver alimentos adicionados\n";
					std::cout << "3. Editar alimentos adicionados\n";
					std::cout << "4. Ver status da Meta Calorica/Nutrientes\n";
					std::cout << "5. Gerar historico de consumo diario\n";
					std::cout << "6. Voltar para o menu principal\n";
					resposta_int = ler_num_no_intervalo(1, 6);
					if (!resposta_int)
						continue;
					switch (resposta_int) {
						case 1:
							usuario.adicionarAlimentoAoDiario();
							break;
						case 2:
							usuario.verAlimentosAdicionados();
							break;
						case 3:
							usuario.editarAlimentosAdicionados();
							break;
						case 4:
							usuario.verNutrientesCalculados();
							break;
						case 5:
							try {
								// Tenta gerar o relatório
								usuario.getPerfilNutricional().gerarRelatorioDeConsumo(usuario.getDiarioAlimentar().getAlimentos());
								std::cout << "Relatório de consumo gerado com sucesso!" << std::endl;
							
							} catch (const std::runtime_error& e) {
								std::cerr << "ERRO FATAL: Falha na geração do relatório. " << e.what() << std::endl;
							}
							break;
						case 6:
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
					std::cout << "1. Ver perfil pessoal\n";
					std::cout << "2. Editar perfil pessoal\n";
					std::cout << "3. Voltar para o menu principal\n";
					std::cout << "Digite sua opcao (1-3): ";
					resposta_int = ler_num_no_intervalo(1, 6);
					if (!resposta_int)
						continue;
					switch (resposta_int) {
						case 1:
							usuario.verDadosUsuario();
							break;
						case 2:
							usuario.editarDadosUsuario();
							break;
						case 3:
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
				std::cout << "Seus arquivos de historico do Diario Alimentar e Perfil pessoal foram salvos.\n";
				std::cout << "Local: " << usuario.getCaminhoDiretorioDiario() << std::endl;
				std::cout << "Fechando o programa...\n";
				exit(0);
				break;
		}
	}
	return 0;
}