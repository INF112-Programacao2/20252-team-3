#include <iostream>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <memory>
#include <iomanip>
#include <limits>

#include "usuario.hpp"
#include "funcoes_leitura.hpp"
#include "perfil_nutricional.hpp"
#include "diario_alimentar.hpp"
#include "solido_contavel.hpp" 
#include "solido_nao_contavel.hpp"
#include "liquido.hpp"
#include "alimento.hpp"
//#include "receita.hpp"

namespace fs = std::filesystem; // namespace para filesystem

// Construtores e Destrutores
Usuario::Usuario() {_diario.preencherAlimentosNoVetor();} // assim que eu crio a classe usuario, ja leio todo o csv e coloco dentro de Diario Alimentar
// CONSTRUTOR REMOVIDO
//Usuario::Usuario(std::string nome, int idade, int peso, double altura):
//	_nome(nome), _idade(idade), _peso(peso), _altura(altura) {}
Usuario::~Usuario() {} 

// Getters
std::string Usuario::getNome() {return _nome;}
int Usuario::getIdade() {return _perfilNutricional.getIdade();}
double Usuario::getPeso() {return _perfilNutricional.getPesoKg();}
double Usuario::getAltura() {return _perfilNutricional.getAlturaCm();}
std::string Usuario::getNivelAtividade() {return _perfilNutricional.getNivelAtividade();}
PerfilNutricional Usuario::getPerfilNutricional() {return _perfilNutricional;}
DiarioAlimentar& Usuario::getDiarioAlimentar() {return _diario;}
std::string Usuario::getCaminhoDiretorioDiario() {return _caminhoDiretorioDiario;}

// Setters
void Usuario::setNome(std::string nome) {_nome = nome;}
void Usuario::setIdade(int idade) {_perfilNutricional.setIdade(idade);}
void Usuario::setPeso(int peso) {_perfilNutricional.setPesoKg(peso);}
void Usuario::setAltura(double altura) {_perfilNutricional.setAlturaCm(altura);}
void Usuario::setNivelAtividade(std::string nivel_atividade)  {_perfilNutricional.setNivelAtividade(nivel_atividade);}

