#include <iostream>
#include <filesystem>
#include <fstream>
#include <exception>
#include <vector>
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
PerfilNutricional Usuario::getPerfilNutricional() {return _perfilNutricional;}
DiarioAlimentar Usuario::getDiarioAlimentar() {return _diarioAlimentar;}
std::string Usuario::getCaminhoDiretorioDiario() {return _caminhoDiretorioDiario;}

// Setters

void setNome(std::string nome) {_nome = nome;}
void setIdade(int idade) {_idade = idade;}
void setPeso(int peso) {_peso = peso;}
void setAltura(double altura) {_altura = altura;}


bool Usuario::lerCaminhoDiretorioDiario() { // chamado pela funcao main na Etapa 2
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
		std::cout << "Voltar para o menu (digite 3)\n";
		std::cout << "Digite uma opcao de (1 - 3): ";
		resposta_int = ler_num_no_intervalo(1, 3);
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
				}
				break; // break para case 2 do switch
			case 3:
				std::cout << "Voltando para o menu do programa\n";
				return false;
				break;
		}
		std::cout << "Caminho do diretorio para Diario Aliemntar do usuario salvo\n";
		break; // break para loop while
	}
	
	// Atribuindo caminho para membro da classe
	_caminhoDiretorioDiario = caminho_diretorio;
	
	return true;
}
bool Usuario::cadastrarUsuario() {
	int resposta_int;
	char resposta_char;
	bool confirmou_cadastro = false;
	
	while (true) {
		std::cout << "\nCadastro do usuario..........\n";
		std::cout << "Criar novo usuario (digite 1)\n";
		std::cout << "Abrir conta de usuario (digite 2)\n";
		std::cout << "Voltar para o menu (digite 3)\n";
		std::cout << "Digite uma opcao de (1 - 3): ";
		resposta_int = ler_num_no_intervalo(1, 3);
		if (!resposta_int)
			continue;
		
		switch (resposta_int) {
			case 1:
				// CADASTRO DE NOVO USUARIO
				std::cout << "\nNovo Usuario.............\n";
				lerDadosPessoaisUsuario(); // le e guarda
				lerDadosNutricionaisUsuario(); // le e guarda
				verDadosUsuario();
				
				// CONFIRMAR OS DADOS 
				while (true) {
					std::cout << "Confirmar cadastro das informacoes pessoais [S/N]: ";
					resposta_char = ler_S_ou_N(); // usuario confirma
					if (!resposta_char)
						continue;
					if (resposta_char == 'S') {
						std::cout << "Dados pessoais confirmados!\n";	
						confirmou_cadastro = true;
						break;
					}
					else { // resposta_char == 'N'
						while (true) {
							std::cout << "Tem certeza? O programa voltara para o inicio do cadastro do novo usuario [S/N]: \n";
							resposta_char = ler_S_ou_N();
							if (!resposta_char)
								continue;
							break;
						}
						if (resposta_char == 'S') {
							std::cout << "Dados pessoais nao confirmados\n";
							break;
						}
						continue; // resposta_char == 'N'. Se a pessoa nao tem certeza, faz a pergunta novamnete
					}	
				}
				
				if (!confirmou_cadastro) // voltar ao Cadastro do Usuario
					continue;
					
				// SALVA OS DADOS INFORMADOS PELO USUARIO EM ARQUIVOS
				std::cout << _nome << "seu cadastro foi realizado com sucesso!\n";
				std::cout << "Seus dados serao salvos na pasta DIARIO_ALIMENTAR\\Usuario_" << _nome << " em " << _caminhoDiretorioDiario << std::endl;
				
				if (salvarDadosUsuario()) // metodo faz a criacao e salvamento dos dados do usuario
					std::cout << "Dados do Usuario " << _nome << " salvos em " << _caminhoDiretorioDiario << std::endl;
				else {
					std::cout << "Nao foi possivel salvar os dados do Usuario " << _nome << " em " << _caminhoDiretorioDiario << std::endl;
					std::cout << "Voltando para Cadastro do Usuario\n";
					continue;
				}
				break; // break para o case 1 do switch
			
			case 2:
				// BUSCA PELOS DADOS DE USUARIO JA CADASTRADO
				if (buscarDadosEmDiretorio()) // se a pasta esse usuario for encontrada
					std::cout << "Usuario " << _nome << " seus dados foram encontrados em " << _caminhoDiretorioDiario << std::endl;
				else {
					std::cout << "Usuario " << _nome << " seus dados foram encontrados em " << _caminhoDiretorioDiario << std::endl;
					std::cout << "Voltando para menu de Cadastro do Usuario\n";
					continue;
				}
				break;
			case 3:
				std::cout << "Voltando para o menu do programa\n";
				return false;
				break;
		}
		break; // break para o while de Cadastro do Usuario
	}
	return true;
	
}
void Usuario::lerDadosPessoaisUsuario() {
	std::string nome;
	int idade, peso;
	double altura;
	
	std::cout << "\nPreencha suas informacoes pessoais\n";
	while (true) {
		std::cout << "Nome (sem caracteres especiais): ";
		nome = ler_nome();
		if (nome.empty())
			continue;
	}
	while (true) {
		std::cout << "Idade (numero inteiro em anos): ";
		altura = ler_inteiro();
		if (altura)
			continue;
	}
	while (true) {
		std::cout << "Peso (numero inteiro em kg): ";
		peso = ler_inteiro();
		if (!peso)
			continue;
		break;
	}
	while (true) {
		std::cout << "Altura (numero decimal em metros - use ponto como separador): ";
		altura = ler_double();
		if (!altura)
			continue;
		break;
	}
	std::cout << "Informacoes pessoais completas!\n";
	
	
	// Chamo os metodos set da classe para adicionar os atributos
	setNome(nome);
	setIdade(idade);
	setPeso(peso);
	setAltura(altura);
	
}
void Usuario::lerDadosNutricionaisUsuario() {
	int resposta_int;
	char resposta_char;
	
	std::string fatores_fisicos[3] = {"Sedentario", "Ativo", "Muito Ativo"};
	std::string fator_fisico;
	
	double meta_calorica_recomendada;
	double meta_calorica;
	
	std::cout << "\nPreencher informacoes nutricionais/saude\n";

// Fator fisico
	while (true) {
		std::cout << "\nFator de atividade fisica (selecione uma opcao abaixo):\n";
		std::cout << "Sedentario (digite 1)\n";
		std::cout << "Ativo (digite 2)\n";
		std::cout << "Muito Ativo (digite 3)\n";
		std::cout << "Descrever melhor cada um (digite 4)\n";
		std::cout << "Digite uma opcao de (1 - 4): ";
		// digite 4 - Vai mostrar quanto de exercicio uma pessoa 
		// tem que fazer para ser classificado em cada categoria
		resposta_int = ler_num_no_intervalo(1, 4);
		if (!resposta_int)
			continue;
		
		switch (resposta_int) {
			case 1:
				std::cout << "Opcao informada: SEDENTARIO\n";
				break;
			case 2:
				std::cout << "Opcao informada: ATIVO\n";
				break;
			case 3:
				std::cout << "Opcao informada: MUITO ATIVO\n";
				break;
			case 4:
				// ADICIONAR DESCRICAO DO NUMERO DE ATIVIDADES PARA A PESSOA SE ENQUADRAR EM CADA CASO
				continue; // volta para leitura do tipo de fator fisico
				break;
			
		}
		break;
	}
	// SALVO O FATOR FISICO NO USUARIO
	fator_fisico = fatores_fisicos[resposta_int-1]; 
							
// Meta calorica
	// 1) Fazer o calculo da meta calorica recomendada para o individuo, usando as informacoes do usuario
	meta_calorica_recomendada = perfilNutricional.calcularMetaCalorica(_idade, _peso, _altura, fator_fisico);
	
	// 2) Decidir o valor da meta calorica (recomendado ou personalizado)
	while (true) {
		std::cout << "\nMeta calorica (quantidade de calorias consumida diariamente)\n";
		std::cout << "Recomendado: " << meta_calorica_recomendada << " kcal\n"; // recomendação baseada no calculo citado acima
		std::cout << "Seguir com valor recomendado [S/N]: ";
		resposta_char = ler_S_ou_N();
		if (!resposta_char)
			continue;
		break;
	}
	if (resposta_char == 'S') {
		std::cout << "Meta calorica (" << meta_calorica_recomendada << " kcal) salva!\n";
		meta_calorica = meta_calorica_recomendada;
	}
	else { // resposta_char == 'N'
		std::string resposta_str;
		while (true) {
			std::cout << "\nPersonalizar a meta calorica.............\n";
			std::cout << "Digite a sua meta calorica (numero decimal): ";
			meta_calorica = ler_double();
			if (!meta_calorica)
				continue;
			break;
		}
		std::cout << "Meta calorica personalizada (" << meta_calorica << " kcal) salva!\n";
		// meta_calorica = meta_calorica;
	}
	
	// Chamo os metodos set da classe perfil Nutricional e passo os atributos
	_perfilNutricional.setFatorFisico(fator_fisico);
	_perfilNutricional.setMetaCalorica(meta_calorica);
	
}
void Usuario::verDadosUsuario() {
	std::cout << "\nDados do Usuario..............\n";
	std::cout << "Nome: " << _nome << std::endl;
	std::cout << "Idade: " << _idade << " anos\n";
	std::cout << "Peso: " << _peso << " kg\n";
	std::cout << "Altura: " << _altura << " metros\n";
	std::cout << "Fator Fisico: " << _perfilNutricional.getFatorFisico() << std::endl;
	std::cout << "Meta Calorica: " << _perfilNutricional.getMetaCalorica() << " kcal" << std::endl;
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
			cin_arquivo << "Fator Fisico: " << _perfilNutricional.getFatorFisico() << std::endl;
			cin_arquivo << "Meta Calorica: " << _perfilNutricional.getMetaCalorica() << std::endl;
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
	// E SE A PASTA ESTIVER VAZIA? R: o loop nao sera executado
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

std::string Usuario::lerAlimento() {
	
	int* ID_alimento_match;
	bool alimento_encontrado;
	std::string nome_alimento;

	while (true) {
		std::cout << "\nPesquisando pelo alimento.....\n"
		std::cout << "Nome do Alimento: ";
		nome_alimento = ler_nome();
		if (nome_alimento.empty())
			continue;
		break
	}
	std::cout << "Buscando no Banco de Dados " << nome_alimento << "...\n";
	try {
		ID_alimento_match = _diario.buscarAlimento(nome_alimento); // retorna o endereco para um array dos IDs
		if(ID_alimento_match == nullptr)
			throw std::invalid_argument("Alimento nao encontrado no Banco de Dados");
		else {
			std::cout << "Alimento encontrado no Banco de Dados";
			alimento_encontrado = true;
		}
	} catch (std::invalid_argument &e) {
		std::cerr << "Erro: " << e.what() << std::endl; 
		alimento_encontrado = false;
	}
	
	if (!alimento_encontrado) {
		return "";
	}
	else {
		// Exibir quais foram os alimentos encontrados
		int num_ID_alimento_match = ID_alimento_match[0];
		std::cout << "\nLista dos alimentos encontrados\n";
		for (int i=1; i<num_ID_alimento_match; i++) {
			try {
				nome_alimento = _diario.buscarAlimentoPeloID(ID_alimento_match[i]);
			}
			catch (std::out_of_range &e) {
				std::cerr << "Acesso a posicao invalida do vetor\n";
				exit(0);
			}
			catch (std::exception &e) {
				std::cerr << "Erro inesperado " << e.what() << std::endl;
			}
			std::cout << i << ") " << nome_alimento;
			
		}
		
		// Decidir quais dos alimentos o usuario quer
		while (true) {
			std::cout << "Escolha uma opcao (1 - " << num_alimentos_encontrados-1 << ")\n";
			resposta_int = ler_num_no_intervalo(1, num_alimentos_encontrados-1);
			if (!resposta_int)
				continue;
		}
		nome_alimento = _diario.buscarAlimentoPeloID(ID_alimento_match[resposta_int]);
		std::cout << "Alimento escolhido " << nome_alimento << std::endl;
		
		return nome_alimento;
	}
}

void Usuario::adicionarAlimentoAoDiario() {

	// PASSAR UM OBJETO ALIMENTO PARA A CLASSE DIARIO
	Alimento alimento; // inicializo a varivavel assim que souber o nome correto
	int resposta_int;
	std::string nome_alimento;
	
	while (true) {
		std::cout << "\nAdicionar alimentos ao Diario Alimentar.........\n";
		std::cout << "Cadastrar alimento ao Diario (digite 1)\n";
		std::cout << "Voltar ao menu principal (digite 2)\n";
		std::cout << "Digite sua opcao (1 - 2): ";
		resposta_int = ler_num_no_intervalo(1, 2);
		if (!resposta_int)
			continue;
		break;
	}
	
	switch (resposta_int) {
		case 1:
			nome_alimento = ler_alimento();
			if (nome_alimento.empty()) {
				while (true) {
					std::cout << "\nAlimento nao encontrado................\n";
					std::cout << "Digitar nome do alimento novamente (digite 1)\n"; 
					std::cout << "Criar um novo alimento e adiciona-lo ao banco de dados (digite 2)\n";
					std::cout << "Voltar ao Menu Principal (digite 3)\n";
					std::cout << "Digite sua opcao (1 - 3): ";
					resposta_int = ler_num_no_intervalo(1, 3);
					if (!resposta_int)
						continue;
					break;
				}
				switch (resposta_int) {
					case 1:
						adicionarAlimentoAoDiario(); // chamo a funcao recursivamente
						return; // funcao termina aqui, pois estou chamando recursivamente
						break;
					case 2:
						nome_alimento = cadastrarNovoAlimentoDB(); 
						// atualizo o nome do alimento no Banco de Dados
						// e retorna o nome do alimento cadastrado
						// Continuo com a leitura depois do switch
						break; 
					case 3:
						std::cout << "Voltando ao menu principal\n";
						return; // fim da funcao lerAlimento
						break;
				}
			}
			// 1) Coloco o nome dentro do objeto alimento
			alimento.setNome(nome_alimento); 
			
			// Prosseguir com o cadastro do alimento
			
			// Ler a Quantidade e a Unidade de Medida
			int num_medidas;
			std::string medidas_alimento[50];
			std::string unidade_medida
			
			medidas_alimento = buscarMedidas(_diario.buscarSubClasse(nome_alimento)); // funcao recebe o tipo de subclasse do alimento (SC, SNC ou L) e retorna as medidas para essa subclasse
			num_medidas = buscarNumMedidas(_diario.buscarSubClasse(nome_alimento)); // igual a outra, mas me retorna quantas medidas
			
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
				
				
				// Atualizo a medida escolhida
				if (resposta_int == num_medidas+1) {
					while (true) {
						medidas_alimento = buscarMedidas(); // se eu nao passo parametro, todas as medidas sao retornadas
						num_medidas = buscarNumMedidas(); // mesma coisa aqui
						std::cout << "Todas as unidades de medida (selecione uma)\n";
						for (int i=0; i<num_medidas; i++) {
							std::cout << "Em " << medidas_alimento[i] << " (digite " << i+1 << ")\n";
						}
						resposta_int = ler_num_no_intervalo(1, num_medidas);
						if (!resposta_int)
							continue;
						break;
					}
					unidade_medida = medida_alimento[resposta_int-1];
				}
					
				else
					unidade_medida = medida_alimento[resposta_int-1];
				
				break; // paro o while da leitura de unidade de medida
			}
			
			// 2) Coloco a unidade de medida dentro do objeto alimento
			alimento.setUnidadeMedida(unidade_medida); 
			
			int quantidade;
			while (true) {
				std::cout << "Digite a quantidade de alimento (em " << unidade_medida << "): ";
				quantidade = ler_inteiro();
				if (!quantidade)
					continue;
				break;
			}
			
			// 3) Coloco a quantidade dentro do objeto alimento
			alimento.setQuantidade(quantidade);
			
			// Ler a refeicao a qual o alimento pertence
			std::string refeicao_escolhida;
			while (true) {
				std::cout << "\nRefeicao em que o alimento foi consumido\n";
				std::cout << "Cafe da Manha (digite 1)\n";
				std::cout << "Almoco (digite 2)\n";
				std::cout << "Jantar (digite 3)\n";
				std::cout << "Lanche (digite 4)\n";
				std::cout << "Digite sua opcao (1 - 4): ";
				resposta_int = ler_num_intervalo();
				if (!resposta_int)
					continue;
				refeicao_escolhida = static_cast<Refeicao>(resposta_int-1);
				break;
			}
			
			// 4) Coloco a refeicao dentro do objeto alimento
			alimento.setRefeicao(refeicao_escolhida);
			
			
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
			
			// 5) Passar o objeto alimento com os atributos setados para o diario
			if (confirmou_cadastro) {
				std::cout << "Alimento adicionado ao seu Diario!\n";
				_diario.adicionarAlimento(alimento); 
				// Para cada alimento adicionado, um contador de num de alimento no diario aumenta
				// E o alimento é colocado em uma outra posicao do vetor de Alimento do Diario
				// Logo, dentro dessa funcao a minha unica preocuopacao deve ser ter um objeto Alimento,
				// que é recriado a cada loop, ou seja, a declaracao é feita dentro do loop while de cadastro
			}
			else {
				std::cout << "Alimento nao adicionado ao Diario\n";
			}
			
			break; // break para o case 1
							
		case 2:
			std::cout << "Voltando ao menu principal\n";
			return;
			break;
	}
	
	// 6) Adicionar mais alimentos
	char resposta_char
	while (true) {
		std::cout << "Adicionar mais alimentos? [S/N]\n";
		resposta_char = ler_S_ou_N();
		if (!resposta_char)
			continue;
		break;
	}
	if (resposta_char == 'S')
		ler_alimento(); //chama a funcao recursivamente
	else {
		std::cout << "Fim do cadastro\n";
		std::cout << "Voltando ao menu principal\n";
		return;
	}
}

