#include <iostream>
#include <filesystem>
#include <fstream>
#include <exception>
#include "usuario.hpp"
#include "funcoes_leitura.hpp"

namespace fs = std::filesystem; // namespace para filesystem

// Construtores e Destrutores
Usuario::Usuario() {}
Usuario::Usuario(std::string nome, int idade, int peso, double altura):
	_nome(nome), _idade(idade), _peso(peso), _altura(altura) {}
Usuario::~Usuario() {} 

// Getters
std::string Usuario::getNome() {return _nome;}
int Usuario::getIdade() {return _idade;}
double Usuario::getPeso() {return _peso;}
double Usuario::getAltura() {return _altura;}
//PerfilNutricional Usuario::getPerfilNutricional() {return _perfilNutricional;}
//DiarioAlimentar Usuario::getDiarioAlimentar() {return _diarioAlimentar;}
std::string Usuario::getCaminhoDiretorioDiario() {return _caminhoDiretorioDiario;}

// Setters
/*
void Usuario::setFatorFisico(std::string fator_fisico) {
	_perfilNutricional.setFatorFisico(fator_fisico);
}
void Usuario::setMetaCalorica(int meta_calorica) {
	_perfilNutricional.setMetaCalorica(meta_calorica);
}
*/
void Usuario::setCaminhoDiretorioDiario(std::string caminho_diretorio) {
	_caminhoDiretorioDiario = caminho_diretorio;
}
/*	
int Usuario::calcularMetaCalorica() {
	return _perfilNutricional.calcularMetaCalorica();
}
*/