bool Usuario::lerCaminhoDiretorioDiario() { // chamado pela funcao main na Etapa 2
	// variaveis auxiliares
	std::string caminho_diretorio;
	int resposta_int;
	bool ha_usuario_diretorio = true;
	bool aviso = false;
	
	// variaveis para caminho do diretorio
	std::string novo_caminho;
	fs::directory_iterator primeiro_arq_diretorio; // sera inicializado
	fs::directory_iterator fim_diretorio; // fica vazio
	
	std::cout << "\nEscolher diretorio para salvar dados.............\n";
	std::cout << 
	"Os dados dos alimentos e nutrientes cadastrados nesse\n" <<
	"programa serao salvos em arquivos txt em um diretorio \n" <<
	"de sua escolha e dentro desse diretorio sera gerado a \n" <<
	"pasta DIARIO_ALIMENTAR, onde ficara salvo os dados\n";
	
	while (true) {
		std::cout << "\nDiretorio.........................\n";
		std::cout << "1. Escolher novo diretorio\n";
		std::cout << "2. Ja tenho um diretorio salvo para o programa\n";
		std::cout << "3. Voltar para o menu\n";
		std::cout << "Digite uma opcao de (1-3): ";
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
					novo_caminho = caminho_diretorio + "\\DIARIO_ALIMENTAR"; // coloco 2 \ ("\\") para mostrar que nao \ de escape
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
				std::cout << "Usuarios ja cadastrados:\n";
				try {
					if (fs::is_empty(caminho_diretorio)) // funcao me disse se o diretorio tem conteudo dentro 
						throw std::invalid_argument("Diretorio vazio\nNao ha nenhum usuario cadastrado ainda :(");
				} catch (std::invalid_argument &e) {
					std::cerr << e.what() << std::endl;
					ha_usuario_diretorio = false;
				}
				
				primeiro_arq_diretorio = fs::directory_iterator(caminho_diretorio);
				fim_diretorio; // fica vazio
					
				if (ha_usuario_diretorio) {
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
		std::cout << "Caminho do diretorio para Diario Alimentar do usuario salvo!\n";
		break; // break para loop while
	}
	
	// Atribuindo caminho para membro da classe
	_caminhoDiretorioDiario = caminho_diretorio + "/DIARIO_ALIMENTAR";
	
	return true;
}
bool Usuario::cadastrarUsuario() {
	int resposta_int;
	char resposta_char;
	bool confirmou_cadastro = false;
	
	while (true) {
		std::cout << "\nCadastro do usuario..........\n";
		std::cout << "1. Criar novo usuario\n";
		std::cout << "2. Abrir conta de usuario\n";
		std::cout << "3. Voltar para o menu\n";
		std::cout << "Digite uma opcao de (1-3): ";
		resposta_int = ler_num_no_intervalo(1, 3);
		if (!resposta_int)
			continue;
		
		switch (resposta_int) {
			case 1:
				// CADASTRO DE NOVO USUARIO
				std::cout << "\nNovo Usuario.............\n";
				leituraDadosUsuario(); // le os dados e guarda
				
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
				if (!salvarDadosEmArquivo()) { // metodo faz a criacao e salvamento dos dados do usuario
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

void Usuario::leituraDadosUsuario() {
    std::string sexo, nome;
    int idade; 
    double altura_m, peso_kg_int;
    
    std::cout << "\n========================================\n";
    std::cout << "     CADASTRO DE DADOS DO USUARIO\n";
    std::cout << "========================================\n";

    // --- 1. Leitura de Dados Pessoais ---

    // Nome (Armazenado em _nome do Usuario)
    while (true) {
        std::cout << "Nome (sem caracteres especiais): ";
        nome = ler_nome();
        if (!nome.empty())
            break;
    }
    // Idade
    while (true) {
        std::cout << "Idade (numero inteiro em anos): ";
        idade = ler_inteiro();
        if (idade > 0 && idade < 120) 
            break;
    }
    // Peso (Em kg)
    while (true) {
        std::cout << "Peso (numero decimal em metros - use ponto): ";
        peso_kg_int = ler_double();
        if (peso_kg_int > 0)
            break;
    }
    // Altura (Em metros)
    while (true) {
        std::cout << "Altura (numero decimal em metros - use ponto): ";
        altura_m = ler_double();
        if (altura_m > 0.5 && altura_m < 3.0) 
            break;
    }

    // --- 2. Leitura de Dados Nutricionais (Sexo e Nivel de Atividade) ---
    
    // Sexo
    while (true) {
        std::cout << "\nSexo (Masculino/Feminino): ";
        std::cin >> sexo;
        if (sexo == "Masculino" || sexo == "Feminino")
            break;
        std::cout << "Opcao invalida. Digite 'Masculino' ou 'Feminino'.\n";
    }

    // Nivel de Atividade Fisica
    std::string fatores_fisicos[] = {
        "Sedentario", "Leve", "Moderado", "Ativo", "Muito Ativo"
    };
    int resposta_int;
    std::string nivel_atividade;

    while (true) {
        std::cout << "\nNivel de atividade fisica (selecione uma opcao):\n";
        std::cout << "1 - Sedentario\n";
        std::cout << "2 - Leve\n";
        std::cout << "3 - Moderado\n";
        std::cout << "4 - Ativo\n";
        std::cout << "5 - Muito Ativo\n";
        std::cout << "Digite uma opcao (1-5): ";
        
        resposta_int = ler_num_no_intervalo(1, 5);
        if (resposta_int != 0) {
            nivel_atividade = fatores_fisicos[resposta_int - 1];
            break;
        }
    }

    // --- 3. Instanciação do Perfil Nutricional e Calculo da CDT ---
    
    double altura_cm = altura_m * 100.0; // PerfilNutricional usa cm
    
    // Instancia o objeto _perfilNutricional, que calcula _caloriasDiariasTotais
    _perfilNutricional = *std::make_unique<PerfilNutricional>(
		sexo,
		idade,
		(double)peso_kg_int,
		altura_cm,
		nivel_atividade
	);
    
    // --- 4. Meta Calórica (Decisão do Usuario) ---
    
    double meta_calorica_recomendada = _perfilNutricional.getCaloriasDiariasTotais();
    char resposta_char;
    double meta_calorica_final;

    while (true) {
        std::cout << "\nMeta calorica diaria (consumo)\n";
        std::cout << "Recomendado (calculado): " << meta_calorica_recomendada << " kcal\n";
        std::cout << "Seguir com valor recomendado [S/N]: ";
        resposta_char = ler_S_ou_N();
        if (resposta_char != '\0')
            break;
    }
    
    if (resposta_char == 'S') {
        meta_calorica_final = meta_calorica_recomendada;
    } else { // resposta_char == 'N'
        while (true) {
            std::cout << "\nDigite a sua meta calorica personalizada (numero decimal): ";
            meta_calorica_final = ler_double();
            // Validação simples: meta deve ser positiva
            if (meta_calorica_final > 0)
                break;
        }
    }
    
    // --- 5. Armazenamento Final ---
    
    // Armazena o nome na classe Usuario
    _nome = nome; 

    // Se a meta final for diferente da calculada, sobrescreve o valor no PerfilNutricional.
    if (meta_calorica_final != meta_calorica_recomendada) {
         _perfilNutricional.setCaloriasDiariasTotais(meta_calorica_final);
    }
    
    std::cout << "Meta calorica final (" << meta_calorica_final << " kcal) salva!\n";
    std::cout << "Cadastro de dados do usuario concluido.\n";
}

void Usuario::verDadosUsuario() const {
    std::cout << "\n========================================\n";
    std::cout << "     RELATÓRIO DOS DADOS DO USUARIO\n";
    std::cout << "========================================\n";

    // 1. Dados da classe Usuario
    std::cout << "Nome: " << _nome << std::endl;
    
    // 2. Dados acessados via _perfilNutricional (ponteiro)
    std::cout << "Idade: " << _perfilNutricional.getIdade() << " anos\n";
    std::cout << "Peso: " << _perfilNutricional.getPesoKg() << " kg\n";
    
    // Converto altura para metros para exibir
    std::cout << "Altura: " << _perfilNutricional.getAlturaCm() / 100.0 << " metros\n"; 
    
    std::cout << "Sexo: " << _perfilNutricional.getSexo() << std::endl;
    std::cout << "Nivel de Atividade: " << _perfilNutricional.getNivelAtividade() << std::endl;
    
    // 3. Meta Calorica (CDT)
    std::cout << "Meta Calorica Diaria: " << _perfilNutricional.getCaloriasDiariasTotais() << " kcal" << std::endl;
    
    std::cout << "========================================\n";
}

void Usuario::editarDadosUsuario() {
    int opcao;
    bool perfil_alterado = false; // Flag para saber se precisamos recalcular a CDT
    
    std::cout << "\n========================================\n";
    std::cout << "     EDIÇÃO DO PERFIL DO USUARIO\n";
    std::cout << "========================================\n";
    
    // Mostra as opções e lê a escolha do usuario
    while (true) {
        std::cout << "\nQual dado voce deseja editar?\n";
        std::cout << "1. Nome: " << _nome << std::endl;
        std::cout << "2. Idade: " << _perfilNutricional.getIdade() << " anos" << std::endl;
        std::cout << "3. Peso: " << _perfilNutricional.getPesoKg() << " kg" << std::endl;
        std::cout << "4. Altura: " << _perfilNutricional.getAlturaCm() / 100.0 << " metros" << std::endl;
        std::cout << "5. Sexo: " << _perfilNutricional.getSexo() << std::endl;
        std::cout << "6. Nivel de Atividade: " << _perfilNutricional.getNivelAtividade() << std::endl;
        std::cout << "7. Sair da Edicao\n";
        std::cout << "Digite uma opcao (1-7): ";

        opcao = ler_num_no_intervalo(1, 7);

        if (opcao == 7) {
            break; // Sai do loop de edição
        }

        switch (opcao) {
            case 1: { // Nome (Apenas na classe Usuario)
                std::string novo_nome;
                std::cout << "Novo Nome: ";
                novo_nome = ler_nome();
                if (!novo_nome.empty()) {
                    _nome = novo_nome;
                    // Não afeta a CDT, perfil_alterado = false
                }
                break;
            }
            case 2: { // Idade (Afeta a CDT)
                int nova_idade;
                std::cout << "Nova Idade (anos): ";
                nova_idade = ler_inteiro();
                if (nova_idade > 0) {
                    _perfilNutricional.setIdade(nova_idade);
                    perfil_alterado = true;
                }
                break;
            }
            case 3: { // Peso (Afeta a CDT)
                double novo_peso;
                std::cout << "Novo Peso (kg): ";
                novo_peso = ler_double();
                if (novo_peso > 0.0) {
                    _perfilNutricional.setPesoKg(novo_peso);
                    perfil_alterado = true;
                }
                break;
            }
            case 4: { // Altura (Afeta a CDT)
                double nova_altura_m;
                std::cout << "Nova Altura (metros - use ponto): ";
                nova_altura_m = ler_double();
                if (nova_altura_m > 0.5) {
                    _perfilNutricional.setAlturaCm(nova_altura_m * 100.0); // Converte para CM
                    perfil_alterado = true;
                }
                break;
            }
            case 5: { // Sexo (Afeta a CDT)
                std::string novo_sexo;
                std::cout << "Novo Sexo (Masculino/Feminino): ";
                novo_sexo = ler_nome(); // Assume função de leitura validada
                if (novo_sexo == "Masculino" || novo_sexo == "Feminino") {
                    _perfilNutricional.setSexo(novo_sexo);
                    perfil_alterado = true;
                }
                break;
            }
            case 6: { // Nivel de Atividade (Afeta a CDT)
                std::string novo_nivel;
                std::cout << "Novo Nivel de Atividade (Sedentario, Leve, Moderado, Ativo, Muito Ativo): ";
                novo_nivel = ler_nome(); // Assume função de leitura validada
                if (!novo_nivel.empty()) {
                    _perfilNutricional.setNivelAtividade(novo_nivel);
                    perfil_alterado = true;
                }
                break;
            }
        }
    }

    // --- Recalculo e Atualização ---
    if (perfil_alterado) {
        // Chama o método da classe PerfilNutricional para recalcular a CDT
        _perfilNutricional.atualizarPerfil();
        std::cout << "\nPerfil nutricional atualizado com sucesso!";
        std::cout << "\nNova Meta Calórica Diaria: " << _perfilNutricional.getCaloriasDiariasTotais() << " kcal.\n";
    } else {
        std::cout << "\nNenhuma alteracao que afete a meta calorica foi salva.\n";
    }
}

bool Usuario::salvarDadosEmArquivo() const {
    // 1. Verificação inicial do Perfil
	// Nao foi necessario
	
    std::ofstream cin_arquivo; // Objeto ofstream reutilizado para ambos os arquivos
    bool sucesso = true;

    // --- Nome dos Arquivos (Conforme Requisitado) ---
    std::string nome_arquivo_pessoal = "DadosPessoais_" + _nome + ".txt";
    std::string nome_arquivo_nutricional = "DadosNutricionais_" + _nome + ".txt";

    // --- Caminho Completo (Usando o atributo _caminhoDiretorioDiario) ---
    // Cria o caminho completo concatenando o diretorio, a barra (/) e o nome do arquivo.
    std::string caminho_arquivo_pessoal = _caminhoDiretorioDiario + "/" + nome_arquivo_pessoal;
    std::string caminho_arquivo_nutricional = _caminhoDiretorioDiario + "/" + nome_arquivo_nutricional;

    // ===============================================
    // 2. Escrevendo o arquivo com os DADOS PESSOAIS
    // ===============================================
    
    cin_arquivo.open(caminho_arquivo_pessoal); 
    try {
        if (cin_arquivo.is_open()) {
            cin_arquivo << "========================================" << std::endl;
            cin_arquivo << " DADOS PESSOAIS DO USUARIO: " << _nome << std::endl;
            cin_arquivo << "========================================" << std::endl;
            cin_arquivo << std::fixed << std::setprecision(2);
            
            // Dados agora acessados via getters do _perfilNutricional
            cin_arquivo << "Nome: " << _nome << std::endl;
            cin_arquivo << "Sexo: " << _perfilNutricional.getSexo() << std::endl;
            cin_arquivo << "Idade: " << _perfilNutricional.getIdade() << " anos" << std::endl;
            cin_arquivo << "Peso: " << _perfilNutricional.getPesoKg() << " kg" << std::endl;
            // Altura em metros
            cin_arquivo << "Altura: " << (_perfilNutricional.getAlturaCm() / 100.0) << " metros" << std::endl; 
            
            cin_arquivo.close();
            std::cout << "\nDados pessoais salvos em: " << caminho_arquivo_pessoal << std::endl;
        } else {
            throw std::logic_error("Nao foi possivel abrir o arquivo de Dados Pessoais.");
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro ao salvar dados pessoais: " << e.what() << std::endl;
        sucesso = false;
    }
    
    // ===============================================
    // 3. Escrevendo o arquivo com os DADOS NUTRICIONAIS
    // ===============================================
    
    // Abrindo o segundo arquivo
    cin_arquivo.open(caminho_arquivo_nutricional); 
    try {
        if (cin_arquivo.is_open()) {
            cin_arquivo << "========================================" << std::endl;
            cin_arquivo << " DADOS NUTRICIONAIS DO USUARIO: " << _nome << std::endl;
            cin_arquivo << "========================================" << std::endl;
            cin_arquivo << std::fixed << std::setprecision(1); // Precisao para calorias
            
            // Fator Fisico (agora Nivel de Atividade)
            cin_arquivo << "Nivel de Atividade: " << _perfilNutricional.getNivelAtividade() << std::endl;
            // Meta Calorica (agora Calorias Diarias Totais)
            cin_arquivo << "Meta Calorica Diaria (CDT): " << _perfilNutricional.getCaloriasDiariasTotais() << " kcal" << std::endl;
            
            cin_arquivo.close();
            std::cout << "\nDados nutricionais salvos em: " << caminho_arquivo_nutricional << std::endl;
        } else {
            throw std::logic_error("Nao foi possivel abrir o arquivo de Dados Nutricionais.");
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro ao salvar dados nutricionais: " << e.what() << std::endl;
        sucesso = false;
    }
            
    return sucesso;
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
	int resposta_int;

	while (true) {
		std::cout << "\nSistema de busca.....\n";
		std::cout << "Nome do Alimento: ";
		nome_alimento = ler_nome();
		if (nome_alimento.empty())
			continue;
		break;
	}
	std::cout << "Buscando no Banco de Dados " << nome_alimento << "...\n";
	try {
		ID_alimento_match = _diario.buscarAlimentoPeloNome(nome_alimento); // retorna o endereco para um array dos IDs
		if (ID_alimento_match == nullptr)
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
		int num_de_IDs_alimento_match;
		try {
			num_de_IDs_alimento_match = ID_alimento_match[0];
		} catch (std::exception &e) {
			std::cout << "ERRO FATAL\nFim do programa\n";
			exit(0);
		}
		
		std::cout << "\nLista dos alimentos encontrados\n";
		for (int i=1; i<num_de_IDs_alimento_match; i++) {
			try {
				nome_alimento = _diario.buscarAlimentoPeloID(ID_alimento_match[i]);
			}
			catch (std::out_of_range &e) {
				std::cerr << "Acesso a posicao invalida do vetor\nFim do programa";
				exit(0);
			}
			catch (std::exception &e) {
				std::cerr << "Erro inesperado " << e.what() << std::endl;
			}
			std::cout << i << ") " << nome_alimento;
			
		}
		
		// Decidir quais dos alimentos o usuario quer
		while (true) {
			std::cout << "Escolha uma opcao (1 - " << num_de_IDs_alimento_match << ")\n";
			resposta_int = ler_num_no_intervalo(1, num_de_IDs_alimento_match);
			if (!resposta_int)
				continue;
		}
		nome_alimento = _diario.buscarAlimentoPeloID(ID_alimento_match[resposta_int]);
		std::cout << "Alimento escolhido " << nome_alimento << std::endl;
		
		return nome_alimento;
	}
}

void Usuario::adicionarAlimentoAoDiario() {

	// O unique_ptr para o alimento de consumo do usuario. 
	// Inicialmente nulo.
	std::unique_ptr<Alimento> alimento_do_usuario = nullptr; 
	Alimento* alimento_modelo = nullptr; // Ponteiro bruto para o modelo do BD (não gerenciado aqui)
	
	int resposta_int;
	std::string nome_alimento;
	
	// Bloco de entrada inicial (mantido)
	while (true) {
		std::cout << "\nAdicionar alimentos ao Diario Alimentar.........\n";
		std::cout << "1. Cadastrar alimento ao Diario\n";
		std::cout << "2. Voltar ao menu principal\n";
		std::cout << "Digite sua opcao (1 - 2): ";
		resposta_int = ler_num_no_intervalo(1, 2);
		if (!resposta_int)
			continue;
		break;
	}
	
	// variaveis auxiliares
	char resposta_char;
	bool confirmou_cadastro = false;
	int num_medidas;
	int quantidade;
	std::string unidade_medida;
	std::vector<std::string> medidas_alimento;
	std::string refeicao_escolhida;
    std::string subclasse = alimento_modelo->getSubClasse();
			
	switch (resposta_int) {
		case 1:
			nome_alimento = lerAlimento(); 
			if (nome_alimento.empty()) {
				// Bloco de tratamento de alimento não encontrado (mantido)
				while (true) {
					std::cout << "\nAlimento nao encontrado................\n";
					std::cout << "1. Tentar adicionar o alimento novamente\n";
					std::cout << "2. Criar um novo alimento e adiciona-lo ao banco de dados\n";
					std::cout << "3. Voltar ao Menu Principal\n";
					std::cout << "Digite sua opcao (1 - 3): ";
					resposta_int = ler_num_no_intervalo(1, 3);
					if (!resposta_int)
						continue;
					break;
				}
				switch (resposta_int) {
					case 1:
						adicionarAlimentoAoDiario(); // Chamada recursiva
						return;
					case 2:
						nome_alimento = cadastrarNovoAlimentoDB(); // ao termino da funcao, o alimento estara no vetor modelo
						break;
					case 3:
						std::cout << "Voltando ao menu principal\n";
						return;
				}
			}
			
			// 1) Obter o modelo base do banco de dados (Diario)
			alimento_modelo = _diario.getAlimento(nome_alimento);

			try {
				if (alimento_modelo == nullptr)
					throw std::logic_error("Alimento nao encontrado no banco de dados apos a leitura.");
			} catch (std::logic_error &e) {
				std::cerr << "ERRO FATAL: " << e.what() << " O alimento " << nome_alimento << " nao pode ser carregado.\n";
				exit(0);
			}

			// 2) Instanciar o objeto da subclasse correta (Factory Manual)
            
            if (subclasse == "SolidoContavel") {
                // Instancia um SolidoContavel usando o construtor de cópia do modelo
                alimento_do_usuario.reset(new SolidoContavel(*static_cast<SolidoContavel*>(alimento_modelo)));
            } else if (subclasse == "SolidoNaoContavel") {
                // Instancia um SolidoNaoContavel usando o construtor de cópia do modelo
                alimento_do_usuario.reset(new SolidoNaoContavel(*static_cast<SolidoNaoContavel*>(alimento_modelo)));
            } else if (subclasse == "Liquido") {
                // Instancia um Liquido usando o construtor de cópia do modelo
                alimento_do_usuario.reset(new Liquido(*static_cast<Liquido*>(alimento_modelo)));
            } else {
                // Tratar o caso de subclasse desconhecida
                std::cerr << "ERRO FATAL: Subclasse do alimento (" << subclasse << ") desconhecida.\n";
                exit(0);
            }
			
			// O objeto alimento_do_usuario ja tem o nome e outras propriedades do modelo base.

			// Prosseguir com o cadastro dos dados de consumo do usuario
			// Ler a Quantidade e a Unidade de Medida
			
			// Usar o modelo para buscar medidas
			medidas_alimento = buscarMedidas(alimento_modelo->getSubClasse());
			num_medidas = buscarNumMedidas(alimento_modelo->getSubClasse());
			
			std::cout << "\nQuantidade do alimento.........\n";
			while (true) {
				// ... (Lógica de seleção de unidade de medida mantida) ...
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
						medidas_alimento = buscarMedidas();
						num_medidas = buscarNumMedidas();
						std::cout << "Todas as unidades de medida (selecione uma)\n";
						for (int i=0; i<num_medidas; i++) {
							std::cout << "Em " << medidas_alimento[i] << " (digite " << i+1 << ")\n";
						}
						resposta_int = ler_num_no_intervalo(1, num_medidas);
						if (!resposta_int)
							continue;
						break;
					}
					unidade_medida = medidas_alimento[resposta_int-1];
				}
					
				else
					unidade_medida = medidas_alimento[resposta_int-1];
				
				break;
			}
			
			// 2) Coloco a unidade de medida dentro do objeto alimento (DO USUARIO)
			alimento_do_usuario->setUnidadeDeMedida(unidade_medida);
			
			while (true) {
				std::cout << "Digite a quantidade de alimento (em " << unidade_medida << "): ";
				quantidade = ler_inteiro();
				if (!quantidade)
					continue;
				break;
			}
			
			// 3) Coloco a quantidade dentro do objeto alimento (DO USUARIO)
			alimento_do_usuario->setQuantidade(quantidade);
			
			// Ler a refeicao a qual o alimento pertence
			while (true) {
				// ... (Lógica de seleção de refeição mantida) ...
				std::cout << "\nRefeicao em que o alimento foi consumido\n";
				std::cout << "1. Cafe da Manha\n";
				std::cout << "2. Almoco\n";
				std::cout << "3. Jantar\n";
				std::cout << "4. Lanche\n";
				std::cout << "Digite sua opcao (1-4): ";
				resposta_int = ler_num_no_intervalo(1, 4);
				if (!resposta_int)
					continue;
				break;
			}
			
			// Mapeamento da Refeição para a string para o cout (ajustado para usar o enum)
            switch (static_cast<Refeicao>(resposta_int-1)) {
                case Refeicao::Cafe_Manha: refeicao_escolhida = "Cafe da Manha"; break;
                case Refeicao::Almoco: refeicao_escolhida = "Almoco"; break;
                case Refeicao::Jantar: refeicao_escolhida = "Jantar"; break;
                case Refeicao::Lanche: refeicao_escolhida = "Lanche"; break;
                default: refeicao_escolhida = "Erro de Refeição"; break;
            }
			
			// 4) Coloco a refeicao dentro do objeto alimento (DO USUARIO)
			alimento_do_usuario->setRefeicao(static_cast<Refeicao>(resposta_int-1));
			
			std::cout << "\nConfirmar informacoes do alimento......\n";
			std::cout << "Nome do alimento: " << nome_alimento << std::endl;
			std::cout << "Quantidade: " << alimento_do_usuario->getQuantidade() << " " << alimento_do_usuario->getUnidadeDeMedida() << std::endl;
			std::cout << "Refeicao: " << refeicao_escolhida << std::endl;
			while (true) {
				std::cout << "Adicionar alimento no Diario Alimentar [S/N]: ";
				resposta_char = ler_S_ou_N();
				if (!resposta_char)
					continue;
				else {
					if (resposta_char == 'S')
						confirmou_cadastro = true;
					else
						confirmou_cadastro = false;
				}
				break;
			}
			
			// 5) Passar o objeto alimento (a NOVA instância) para o vetor do usuario
			if (confirmou_cadastro) {
				std::cout << "Alimento adicionado ao seu Diario!\n";
				_diario.adicionarAlimentoAoVetorDoUsuario(std::move(alimento_do_usuario)); 
			}
			else {
				std::cout << "Alimento nao adicionado ao Diario\n";
			}
			
			break; // break para o case 1
							
		case 2:
			std::cout << "Voltando ao menu principal\n";
			return;
	}
	
	// 6) Adicionar mais alimentos (mantido)
	while (true) {
		std::cout << "Adicionar mais alimentos? [S/N]\n";
		resposta_char = ler_S_ou_N();
		if (!resposta_char)
			continue;
		break;
	}
	if (resposta_char == 'S')
		adicionarAlimentoAoDiario();
	else {
		std::cout << "Fim do cadastro\n";
		std::cout << "Voltando ao menu principal\n";
		return;
	}
}

std::vector<std::string> Usuario::buscarMedidas(std::string subclasse_alimento) {
	std::vector<std::string> medidas_solido_contaveis = {
		"Unidades", "Meia Duzia", "Duzia"
	};
	std::vector<std::string> medidas_solido_nao_contaveis = {
		"Xicara", "Meia Xicara", "Quarto Xicara", "Copo Americano", "Colher de Sopa", "Colher de Sobremesa",
		"Colher de Cha"
	};
	std::vector<std::string> medidas_liquido = {
		"Litros", "Mililitros"
	};
	
	std::vector<std::string> todas_medidas;
	// funcao insert(posicao em que o insert comecao, comeco do vetor inserido, fim do vetor inserido)
	todas_medidas.insert(todas_medidas.end(), medidas_solido_contaveis.begin(), medidas_solido_contaveis.end());
	todas_medidas.insert(todas_medidas.end(), medidas_solido_nao_contaveis.begin(), medidas_solido_nao_contaveis.end());
	todas_medidas.insert(todas_medidas.end(), medidas_liquido.begin(), medidas_liquido.end());
	
	
	if (subclasse_alimento.empty()) { // quando nao especifica nenhuma subclasse, retorno todas as medidas
		return todas_medidas;
	}
	else {
		if (subclasse_alimento == "SolidoContavel")
			return medidas_solido_contaveis;
		else if (subclasse_alimento == "SolidoNaoContavel")
			return medidas_solido_nao_contaveis;
		else
			return medidas_liquido;
	}
	
}
int Usuario::buscarNumMedidas(std::string subclasse_alimento) {
	return buscarMedidas(subclasse_alimento).size();
}



std::string Usuario::cadastrarNovoAlimentoDB() {
    std::unique_ptr<Alimento> novo_alimento_ptr; 
    int resposta_int;
    
    std::cout << "\nCadastro de Novo Alimento no Banco De Dados\n";
    
    std::string nome;
    std::cout << "Informacoes gerais do alimento.........\n";
    while (true) {
        std::cout << "Nome: ";
        nome = ler_nome();
        if (!nome.empty())
            break;
    }
    
    // Escolha da Subclasse
    int tipo_alimento;
    while (true) {
        std::cout << "Tipo de Alimento (escolha uma opcao abaixo):\n";
        std::cout << "  1. Solido Contavel (ex: maca, ovo)\n";
        std::cout << "  2. Solido Nao Contavel (ex: arroz, queijo)\n";
        std::cout << "  3. Liquido (ex: leite, suco)\n";
        std::cout << "Digite uma opcao (1-3): ";
        tipo_alimento = ler_num_no_intervalo(1, 3);
        if (tipo_alimento != 0)
            break;
    }
    
    // Criação do objeto da subclasse usando std::make_unique
    if (tipo_alimento == 1) {
        novo_alimento_ptr = std::make_unique<SolidoContavel>();
    } else if (tipo_alimento == 2) {
        novo_alimento_ptr = std::make_unique<SolidoNaoContavel>();
    } else { // tipo_alimento == 3
        novo_alimento_ptr = std::make_unique<Liquido>();
    }

    novo_alimento_ptr->setNome(nome);
    
    while (true) {
		std::cout << "Categoria (escolha uma opcao abaixo): \n"; // Adicionado \n para melhor formatação
		std::cout << "1. Cereais e Derivados\n";
		std::cout << "2. Verduras, hortalicas e derivados\n";
		std::cout << "3. Frutas e derivados\n";
		std::cout << "4. Leguminosas e derivados\n";
		std::cout << "5. Nozes e sementes\n";
		std::cout << "6. Gorduras e oleos\n";
		std::cout << "7. Pescados e frutos do mar\n";
		std::cout << "8. Carnes e derivados\n";
		std::cout << "9. Leite e derivados\n";
		std::cout << "10. Bebidas (alcoólicas e não alcoolicas)\n";
		std::cout << "11. Ovos e derivados\n";
		std::cout << "12. Miscelaneas (molhos, condimentos e temperos)\n";
		std::cout << "13. Outros alimentos industrializados (molhos, condimentos e temperos)\n";
		std::cout << "14. Alimentos preparados\n";
		std::cout << "Digite uma opcao (1 - 14): ";
        resposta_int = ler_num_no_intervalo(1, 14);
        if (resposta_int != 0)
            break;
    }
    novo_alimento_ptr->setCategoria(static_cast<CategoriaAlimento>(resposta_int - 1));
    
    double valor_nutriente;
    std::string nome_nutriente, nome_unidade;
    std::vector<std::string> macronutrientes = {"Energia ou Calorias", "Proteinas", "Carboidratos",
    "Lipidios", "Colesterol", "FibraAlimentar"};
    std::vector<std::string> micronutrientes = {"Calcio", "Magnesio", "Sodio", "Potassio", "Ferro", 
    "VitaminaA", "VitaminaB1", "VitaminaC"};
    std::vector<std::string> unidades = {"gramas/g", "kilocalorias/kcal", "miligramas/mg"};
    
    std::cout << "Informacoes dos macronutrientes do alimento.........\n";
    std::cout << "Coloque 0 (zero) caso nao saiba a quantidade\n";
    for (int i = 0; i < 6; i++) {
        nome_nutriente = macronutrientes[i];
        nome_unidade = unidades[0];
        if (nome_nutriente == "Energia ou Calorias")
            nome_unidade = unidades[1];
        if (nome_nutriente == "Colesterol")
            nome_unidade = unidades[2];
        while (true) {
            std::cout << nome_nutriente << " (em " << nome_unidade << ") (valor decimal): ";
            valor_nutriente = ler_double();
            if (valor_nutriente != 0.0)
                break;
        }
        
        novo_alimento_ptr->adicionarNutriente(nome_nutriente, valor_nutriente);
    }
    
    std::cout << "Informacoes dos micronutrientes do alimento.........\n";
    std::cout << "Coloque 0 (zero) caso nao saiba a quantidade\n";
    for (int i = 0; i < 7; i++) {
        nome_nutriente = micronutrientes[i];
        nome_unidade = unidades[2];
        while (true) {
            std::cout << nome_nutriente << " (em " << nome_unidade << ") (valor decimal): ";
            valor_nutriente = ler_double();
            if (valor_nutriente != 0.0)
                break;
        }
        novo_alimento_ptr->adicionarNutriente(nome_nutriente, valor_nutriente);
    }
    
    _diario.adicionarAlimentoAoDB(std::move(novo_alimento_ptr));
    
    std::cout << "Cadastro do novo alimento realizado com sucesso\n";
    return nome;
}

void Usuario::verAlimentosAdicionados() {
	int resposta_int;
	std::string refeicao;
	std::string categoria;

    // Vetor de mapeamento das refeições
    const std::vector<std::string> map_refeicoes = {
        "Todas",            // Índice 0: Mapeia opção 1
        "Cafe da Manha",    // Índice 1: Mapeia opção 2
        "Almoco",           // Índice 2: Mapeia opção 3
        "Jantar",           // Índice 3: Mapeia opção 4
        "Lanche"            // Índice 4: Mapeia opção 5
    };
    
    // Vetor de mapeamento das categorias
    const std::vector<std::string> map_categorias = {
        "Todas",                                  // Índice 0: Mapeia opção 1
        "Cereais e Derivados",                    // Índice 1: Mapeia opção 2
        "Verduras, hortaliças e derivados",       // Índice 2: Mapeia opção 3
        "Frutas e derivados",                     // Índice 3: Mapeia opção 4
        "Leguminosas e derivados",                // Índice 4: Mapeia opção 5
        "Nozes e sementes",                       // Índice 5: Mapeia opção 6
        "Gorduras e óleos",                       // Índice 6: Mapeia opção 7
        "Pescados e frutos do mar",               // Índice 7: Mapeia opção 8
        "Carnes e derivados",                     // Índice 8: Mapeia opção 9
        "Leite e derivados",                      // Índice 9: Mapeia opção 10
        "Bebidas",                                // Índice 10: Mapeia opção 11
        "Ovos e derivados",                       // Índice 11: Mapeia opção 12
        "Miscelâneas",                            // Índice 12: Mapeia opção 13
        "Outros alimentos industrializados",      // Índice 13: Mapeia opção 14
        "Alimentos preparados"                    // Índice 14: Mapeia opção 15
    };

	std::cout << "\nAlimentos ja adicionados ao Diario Alimentar\n";

    // --- 1. Escolha da Refeição ---
	std::cout << "Escolha uma refeicao:\n";
	while (true) {
        // Exibição baseada no vetor de mapeamento
        std::cout << map_refeicoes[0] << " as refeicoes (digite 1)\n"; // Exibe "Todas as refeicoes"
		for (size_t i = 0; i < map_refeicoes.size(); ++i) {
            if (i > 0) {
			    std::cout << map_refeicoes[i] << " (digite " << i + 1 << ")\n";
            }
		}
		resposta_int = ler_num_no_intervalo(1, map_refeicoes.size());
		if (!resposta_int)
			continue;
		break;
	}
    // Mapeamento: Usa a escolha para indexar o vetor
	refeicao = map_refeicoes[resposta_int - 1]; 

    // --- 2. Escolha da Categoria ---
	while (true) {
		std::cout << "\nEscolha uma categoria:\n";
		std::cout << map_categorias[0] << " as categorias (digite 1)\n"; // Exibe "Todas as categorias"
        // Exibição baseada no vetor de mapeamento
		for (size_t i = 0; i < map_categorias.size(); ++i) {
             if (i > 0) {
			    std::cout << map_categorias[i] << " (digite " << i + 1 << ")\n";
             }
		}
		std::cout << "Digite uma opcao (1 - " << map_categorias.size() << "): ";
		resposta_int = ler_num_no_intervalo(1, map_categorias.size());
		if (!resposta_int)
			continue;
		break;
	}
	
    // Mapeamento: Usa a escolha para indexar o vetor
	categoria = map_categorias[resposta_int - 1];
	
    // --- 3. Chamada da Função de Exibição ---
	_diario.exibirAlimentosNoDiario(refeicao, categoria);
}

void Usuario::editarAlimentosAdicionados() {
	std::string nome_alimento;
	int quantidade, nova_quantidade;
	std::string unidade_medida;
	int resposta_int;
	
	// 1) Mostra todos os alimentos cadastrados
	_diario.exibirAlimentosNoDiario();
	
	// 2) Le o nome do alimento que o usuario deseja editar
	std::cout << "\nEditar Diario Alimentar\n";
	while (true) {
		std::cout << "Nome do alimento: ";
		nome_alimento = ler_nome(); // nome do alimento é um nome valido (dentro do banco de dados)
		if (nome_alimento.empty())
			continue;
	}
	// 3) Vejo se esse alimento esta no Diario (e aproveito para atualizar o nome)
	if (!(_diario.buscarAlimentoNoDiario(nome_alimento))) // retorna false se nao encontrar o nome
		std::cout << "Alimento nao encontrado";
	else {
		std::cout << "Alimento encontrado\n";
		_diario.exibirInformacoes(nome_alimento); // mostra as informacoes do alimento
		while (true) {
			std::cout << "1. Remover esse alimento do diario\n";
			std::cout << "2. Alterar quantidade consumida\n";
			std::cout << "3. Voltar ao menu principal\n";
			std::cout << "Digite uma opcao (1 - 3): ";
			resposta_int = ler_num_no_intervalo(1, 3);
			if (!resposta_int)
				continue;
		}
		switch (resposta_int) {
			case 1:
				_diario.removerAlimentoDoDiario(nome_alimento);
				break;
			case 2:
				quantidade = _diario.getAlimento(nome_alimento)->getQuantidade();
				unidade_medida = _diario.getAlimento(nome_alimento)->getUnidadeDeMedida();
				std::cout << "Antiga quantidade de alimento\n";
				std::cout << "Nome: " << nome_alimento << std::endl;
				std::cout << "Quantidade (em" << unidade_medida << ") " << quantidade << std::endl;
				std::cout << "Nova quantidade de alimento\n";
				while (true) {
					std::cout << "Quantidade (em " << unidade_medida << "): ";
					resposta_int = ler_inteiro();
					if (!resposta_int)
						continue;
					break;
				}
				_diario.getAlimento(nome_alimento)->setQuantidade(nova_quantidade);
				break;
			case 3:
				std::cout << "Voltando ao menu principal";
				return;
		}
	}
	
	while (true) {
		std::cout << "1. Editar outro alimento\n";
		std::cout << "2. Voltar ao menu principal\n";
		std::cout << "Digite sua opcao (1-2): ";
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

void Usuario::verNutrientesCalculados() {
	// variaveis auxiliares
	int resposta_int_menu;     
	int resposta_int_detalhe; 
	
	// vetores para fazer o mapeamento da opcao escolhida para uma string
	const std::vector<std::string> map_refeicoes = {
        "",                     // Índice 0 (vazio)
        "Todas",                // Opção 1
        "Cafe da manha",        // Opção 2
        "Almoco",               // Opção 3
        "Jantar",               // Opção 4
        "Lanche"                // Opção 5 (engloba todos os lanches)
    };
    const std::vector<std::string> map_tipos_nutrientes = {
        "",                     // Índice 0 (vazio)
        "Todos",                // Opção 1
        "Macronutrientes",      // Opção 2
        "Micronutrientes"       // Opção 3
    };
	
	std::string arg_refeicao = "Todas";       // Valor padrão para analise de nutrientes
    std::string arg_tipo_nutriente = "Todos"; // Valor padrão para analise de refeições

	std::cout << "Nutrientes.......... (Quantidade absoluta e porcentagem em relacao a dieta)\n";

	// --- 1. MENU PRINCIPAL: ESCOLHA DO TIPO DE ANALISE ---
	while (true) {
		std::cout << "\nSelecione o tipo de analise:\n";
		std::cout << "1. Analisar por REFEICAO (exibe todos os nutrientes da refeicao escolhida)\n";
		std::cout << "2. Analisar por TIPO DE NUTRIENTE (exibe o total de nutrientes agrupados)\n";
		std::cout << "3. Voltar ao menu principal\n";
		std::cout << "Digite sua opcao (1-3): ";
		resposta_int_menu = ler_num_no_intervalo(1, 3);
		if (!resposta_int_menu)
			continue;
		break;
	}
	
	if (resposta_int_menu == 3) {
		std::cout << "Voltando ao menu principal\n";
		return;
	}
	
	// --- 2. SWITCH DETALHADO E MAPEAMENTO ---
	switch (resposta_int_menu) {
		
		case 1: // Analise por REFEIÇÃO
			while (true) {
				std::cout << "\nEscolha a Refeicao para verificar os nutrientes:\n";
				std::cout << "1. Todas as refeicoes\n";
				std::cout << "2. Cafe da manha\n";
				std::cout << "3. Almoco\n";
				std::cout << "4. Jantar\n";
				std::cout << "5. Lanche\n";
				std::cout << "Digite uma opcao (1-5): ";
				resposta_int_detalhe = ler_num_no_intervalo(1, 5);
				if (!resposta_int_detalhe)
					continue;
				break;
			}
            
            // Mapeia a escolha da refeição para a string correta
            arg_refeicao = map_refeicoes[resposta_int_detalhe];
            // O tipo de nutriente padrão é 'Todos'
            arg_tipo_nutriente = "Todos"; 
			break;
			
		case 2: // Analise por TIPO DE NUTRIENTE 
			while (true) {
				std::cout << "\nEscolha o Tipo de Nutriente para analise total:\n";
				std::cout << "1. Todos os nutrientes (Macro e Micro)\n";
				std::cout << "2. Apenas Macronutrientes\n";
				std::cout << "3. Apenas Micronutrientes\n";
				std::cout << "Digite uma opcao (1-3): ";
				resposta_int_detalhe = ler_num_no_intervalo(1, 3);
				if (!resposta_int_detalhe)
					continue;
				break;
			}
            
            // Mapeia a escolha do tipo de nutriente para a string correta
            arg_tipo_nutriente = map_tipos_nutrientes[resposta_int_detalhe];
            // A refeição padrão é 'Todas'
            arg_refeicao = "Todas"; 
			break;
	}
    
    // --- 3. BUSCANDO PELOS NUTRIENTES NO DIARIO ---
    
    // Confirmação da escolha antes de exibir
    std::cout << "\n--- Solicitando analise ---\n";
    std::cout << "Refeicao selecionada: " << arg_refeicao << std::endl;
    std::cout << "Tipo de Nutriente selecionado: " << arg_tipo_nutriente << std::endl;
    std::cout << "---------------------------\n";

    _diario.exibirNutrientesNoDiario(arg_refeicao, arg_tipo_nutriente);
	
	std::cout << "\nAnalise concluida.\n";
}

/*
void Usuario::criarReceita() {
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
		std::vector<std::string> medidas_alimento = buscarMedidas(_diario.buscarSubClasse(nome_ingrediente));
		int num_medidas = buscarNumMedidas(_diario.buscarSubClasse(nome_ingrediente));
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
*/