std::vector<std::string> buscarMedidas(std:string subclasse_alimento="") {
	std::vector<std::string> medidas_solido_contaveis = {
		"Unidades", "Meia Duzia", "Duzia"
	};
	std::vector<std::string> medidas_solido_nao_contaveis = {
		"Xicara", "Meia Xicara", "Quarto Xicara", "Copo Americano", "Colher de Sopa", "Colher de Sobremesa",
		"Colher de Chá"
	};
	std::vector<std::string> medidas_liquido = {
		"Litros", "Mililitros"
	};
	
	std::vector<std::string> todas_medidas;
	// funcao insert(posicao em que o insert comecao, comeco do vetor inserido, fim do vetor inserido)
	todas_medidas.insert(todas_medidas.end(), medidas_solido_contaveis().begin(), medidas_solido_contaveis().end());
	todas_medidas.insert(todas_medidas.end(), medidas_solido_nao_contaveis().begin(), medidas_solido_nao_contaveis().end());
	todas_medidas.insert(todas_medidas.end(), medidas_liquido().begin(), medidas_liquido().end());
	
	
	if (subclasse_alimento.empty()) { // quando nao especifica nenhuma subclasse, retorno todas as medidas
		return todas_medidas;
	}
	else {
		if (subclasse_alimento == "SolidoContavel")
			return medidas_solido_contaveis;
		else if (subclasse_alimento == "SolidoNaoContavel")
			return medidas_nao_solido_contaveis;
		else
			return medidas_liquido;
	}
	
}
int buscarNumMedidas(std:string subclasse_alimento="") {
	return buscarMedidas(subclasse_alimento).size();
}