void Usuario::lerCaminhoDiretorioDiario() {
	std::string caminho_diretorio;
	int resposta_int;
	
	std::cout << "\nEscolher diretorio para salvar dados.............\n";
	std::cout << 
	"Os dados dos alimentos e nutrientes cadastrados nesse\n" <<
	"programa serao salvos em arquivos txt em um diretorio \n" <<
	"de sua escolha e dentro desse diretorio sera gerado a \n" <<
	"pasta DIARIO_ALIMENTAR, onde ficara salvo os dados\n";
	
	while (true) {
		std::cout << "\nDiretorio.........................\n";
		std::cout << "Escolher novo diretorio (digite 1)\n";
		std::cout << "Ja tenho um diretorio salvo para o programa (digite 2)\n";
		resposta_int = ler_num_no_intervalo(1, 2);
		if(!resposta_int)
			continue;
		
		switch (resposta_int) {
			case 1:
				std::cout << "\nCriando diretorio...............\n";
				while (true) {
					std::cout << "\nInforme o caminho do diretorio: ";
					caminho_diretorio = ler_caminho_dir();
					if (caminho_diretorio.empty()) // se a str volta vazia = leitura invalida
						continue;
					if (fs::path(caminho_diretorio).filename().string() == "DIARIO_ALIMENTAR") {
						std::cout << "Diretorio selecionado = o proprio diretorio DIARIO_ALIMENTAR\n";
						std::cout << "Selecione a opcao: (Ja tenho um diretorio salvo para o programa) e passe esse caminho\n";				
						break;
					}
					std::cout << "Diretorio " << caminho_diretorio << " encontrado\n";
					// GERACAO DE UMA PASTA DIARIO_ALIMENTAR NESSE DIRETORIO
					std::string novo_caminho = caminho_diretorio + "\\DIARIO_ALIMENTAR"; // coloco 2 \ ("\\") para mostrar que nao \ de escape
					try {
						bool diretorio_foi_criado = fs::create_directory(fs::path(novo_caminho));
						if (diretorio_foi_criado)
							std::cout << "Diretorio DIARIO_ALIMENTAR criado em " << novo_caminho << std::endl;
						else { // se nao foi criado, testar para ver se caminho ja existia
							if (fs::exists(novo_caminho))
								throw std::invalid_argument("Diretorio DIARIO ALIMENTAR ja existe no caminho informado\n"
							"Selecione a opcao: (Ja tenho um diretorio salvo para o programa) e passe o caminho do DIARIO\n");
						}
					} catch (fs::filesystem_error &e) { // erro na geracao do diretorio
						std::cerr << "Erro: " << e.what() << std::endl;
						continue;
					} catch (std::invalid_argument &e) { // erro diretorio ja existe (mudar a opcao selcionada)
						std::cerr << "Erro: " << e.what() << std::endl;	
						break;
					} catch (std::exception &e) { // outro erro
						std::cerr << "Erro inesperado: " << e.what() << std::endl;	
						continue;
					}
					break; // paro o while do case 1
				}
				break; // paro o case 1:
					
				
			case 2:
				std::cout << "\nAbrindo diretorio DIARIO_ALIMENTAR\n";
				while (true) {
					std::cout << "\nInforme o caminho para a pasta DIARIO_ALIMENTAR: ";
					caminho_diretorio = ler_caminho_dir("DIARIO_ALIMENTAR");
					if (caminho_diretorio.empty())
						continue;
					break;
				}
				std::cout << "Diretorio " << caminho_diretorio << " encontrado\n";
				// INFORMAR OS USUARIOS JA CADASTRADOS (DENTRO DA PASTA DE DIARIO ALIMENTAR)
				bool ha_usuario_diretorio = true;
				std::cout << "Usuarios ja cadastrados:\n";
				try {
					if (fs::is_empty(caminho_diretorio)) // funcao me disse se o diretorio tem conteudo dentro 
						throw std::invalid_argument("Diretorio vazio\nNao ha nenhum usuario cadastrado ainda :(");
				} catch (std::invalid_argument &e) {
					std::cerr << e.what() << std::endl;
					ha_usuario_diretorio = false;
				}
					fs::directory_iterator primeiro_arq_diretorio = fs::directory_iterator(caminho_diretorio);
					fs::directory_iterator fim_diretorio; // fica vazio
					
				if (ha_usuario_diretorio) {
					bool aviso = false;
					for (fs::directory_iterator i = primeiro_arq_diretorio; i != fim_diretorio; i++) {
						// i é um ponteiro para o caminho do arquivo no d iretorio
						// que aponta para um objeto fs::directory_entry
						const fs::directory_entry conteudo = *i;
						std::string nome_arq = conteudo.path().filename().string();
						std::string tipo_arq;
						if (conteudo.is_directory()) {
							tipo_arq = "[DIR]";
							// Se é um diretorio, preciso verificar se USUARIO_ é uma subtring do nome realmente
							if (nome_arq.find("USUARIO_") == std::string::npos) // se o find retorna npos, a substring nao foi encontrada
							aviso = true;
						}
						else if (conteudo.is_regular_file()) {
							tipo_arq = "[ARQ]";
							aviso = true;
						}
						else {
							tipo_arq = "[OUTRO]";
							aviso = true;
						}
	
						std::cout << tipo_arq << " " << conteudo.path().filename().string() << std::endl;
					}
					if (aviso) {
						std::cout << "AVISO: Certifique que a pasta DIARIO_ALIMENTAR tenha APENAS as pastas dos Usuarios\n";
						std::cout << "Retire quaisquer outros arquivos ou diretorios que nao estejam relacionados";
					}
					// ADICIONAR OPCAO PARA O USUARIO PODER REAPROVEITAR OS DADOS DE ALGUMA PASTA USUARIO_...
				}
				break; // break para case 2 do switch
		}
		std::cout << "Caminho do diretorio para Diario Aliemntar do usuario salvo\n";
		// usuario.setCaminhoDiretorioDiario(caminho_diretorio);
		break; // break para loop while
	}
	// Atribuindo caminho para membro da classe
	_caminhoDiretorioDiario = caminho_diretorio;
}
void Usuario::cadastrarUsuario() {
	int resposta_int;
	char resposta_char; 
	
	
	while (true) {
		std::cout << "\nCadastro do usuario..........\n";
		std::cout << "Criar novo usuario (digite 1)\n";
		std::cout << "Abrir conta de usuario (digite 2)\n";
		resposta_int = ler_num_no_intervalo(1, 2);
		if (!resposta_int)
			continue;
		
		switch (resposta_int) {
			case 1:
				// CADASTRO DE NOVO USUARIO
				while (true) {
					std::cout << "\nNovo Usuario.............\n";
					// if (voltar())..............................
					//	break;
					lerDadosPessoaisUsuario();
					lerDadosNutricionaisUsuario();
					verDadosUsuario();
					// CONFIRMAR OS DADOS 
					while (true) {
						std::cout << "Confirmar cadastro das informacoes pessoais [S/N]: ";
						resposta_char = ler_S_ou_N(); // usuario confirma
						if (!resposta_char)
							continue;
						break;
					}
					if (resposta_char == 'S') {
						std::cout << "Dados pessoais confirmados!\n";
						break; // break para o loop while do cadastro do novo usuario	
					}
					else { // resposta_char == 'N'
						while (true) {
							std::cout << "Tem certeza? O programa voltara para o inicio do cadastro do novo usuario [S/N]: \n";
							resposta_char = ler_S_ou_N();
							if (!resposta_char)
								continue;
							break;
						}
					}
					if (resposta_char == 'S') // voltar ao Cadastro do Usuario
						continue;
					else { // resposta_char_2 == 'N'
						std::cout << "Como nao tem certeza, dados pessoais confirmados!\n";
						break; // break para o loop while do cadastro do novo usuario	
					}
				}
				std::cout << _nome << "seu cadastro foi realizado com sucesso!\n";
				std::cout << "Seus dados serao salvos na pasta DIARIO_ALIMENTAR\\Usuario_" << _nome << " em " << _caminhoDiretorioDiario << std::endl;
				
				// SALVA OS DADOS INFORMADOS PELO USUARIO EM ARQUIVOS
				if (salvarDadosUsuario()) // metodo faz a criacao e salvamento dos dados do usuario
					std::cout << "Dados do Usuario " << _nome << " salvos em " << _caminhoDiretorioDiario << std::endl;
				else {
					std::cout << "Nao foi possivel salvar os dados do Usuario " << _nome << " em " << _caminhoDiretorioDiario << std::endl;
					std::cout << "Voltando para menu de Cadastro do Usuario\n";
					continue;
				}
				break; // break para o case 1 do switch
			
			case 2:
				// BUSCA PELOS DADOS DE USUARIO JA CADASTRADO
				if (buscarDadosEmDiretorio()) // se a pasta esse usuario for encontrada
					std::cout << "Usuario " << _nome << " encontrado em " << _caminhoDiretorioDiario << std::endl;
				else {
					std::cout << "Usuario " << _nome << " nao encontrado em " << _caminhoDiretorioDiario << std::endl;
					std::cout << "Voltando para menu de Cadastro do Usuario\n";
					continue;
				}
				break;
		}
	}
	
}
void Usuario::lerDadosPessoaisUsuario() {
	std::string resposta_str;
	
	std::cout << "\nPreencha suas informacoes pessoais\n";
	while (true) {
		std::cout << "Nome (sem caracteres especiais): ";
		try {
			std::getline(std::cin, resposta_str);
			for (unsigned char caracter : resposta_str) {
				if (!(std::isalpha(caracter)) && caracter != ' ') // se algum dos caracteres nao for letra e nao for espaco (eu conto como letra))
					throw std::invalid_argument("Nome precisa ter apenas letras");
			}
		} catch (std::invalid_argument &e) {
			std::cerr << "Erro: " << e.what() << std::endl;
			continue;
		}
		try {
			_nome = resposta_str;
		} catch (std::exception &e) {
			std::cerr << "Erro inesperado: " << e.what() << std::endl;
			continue;
		}
		break;
	}
	while (true) {
		std::cout << "Idade (numero inteiro em anos): ";
		try {
			std::getline(std::cin, resposta_str);
			for (unsigned char digito : resposta_str) {
				if (!std::isdigit(digito))
					throw std::invalid_argument("Idade precisa ser inteiro");
			}
		} catch (std::invalid_argument &e) {
			std::cerr << "Erro: " << e.what() << std::endl;
			continue;
		}
		try {
			_idade = std::stoi(resposta_str);
		} catch (std::exception &e) {
			std::cerr << "Erro inesperado: " << e.what() << std::endl;
			continue;
		}
		break;
	}
	while (true) {
		std::cout << "Peso (numero inteiro em kg): ";
		try {
			std::getline(std::cin, resposta_str);
			for (unsigned char digito : resposta_str) {
				if (!std::isdigit(digito))
					throw std::invalid_argument("Peso precisa ser inteiro");
			}
		} catch (std::invalid_argument &e) {
			std::cerr << "Erro: " << e.what() << std::endl;
			continue;
		}
		try {
			_peso = std::stoi(resposta_str);
		} catch (std::exception &e) {
			std::cerr << "Erro inesperado: " << e.what() << std::endl;
			continue;
		}
		break;
	}
	while (true) {
		std::cout << "Altura (numero decimal em metros - use ponto como separador): ";
		try {
			std::getline(std::cin, resposta_str);
			int cont_ponto = 0;
			for (unsigned char digito : resposta_str) {
				if (!std::isdigit(digito) && digito != '.') // se o caracter nao for um digito e nao for ponto
					throw std::invalid_argument("Altura precisa ser decimal - com ponto");
				if (digito == '.')
					cont_ponto++;
			}
			if (cont_ponto <= 0 || cont_ponto > 1)
				throw std::invalid_argument("Altura precisa ser decimal - com ponto");
		} catch (std::invalid_argument &e) {
			std::cerr << "Erro: " << e.what() << std::endl;
			continue;
		}
		try {
			_altura = std::stod(resposta_str);
		} catch (std::exception &e) {
			std::cerr << "Erro inesperado: " << e.what() << std::endl;
			continue;
		}
		break;
	}
}
void Usuario::lerDadosNutricionaisUsuario() {
	int resposta_int;
	char resposta_char;
	std::string resposta_str;
	
	std::cout << "\nPreencher informacoes nutricionais/saude\n";

// Fator fisico
	while (true) {
		std::cout << "\nFator de atividade fisica (selecione uma opcao abaixo):\n";
		std::cout << "Sedentario (digite 1)\n";
		std::cout << "Ativo (digite 2)\n";
		std::cout << "Muito Ativo (digite 3)\n";
		std::cout << "Descrever melhor cada um (digite 4)\n";
		// digite 4 - Vai mostrar quanto de exercicio uma pessoa 
		// tem que fazer para ser classificado em cada categoria
		resposta_int = ler_num_no_intervalo(1, 4);
		if (!resposta_int)
			continue;
		
		switch (resposta_int) {
			case 1:
				//_perfilNutricional.setfator_fisico("Sedentario");
				std::cout << "Opcao informada: SEDENTARIO\n";
				break;
			case 2:
				//_perfilNutricional.setfator_fisico("Ativo")
				std::cout << "Opcao informada: ATIVO\n";
				break;
			case 3:
				//_perfilNutricional.setfator_fisico("Muito Ativo")
				std::cout << "Opcao informada: MUITO ATIVO\n";
				break;
			case 4:
				// ADICIONAR DESCRICAO DO NUMERO DE ATIVIDADES PARA A PESSOA SE ENQUADRAR EM CADA CASO
				break;
			
		}
		break;
	}
	// SALVO O FATOR FISICO NO USUARIO
	// usuario.setFatorFisico(fator_fisico);
							
// Meta calorica
	// 1) Fazer o calculo da meta calorica recomendada para o individuo, usando as informacoes do usuario
	// calcularMetaCalorica();
	// 2) Decidir o valor da meta calorica (recomendado ou personalizado)
	while (true) {
		std::cout << "\nMeta calorica (quantidade de calorias consumida diariamente)\n";
		// ALTERAR _perfilNutricional.getMetaCalorica
		std::cout << "Recomendado: " << "meta_calorica" << " kcal\n"; // recomendação baseada no calculo citado acima
		std::cout << "Seguir com valor recomendado [S/N]: ";
		resposta_char = ler_S_ou_N();
		if (!resposta_char)
			continue;
		break;
	}
	if (resposta_char == 'S') {
		// ALTERAR _perfilNutricional.getMetaCalorica
		std::cout << "Meta calorica (" << "meta_calorica" << " kcal) salva!\n";
		// setMetaCalorica(meta_calorica);
	}
	else { // resposta_char == 'N'
		std::string resposta_str;
		while (true) {
			std::cout << "\nPersonalizar a meta calorica.............\n";
			std::cout << "Digite a sua meta calorica (numero inteiro): ";
			try {
				std::getline(std::cin, resposta_str);
			for (unsigned char digito : resposta_str) {
				if (!std::isdigit(digito))
					throw std::invalid_argument("Meta calorica precisa ser inteiro");
				}
			} catch (std::invalid_argument &e) {
				std::cerr << "Erro: " << e.what() << std::endl;
				continue;
			}
			try {
				// ALTERAR _perfilNutricional.setMetaCalorica
				int meta_calorica = std::stoi(resposta_str);
			} catch (std::exception &e) {
				std::cerr << "Erro inesperado: " << e.what() << std::endl;
				continue;
			}
			break;
		}
		// ALTERAR _perfilNutricional.getMetaCalorica
		std::cout << "Meta calorica personalizada (" << "meta_calorica" << " kcal) salva!\n";
		// setMetaCalorica(meta_calorica);
	}
	
}
void Usuario::verDadosUsuario() {
	std::cout << "\nDados do Usuario..............\n";
	std::cout << "Nome: " << _nome << std::endl;
	std::cout << "Idade: " << _idade << " anos\n";
	std::cout << "Peso: " << _peso << " kg\n";
	std::cout << "Altura: " << _altura << " metros\n";
	//std::cout << "Fator Fisico: " << _perfilNutricional.getFatorFisico() << std::endl;
	//std::cout << "Meta Calorica: " << _perfilNutricional.getMetaCalorica() << std::endl;
	
}