std::string Usuario::cadastrarNovoAlimentoDB() {
	Alimento novo_alimento;
	int resposta_int;
	
	std::cout << "\nCadastro de Novo Alimento no Banco De Dados\n";
	
	std::string nome;
	std::cout << "Informacoes gerais do alimento.........\n";
	while (true) {
		std::cout << "Nome: ";
		nome = ler_nome();
		if (nome.empty())
			continue;
	}
	// 1) Coloco o nome no objeto alimento
	alimento.setNome(nome);
	
	CategoriaAlimento categoria; // crio enum CategoriaAlimento
	while (true) {
		std::cout << "Categoria (escolha uma opcao abaixo): ";
		std::cout << "Cereais e Derivados (digite 1)\n";
		std::cout << "Verduras, hortaliças e derivados (digite 2)\n";
		std::cout << "Frutas e derivados (digite 3)\n";
		std::cout << "Leguminosas e derivados (digite 4)\n";
		std::cout << "Nozes e sementes (digite 5)\n";
		std::cout << "Gorduras e óleos (digite 6)\n";
		std::cout << "Pescados e frutos do mar (digite 7)\n";
		std::cout << "Carnes e derivados (digite 8)\n";
		std::cout << "Leite e derivados (digite 9)\n";
		std::cout << "Bebidas (alcoólicas e não alcoólicas) (digite 10)\n";
		std::cout << "Ovos e derivados (digite 11)\n";
		std::cout << "Miscelâneas (molhos, condimentos e temperos) (digite 12)\n";
		std::cout << "Outros alimentos industrializados (molhos, condimentos e temperos) (digite 13)\n";
		std::cout << "Alimentos preparados (digite 14)\n";
		std::cout << "Digite uma opcao (1 - 14): ";
		resposta_int = ler_num_no_intervalo(1, 14);
		if (!resposta_int)
			continue;
	}
	categoria = static_cast<CategoriaAlimento>(resposta_int-1);
	
	//2) Passo a variavel do tipo CategoriaAlimento para o objeto alimento
	alimento.setCategoria(categoria);
	
	double valor_nutriente;
	std::string nome_nutriente, nome_unidade;
	std::string macronutrientes[10] = {"Energia ou Calorias", "Proteinas", "Carboidratos",
	"Lipidios", "Colesterol", "FibraAlimentar"};
	std::string micronutrientes[10] = {"Calcio", "Magnesio", "Sodio", "Potassio", "Ferro", 
	"VitaminaA/Retinol", "VitaminaB1/Tiamina", "VitaminaC"};
	std::string unidades = {"gramas/g", "kilocalorias/kcal", "miligramas/mg", "microgramas/mcg"};

	std::cout << "Informacoes dos macronutrientes do alimento.........\n";
	std::cout << "Coloque 0 (zero) caso nao saiba a quantidade\n";
	for (int i=0; i<6; i++) { // para cada um dos nutrientes
		nome_nutriente = macronutrientes[i];
		nome_unidade = unidades[0];
		if (nome_nutriente == "Colesterol")
			nome_unidade = unidades[2];
		while (true) {
			std::cout << nome_nutriente << " (em " << nome_unidade << ") (valor decimal): "
			valor_nutriente = ler_double();
			if (!nutriente)
				continue;
		}
		
		// 3) Passo os macronutrientes (um de cada vez) para o objeto alimentoo
		alimento.setMacroNutriente(valor_nutriente, nome_nutriente);
	}
	std::cout << "Informacoes dos micronutrientes do alimento.........\n";
	std::cout << "Coloque 0 (zero) caso nao saiba a quantidade\n";
	for (int i=0; i<6; i++) { // para cada um dos nutrientes
		nome_nutriente = micronutrientes[i];
		nome_unidade = unidades[2];
		if (nome_nutriente == "VitaminaA/Retinol")
			nome_unidade = unidades[3];
		while (true) {
			std::cout << nome_nutriente << " (em " << nome_unidade << ") (valor decimal): "
			valor_nutriente = ler_double();
			if (!nutriente)
				continue;
		}
		// 4) Passo os micronutrientes (um de cada vez) para o objeto alimentoo
		alimento.setMicroNutriente(valor_nutriente, nome_nutriente);
	}
	
	_diario.adicionarAlimentoAoDB(alimento);
	
	std:cout << "Cadastro do novo alimento realizado com sucesso\n";
	return nome;
}