bool Usuario::salvarDadosUsuario() {
	// PEGAR OS DADOS QUE O USUARIO INFORMOU E COLOCAR NOS ARQUIVOS:
	// "DadosPessoais_nomeUsuario" e DadosNutricionais_nomeUsuario"
	// AMBOS OS ARQUIVOS SERAO SALVOS EM _caminhoDiretorioDiario
	std::string caminho_arquivo_pessoal = "DadosPessoais_"+_nome;
	std::string caminho_arquivo_nutricional = "DadosNutricionais_"+_nome;
	std::ofstream cin_arquivo; 
	
	// Escrevendo o arquivo com os dados pessoais
	cin_arquivo.open(caminho_arquivo_pessoal); // gera o arquivo, caso ainda nao exista
	try {
		if (cin_arquivo.is_open()) {
			cin_arquivo << "Dados Pessoais do " << _nome << std::endl;
			cin_arquivo << std::endl;
			cin_arquivo << "Nome: " << _nome << std::endl;
			cin_arquivo << "Idade: " << _idade << std::endl;
			cin_arquivo << "Peso: " << _peso << std::endl;
			cin_arquivo << "Altura: " << _altura << std::endl;
			cin_arquivo.close();
		}
		else
			throw std::logic_error("Nao foi possivel abrir o arquivo com os Dados do Usuario");
	} catch (std::logic_error &e) {
		std::cerr << "Erro: " << e.what() << std::endl;
		return false;
	}
	
	// Escrevendo o arquivo com os dados nutricionais
	cin_arquivo.open(caminho_arquivo_pessoal); // gera o arquivo, caso ainda nao exista
	try {
		if (cin_arquivo.is_open()) {
			cin_arquivo << "Dados Pessoais do " << _nome << std::endl;
			cin_arquivo << std::endl;
			//cin_arquivo << "Fator Fisico: " << _perfilNutricional.getFatorFisico() << std::endl;
			//cin_arquivo << "Meta Calorica: " << _perfilNutricional.getMetaCalorica() << std::endl;
			cin_arquivo.close();
		}
		else
			throw std::logic_error("Nao foi possivel abrir o arquivo com os Dados do Usuario");
	} catch (std::logic_error &e) {
		std::cerr << "Erro: " << e.what() << std::endl;
		return false;
	}
			
	return true;
}