void verAlimentosAdicionados() {
	int resposta_int;
	std::string refeicao, categoria;
	
	std::cout << "\nAlimentos ja adicionados ao Diario Alimentar\n";
	std::cout << "Escolha um refeicao:\n";
	while (true) {
		std::cout << "Todas as refeicoes (digite 1)\n";
		std::cout << "Cafe da Manha (digite 2)\n";
		std::cout << "Almoco (digite 3)\n";
		std::cout << "Jantar (digite 4)\n";
		std::cout << "Lanche (digite 5)\n";
		resposta_int = ler_num_no_intervalo(1, 5);
		if (!resposta_int)
			continue;
		break;
	}
	if (resposta_int == 1)
		refeicao = "all";
	else
		refeicao = static_cast<Refeicao>(resposta_int-2);

	
	while (true) {
		std::cout << "\nEscolha uma categoria:\n";
		std::cout << "Categoria (escolha uma opcao abaixo): ";
		std::cout << "Todas as categorias (digite 1)\n";
		std::cout << "Cereais e Derivados (digite 2)\n";
		std::cout << "Verduras, hortaliças e derivados (digite 3)\n";
		std::cout << "Frutas e derivados (digite 4)\n";
		std::cout << "Leguminosas e derivados (digite 5)\n";
		std::cout << "Nozes e sementes (digite 6)\n";
		std::cout << "Gorduras e óleos (digite 7)\n";
		std::cout << "Pescados e frutos do mar (digite 8)\n";
		std::cout << "Carnes e derivados (digite 9)\n";
		std::cout << "Leite e derivados (digite 10)\n";
		std::cout << "Bebidas (alcoólicas e não alcoólicas) (digite 11)\n";
		std::cout << "Ovos e derivados (digite 12)\n";
		std::cout << "Miscelâneas (molhos, condimentos e temperos) (digite 13)\n";
		std::cout << "Outros alimentos industrializados (molhos, condimentos e temperos) (digite 14)\n";
		std::cout << "Alimentos preparados (digite 15)\n";
		std::cout << "Digite uma opcao (1 - 14): ";
		resposta_int = ler_num_no_intervalo(1, 15);
		if (!resposta_int)
			continue;
		break;
	}
	
	if (resposta_int == 1)
		categoria = "all";
	categoria = static_cast<CategoriaAlimento>(resposta_int-2);
	
	_diario.exibirAlimentosCadastrados(refeicao, categoria);
}

void editarAlimentosAdicionados() {
	std::string nome_alimento;
	int resposta_int;
	
	// 1) Mostra todos os alimentos cadastrados
	_diario.exibirAlimentosCadastrados();
	
	// 2) Le o nome do alimento que o usuario deseja editar
	std::cout << "\nEditar Diario Alimentar\n";
	while (true) {
		std::cout << "Nome do alimento: ";
		nome_alimento = ler_nome();
		if (nome_alimento.empty())
			continue;
	}
	// 3) Vejo se esse alimento esta no Diario
	nome_alimento = _diario.buscarAlimentoDiario(nome_alimento); // retorna "" se nao encontrar o nome
	if (nome_alimento.empty())
		std::cout << "Alimento nao encontrado";
	else {
		std::cout << "Alimento encontrado\n";
		_diario.mostrarInformacoes(nome_alimento); // mostra as informacoes do alimento
		while (true) {
			std::cout << "Remover esse alimento do diario (digite 1)\n";
			std::cout << "Alterar quantidade consumida (digite 2)\n";
			std::cout << "Voltar ao menu principal (digite 3)\n";
			std::cout << "Digite uma opcao (1 - 3): ";
			resposta_int = ler_num_no_intervalo(1, 3);
			if (!resposta_int)
				continue;
		}
		switch (resposta_int) {
			case 1:
				_diario.remover(nome_alimento);
				break
			case 2:
				int quantidade, nova_quantidade;
				std::string unidade_medida;
				quantidade = _diario.getAlimento(nome_alimento).getQuantidade();
				unidade_medida = _diario.getAlimento(nome_alimento).getUnidadeMedida();
				std::cout << "Antiga quantidade de alimento\n";
				std::cout << "Nome: " << nome_alimento << std::endl;
				std::cout << "Quantidade (em" << unidade_medida << ") " << quantidade << std::endl;
				std::cout << "Nova quantidade de alimento\n"
				while (true) {
					std::cout << "Quantidade (em " << unidade_medida << "): ";
					resposta_int = ler_inteiro();
					if (!resposta_int)
						continue;
					break;
				}
				_diario.mudarQuantidade(nome_alimento, nova_quantidade);
				break;
			case 3:
				std::cout << "Voltando ao menu principal";
				return;
		}
	}
	
	while (true) {
		std::cout << "Editar outro alimento (digite 1)\n";
		std::cout << "Voltar ao menu principal (digite 2)\n";
		resposta_int = ler_num_no_intervalo(1, 2);
		if (!resposta_int)
			continue;
	}
	switch (resposta_int) {
		case 1:
			editarAlimentosAdicionados(); // chamo a minha funcao recursivamente
			break;
		case 2:
			std::cout << "Voltando ao menu principal"; // acabo a minha funcao
			return;
	}
}