bool Usuario::buscarDadosEmDiretorio() {
	// Buscar em _caminhoDiretorioDiario pela pasta USUARIO_nomeUsuario
	fs::directory_iterator primeiro_arq_diretorio = fs::directory_iterator(_caminhoDiretorioDiario); // ponteiro para lista dos arquivos
	fs::directory_iterator fim_diretorio; // fica vazio
	fs::directory_entry arquivo;
	std::string nome_arq;
	std::string nome_esperado;
	bool encontrou_pasta = false;
	// E SE A PASTA ESTIVER VAZIA???
	for (fs::directory_iterator i = primeiro_arq_diretorio; i != fim_diretorio; i++) {
		arquivo = *i;
		nome_arq = arquivo.path().filename().string();
		nome_esperado = "USUARIO_"+_nome;
		if (nome_arq == nome_esperado) {
			std::cout << "Pasta " << nome_arq << " encontrada em " << _caminhoDiretorioDiario << std::endl;
			encontrou_pasta = true;
			break;
		}
	}
	if (!encontrou_pasta) {
		std::cout << "Pasta " << nome_arq << " nao encontrada em " << _caminhoDiretorioDiario << std::endl;
		return false;
	}
	
	// Verificar se na pasta ha os arquivos "DadosPessoais_nomeUsuario" e DadosNutricionais_nomeUsuario"
	std::string nome_esperado_p = "DadosPessoais_"+_nome;
	std::string nome_esperado_n = "DadosNutricionais_"+_nome;
	bool encontrou_arq_p = false;
	bool encontrou_arq_n = false;
	primeiro_arq_diretorio = fs::directory_iterator(arquivo);
	
	for (fs::directory_iterator j = primeiro_arq_diretorio; j != fim_diretorio; j++) {
		arquivo = *j;
		nome_arq = arquivo.path().filename().string();
		if (nome_arq == nome_esperado_p){ // se nao for igual a nenhum dos arquivos
			std::cout << "Arquivo " << nome_arq << " encontrado em" << _caminhoDiretorioDiario << std::endl;
			encontrou_arq_p = true;
		}
		if (nome_arq == nome_esperado_n) { // se nao for igual a nenhum dos arquivos
			std::cout << "Arquivo " << nome_arq << " encontrado em" << _caminhoDiretorioDiario << std::endl;
			encontrou_arq_n = true;
		} 
	}
	if (!encontrou_arq_p)
		std::cout << "Arquivo " << nome_esperado_p << " nao encontrado em" << _caminhoDiretorioDiario << std::endl;
	if (!encontrou_arq_n)
		std::cout << "Arquivo " << nome_esperado_n << " nao encontrado em" << _caminhoDiretorioDiario << std::endl;
	if (!encontrou_arq_p || !encontrou_arq_n)
		return false;
	return true;
}