void verNutrientesCalculados() {
	
	std::string macronutrientes[10] = {"Energia ou Calorias", "Proteinas", "Carboidratos",
	"Lipidios", "Colesterol", "FibraAlimentar"};
	std::string micronutrientes[10] = {"Calcio", "Magnesio", "Sodio", "Potassio", "Ferro", 
	"VitaminaA/Retinol", "VitaminaB1/Tiamina", "VitaminaC"};
	int resposta_int;
	
	std::cout << "Nutrientes..........\n";
	// exibi a quantidade absoluta de nutrientes e as porcentagens
	// em relacao a dieta (que esta dentro do PerfilNutricional
	_perfilNutricional.getMetricasUsuario();
	_diario.exibirNutrientesTotais(); 
	
	while (true) {
		std::cout << "\nAnalisar os nutreientes por refeicao (digite 1)\n";
		std::cout << "Analisar os macronutrientes (digite 2)\n";
		std::cout << "Analisar os micronutrientes (digite 3)\n";
		std::cout << "Voltar ao menu principal (digite 4)\n";
		resposta_int = ler_num_no_intervalo(1, 4);
		if (!reposta_int)
			continue;
		break;
	}
	
	switch (resposta_int) {
		case 1:
			while (true) {
				std::cout << "\nEscolha uma refeicao para verificar os nutrientes:\n";
				std::cout << "Cafe da manha (digite 1)\n";
				std::cout << "Almoco (digite 2)\n";
				std::cout << "Jantar (digite 3)\n";
				std::cout << "Lanche (digite 4)\n";
				std::cout << "Digite uma opcao (1 - 4)\n";
				resposta_int = ler_num_no_intervalo(1, 4);
				if (!reposta_int)
					continue;
				break;
			}
			break;
		case 2:
			std::cout << "Analisar macronutriente especifico:\n";
			while (true) {
				for (int i=0; i<6; i++)
					std::cout << macronutrientes[i] << " (digite " << i+1 << ")\n";
				std::cout << "Digite uma opcao (1 - 6): ";
				resposta_int = ler_num_no_intervalo(1, 6);
				if (!resposta_int)
					continue;
				break;
			}
			break;
		case 3:
			std::cout << "Analisar micronutriente especifico:\n";
			while (true) {
				for (int i=0; i<6; i++)
					std::cout << micronutrientes[i] << " (digite " << i+1 << ")\n";
				std::cout << "Digite uma opcao (1 - 6): ";
				resposta_int = ler_num_no_intervalo(1, 6);
				if (!resposta_int)
					continue;
				break;
			}
			break;
		case 4:
			std::cout << "Voltando ao menu principal\n";
			return;
			break;
	
	}
	
	std::cout << "Ver nutrientes consumidos no cafe da manha\n";
	std::cout << "Ver nutrientes consumidos no almoco\n";
	std::cout << "Ver nutrientes consumidos no jantar\n";
	std::cout << "Ver nutrientes consumidos no lanche\n";
	
	std::cout << "Analisar as calorias consumidas:\n";
	
	
	
	std::cout << "Analisar micronutriente especifico:\n";
	while (true) {
		for (int i=0; i<6; i++)
			std::cout << micronutrientes[i] << " (digite " << i+1 << ")\n";
		std::cout << "Digite uma opcao (1 - 8): ";
		resposta_int = ler_num_no_intervalo(1, 8);
		if (!resposta_int)
			continue;
		break;
	}
}

void criarReceita() {
	std::string nome_ingrediente;
	int quantidade;
	std::string medida;
	int resposta_int;
	
	std::cout << "\nCriando nova receita.........\n";
	
	int i = 1;
	while (true) {
		std::cout << "Ingrediente " << i << std::endl;
		
		// 1) Nome do alimento
		while (true) {
			std::cout << "Nome: ";
			nome_ingrediente = lerAlimento();
			if (nome_ingrediente.empty()) {
				std::cout << "Alimento nao encontrado\n";
				while (true) {
					std::cout << "Continuar receita (digite 1)\n";
					std::cout << "Voltar ao menu principal (digite 2)\n";
					resposta_int = ler_num_no_intervalo(1, 2);
					if (!resposta_int)
						continue;
					break;
				}
				switch (resposta_int) {
					case 1:
						continue; // reinicia o loop do i-esimo ingrediente
					case 2:
						std::cout << "Voltando ao menu principal\n";
						return; // termina a funcao
					
				}
			}
			break;
		}
		// 2) Unidade de Medida
		
		// CRIAR A FUNCAO LER MEDIDA.....................................
		std::string medidas_alimento[50] = buscarMedidas(_diario.buscarSubClasse(nome_alimento))
		int num_medidas = buscarNumMedidas(_diario.buscarSubClasse(nome_alimento))
		while (true) {
			std::cout << "Unidade de medida";
			std::cout << "Medidas disponiveis: ";
			for (int i=0; i<num_medidas; i++) {
				if (i == num_medidas-1)
					std::cout << medidas_alimento[i] << "\n";
				else
					std::cout << medidas_alimento[i] << ", ";
			}
			std::cout << "Medida: ";
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