void Usuario::lerAlimento() {
	std::string nome_alimento, refeicao, unidade_medida;
	bool alimento_encontrado;
	double quantidade;
	int resposta_int;
	
	while (true) {
		std::cout << "\nAdicionar alimentos ao Diario Alimentar.........\n";
		std::cout << "Pesquisar o nome do alimento (digite 1)\n";
		std::cout << "Voltar ao menu principal (digite 2)\n";
		std::cout << "Digite sua opcao (" << 1 << " - " << 2 << "): ";
		resposta_int = ler_num_no_intervalo(1, 2);
		if (!resposta_int)
			continue;
		break;
	}
	switch (resposta_int) {
		case 1:
			while (true) {
				std::cout << "\nCadastrando alimento.....\n"
				std::cout << "Nome do Alimento: ";
				nome_alimento = ler_nome();
				if (nome_alimento.empty())
					continue;
			
				std::cout << "Buscando no Banco de Dados " << nome_alimento << "...\n";
				try {
					nome_alimento = buscarAlimento(nome_alimento); // retorna o nome do alimento do DB selcionado pelo Usuario ou retorna "" caso ele nao decida
					if(nome_alimento.empty())
						throw std::invalid_argument("Alimento nao encontrado no Banco de Dados")
					else {
						std::cout << "Alimento encontrado no Banco de Dados";
						alimento_encontrado = true;
					}
				} catch (std::invalid_argument &e) {
					std::cerr << "Erro: " << e.what() << std::endl; 
					alimento_encontrado = false;
				}
				
				if (!alimento_encontrado) {
					while (true) {
						std::cout << "Digitar nome do alimento novamente (digite 1)\n"; 
						std::cout << "Adicionar novo alimento ao banco de dados (digite 2)\n";
						std::cout << "Voltar ao Menu Principal (digite 3)\n";
						std::cout << "Digite sua opcao (" << 1 << " - " << 3 << "): ";
						resposta_int = ler_num_no_intervalo(1, 3);
						if (!resposta_int)
							continue;
						break;
					}
					switch (resposta_int) {
						case 1:
							continue; // volta ao while da leitura do nome do alimento
							break;
						case 2:
							try {
								nome_alimento = cadastrarAlimentoDB(nome_alimento); // ATUALIZO O NOME DO ALIMENTO DE ACORDO COM O DB
								if (nome_alimento.empty()) // se nome de alimento nao voltar vazio = usuario escolheu uma opcao do DB que o agradou
									throw std::logic_error("Nao foi possivel realizar o cadastro do alimento");
								else 
									std:cout << "Cadastro do novo alimento realizado com sucesso\n";
							} catch (std::logic_error &e) {
								std::cerr << "Erro: " << e.what() < std::endl;
								continue; // volta para o loop de "alimento nao encontrado"
							}
							break; 
						case 3:
							return; // para a funcao lerAlimento
							break;
					}
					
				}
				// Prosseguir com o cadastro do alimento
				
				// Ler a Unidade de Medida
				int num_medidas;
				std::string medidas_alimento[50];
				std::string medida_escolhida;
				// Sistema sabe que banana é um alimento sólido e contavel 
				// (informação esta no banco de dados), logo, a unidade de 
				// medida deve concordar com essa medida
				medidas_alimento = buscarMedidas(nome_alimento); // funcao retorna um ponteiro para um array com as medidas
				num_medidas = buscarNumMedidas(nome_alimento);
				std::cout << "\nQuantidade do alimento.........\n";
				while (true) {
					std::cout << "Selecione a unidade de medida: \n";
					for (int i=0; i<num_medidas; i++) {
						std::cout << "Em " << medidas_alimento[i] << " (digite " << i+1 << ")\n";
					}
					std::cout << "Em outra unidade de medida (digite " << num_medidas+1 << ")\n";
					resposta_int = ler_num_no_intervalo(1, num_medidas+1);
					if (!resposta_int)
						continue;
					/*
					if (resposta_int == num_medidas+1) {
						while (true) {
							medidas_alimento = buscarMedidas(nome_alimento, "all"); // funcao retorna todas as medidas, quando argumento "all" passado
							num_medidas = buscarNumMedidas(nome_alimento, "all");
							std::cout << "Selecione a unidade de medida: \n";
							for (int i=0; i<num_medidas; i++) {
								std::cout << "Em " << medidas_alimento[i] << " (digite " << i+1 << ")\n";
							}
							resposta_int = ler_num_no_intervalo(1, num_medidas);
							if (!resposta_int)
								continue;
						}
					}
					*/
					
					// Atualizo a medida escolhida
					medida_escolhida = medida_alimento[resposta_int-1];
					
					break; // paro o while da leitura de unidade de medida
				}
				while (true) {
					std::cout << "\nQuantidade de alimento (em " << medida_escolhida << "): ";
					quantidade = ler_numero();
					if (!quantidade)
						continue;
					break;
				}
				
				// Ler a refeicao a qual o alimento pertence
				std::string refeicoes[4] = {"Cafe da Manha", "Almoco", "Jantar", "Lanche"};
				std::string refeicao_escolhida;
				while (true) {
					std::cout << "\nRefeicao em que o alimento foi consumido\n";
					std::cout << "Cafe da Manha (digite 1)\n";
					std::cout << "Almoco (digite 2)\n";
					std::cout << "Jantar (digite 3)\n";
					std::cout << "Lanche (digite 4)\n";
					std::cout << "Digite sua opcao (" << 1 << " - " << 4 << "): ";
					resposta_int = ler_num_intervalo();
					if (!resposta_int)
						continue;
					refeicao_escolhida = refeicao[resposta_int-1];
					// IDEIA - USAR OBJETO ALIMENTO E IR COLOCANDO AS VARIAVEIS LOCAIS COMO ATRIBUTOS DELE
					break;
				}
				char resposta_char;
				bool confirmou_cadastro;
				std::cout << "\nConfirmar informacoes do alimento......\n";
				std::cout << "Nome do alimento: " << nome_alimento;
				std::cout << "Quantidade: " << quantidade << " " << medida_escolhida << std::endl;
				std::cout << "Refeicao: " << refeicao_escolhida << std::endl;
				while (true) {
					std::cout << "Adicionar alimento no Diario Alimentar [S/N]: ";
					resposta_char = ler_S_ou_N();
					if (!resposta_char)
						continue;
					else {
						if (resposta_char == 'S')
							confirmou_cadastro = true;
						else // resposta_char == 'N'
							confirmou_cadastro = false;
					}
					break;
				}
				if (confirmou_cadastro) {
					std::cout << "Alimento adicionado ao seu Diario!\n";
					_diario.adicionarAlimento(alimento); // COMPLETAR.....................
					// Para cada alimento adicionado, um contador de num de alimento no diario aumenta
					// E o alimento é colocado em uma outra posicao do vetor de Alimento do Diario
					// Logo, dentro dessa funcao a minha unica preocuopacao deve ser ter um objeto Alimento,
					// que é recriado a cada loop, ou seja, a declaracao é feita dentro do loop while de cadastro
				}
				else
					std::cout << "Alimento nao adicionado ao Diario\n";
				
				while (true) {
					std::cout << "Adicionar outro alimento no Diario (digite 1)\n";
					std::cout << "Voltar ao menu principal (digite 2)\n";
					std::cout << "Digite sua opcao (" << 1 << " - " << 2 << "): ";
					resposta_int = ler_num_no_intervalo(1, 2);
					if (!resposta_int)
						continue;
					break;
				}
				switch (resposta_int) {
					case 1:
						continue; // volta ao loop while de cadastro de alimento
						// Preciso criar 
					case 2:
						std::cout << "Fim do cadastro de alimento\n";
						return;
						break;
				}
			}
			
		case 2:
			std::cout << "Fim do cadastro de alimento\n";
			return;
			// voltar();
			break;
	}
}

void Usuario::gerarPastaUsuario() {}
void Usuario::visualizarInfomacoesPessoais() {}
void Usuario::lerFatorFisico() {}
void Usuario::lerMetaCalorias() {}
void Usuario::visualizarPerfilNutricional() {}
void Usuario::salvarPerfilNutricional() {}
void Usuario::adicionarAlimentoDiario() {}
void Usuario::visualizarDiario() {}
void Usuario::adicionarNovoAlimento() {}
void Usuario::gerarArquivoHistorico() {} 