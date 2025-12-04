#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>
#include <iomanip> 
#include <cstdio> 
#include <memory>
#include <map>


#include "diario_alimentar.hpp"
#include "alimento.hpp"
#include "nutriente.hpp"
#include "liquido.hpp"
#include "solido_contavel.hpp"
#include "solido_nao_contavel.hpp"


DiarioAlimentar::DiarioAlimentar() {}
DiarioAlimentar::~DiarioAlimentar() {}

std::vector<std::unique_ptr<Alimento>> DiarioAlimentar::getAlimentos() {
    std::vector<std::unique_ptr<Alimento>> vetorDeCopias; 

    for (const auto& ptr : _vectorAlimentosDoUsuario) {
        
        Alimento* alimento_base_ptr = ptr.get();
        std::string tipo = alimento_base_ptr->getSubClasse();
        
        // 1. SolidoContavel
        if (tipo == "SolidoContavel") {
            // Tenta converter para o ponteiro de subclasse
            SolidoContavel* sc_ptr = dynamic_cast<SolidoContavel*>(alimento_base_ptr);
            
            if (sc_ptr) {
                // Cria uma nova CÓPIA do objeto e o armazena em um unique_ptr
                vetorDeCopias.push_back(std::make_unique<SolidoContavel>(*sc_ptr));
            }
        } 
        // 2. SolidoNaoContavel
        else if (tipo == "SolidoNaoContavel") {
            SolidoNaoContavel* snc_ptr = dynamic_cast<SolidoNaoContavel*>(alimento_base_ptr);
            
            if (snc_ptr) {
                vetorDeCopias.push_back(std::make_unique<SolidoNaoContavel>(*snc_ptr));
            }
        } 
        // 3. Liquido
        else if (tipo == "Liquido") {
            Liquido* l_ptr = dynamic_cast<Liquido*>(alimento_base_ptr);
            
            if (l_ptr) {
                vetorDeCopias.push_back(std::make_unique<Liquido>(*l_ptr));
            }
        }
        else {
             // Caso o tipo não seja reconhecido
             throw std::runtime_error("Erro: Subclasse de Alimento desconhecida.");
        }
    }

    return vetorDeCopias;
}
int* DiarioAlimentar::buscarAlimentoPeloNome(std::string nome_alimento){
    std::ifstream arquivo("TabelaParaUsuario.csv");
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo CSV!" << std::endl;
        return nullptr;
    }

    std::string linha;
    std::vector<int> resultados;

    // Normaliza o nome para facilitar o match
    std::string nome_busca = nome_alimento;
    std::transform(nome_busca.begin(), nome_busca.end(),
                   nome_busca.begin(), ::tolower);

    // Descarta a primeira linha (cabeçalho)
    std::getline(arquivo, linha);

    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);

        std::string idStr, categoria, descricao;

        // Lê as 3 primeiras colunas
        idStr = lerCampoCSV(ss);
        categoria = lerCampoCSV(ss);
        descricao = lerCampoCSV(ss);

        // Transformar descrição em lowercase
        std::string descricao_lower = descricao;
        std::transform(descricao_lower.begin(), descricao_lower.end(),
                       descricao_lower.begin(), ::tolower);

        // Match parcial
        if (descricao_lower.find(nome_busca) != std::string::npos) {
            int id = std::stoi(idStr);
            resultados.push_back(id);
        }
    }

    arquivo.close();

    // Nada encontrado
    if (resultados.empty()) {
        return nullptr;
    }

    // Cria array dinâmico: posição 0 = quantidade de elementos
    int* vetor_resultado = new int[resultados.size() + 1];
    vetor_resultado[0] = resultados.size();

    for (size_t i = 0; i < resultados.size(); i++) {
        vetor_resultado[i + 1] = resultados[i];
    }

    return vetor_resultado;
}


std::string DiarioAlimentar::buscarAlimentoPeloID(int id){
    std::ifstream arquivo("TabelaParaUsuario.csv");
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo CSV!" << std::endl;
        return "";
    }

    std::string linha;

    // Pular cabeçalho
    std::getline(arquivo, linha);

    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);

        // Lê ID, categoria e descrição 
        std::string idStr = lerCampoCSV(ss);
        std::string categoria = lerCampoCSV(ss);
        std::string descricao = lerCampoCSV(ss);

        // Converter ID do CSV para inteiro
        if (std::stoi(idStr) == id) {
            return descricao;
        }
    }

    return "";
}

void DiarioAlimentar::adicionarAlimentoAoVetorDoUsuario(std::unique_ptr<Alimento> alimentoPtr) {
    _vectorAlimentosDoUsuario.push_back(std::move(alimentoPtr));
}

void DiarioAlimentar::exibirAlimentos(){
    return;
}

std::string DiarioAlimentar::lerCampoCSV(std::stringstream& ss) {
    std::string campo;

    // 1. O campo deve começar com aspas duplas (")
    if (ss.peek() == '"') {
        ss.get(); // Consome e remove a primeira aspa (")
        
        // 2. Lê o conteúdo do campo (campo) até encontrar a próxima aspa (")
        std::getline(ss, campo, '"'); 
        
        // 3. Consome a aspa final (")
        if (ss.peek() == '"') {
            ss.get(); 
        }
        
        // 4. Consome o delimitador (,) se ele existir (não existe no último campo)
        if (ss.peek() == ',') {
            ss.get(); 
        }
    } else {
        // A leitura padrão é até a vírgula.
        std::getline(ss, campo, ',');
        
        //  bloco 'else' só alcançado em caso de erro de formato.
    }

    return campo;
}


void DiarioAlimentar::preencherAlimentosNoVetor() {

	const std::string& nome_arquivo = "TabelaParaUsuario.csv";

	// 1. Configurar nomes de arquivos e abrir o arquivo de entrada
	std::ifstream arquivo_entrada(nome_arquivo);

	if (!arquivo_entrada.is_open()) {
		std::cerr << "Erro Fatal: Nao foi possivel abrir a tabela com os alimentos: " << nome_arquivo << std::endl;
		std::cerr << "Confira o nome do arquivo e se ele esta no mesmo diretorio dos executavel\n";
		exit(0);
	}

	// 2. Ler o cabeçalho
	std::string linha_cabecalho;
	std::getline(arquivo_entrada, linha_cabecalho);

	// 3. Ler o conteúdo restante do CSV e colocar os valores em _vectorAlimentos
	std::string linha;
	
	while (std::getline(arquivo_entrada, linha)) {
		if (linha.empty())
			continue;
		std::stringstream leitor_stream(linha);
		std::unique_ptr<Alimento> alimento_atual = nullptr;
		std::vector<std::string> linha_nutricional;
		std::vector<std::string> subclasses_alimento =
		{
			"SolidoContavel",
			"SolidoNaoContavel",
			"Liquido"
		};

		std::string id_string = lerCampoCSV(leitor_stream); // ID_Alimento (1ª coluna)
		std::string categoria = lerCampoCSV(leitor_stream); // Categoria_Alimento (2ª coluna)
		std::string nome = lerCampoCSV(leitor_stream); // Nome_Alimento (3ª coluna)
		std::string subclasse = lerCampoCSV(leitor_stream); // Subclasse_Alimento (4ª coluna)
		
		// Converter o tipo Alimento para a sua SubClasse
		if (std::find(subclasses_alimento.begin(), subclasses_alimento.end(), subclasse) != subclasses_alimento.end()) {
			if (subclasse == "SolidoContavel") {
				// Uso de make_unique (ou new/construtor direto)
				alimento_atual = std::make_unique<SolidoContavel>(std::stoi(id_string), categoria, nome, subclasse);
			}
			else if (subclasse == "SolidoNaoContavel") {
				alimento_atual = std::make_unique<SolidoNaoContavel>(std::stoi(id_string), categoria, nome, subclasse);
			}
			else if (subclasse == "Liquido") {
				alimento_atual = std::make_unique<Liquido>(std::stoi(id_string), categoria, nome, subclasse);
			}
		}
		else {
			std::cerr << "Erro fatal: Subclasse de Alimento nao reconhecida: " << subclasse << std::endl;
			exit(0);
		}

		// Nutrientes (resto das colunas)
		std::vector<std::string> tipos_nutrientes =
		{
		"Energia ou Calorias",
		"Proteinas",
		"Lipidios",
		"Colesterol",
		"Carboidratos",
		"FibraAlimentar",
		"Calcio",
		"Magnesio",
		"Ferro",
		"Sodio",
		"Potassio",
		"VitaminaA",
		"VitaminaB1",
		"VitaminaC"
		};
		for (std::string tipo_nutriente : tipos_nutrientes) {
			std::string valor_nutriente = lerCampoCSV(leitor_stream);

			double valor_convertido;
    
			// --- 1. Tratamento de valores não numéricos ('NA' e 'Traco') ---
			if (valor_nutriente == "NA" || valor_nutriente == "Traco" || valor_nutriente.empty()) {
				valor_convertido = 0.0; // Assume 0 para dados ausentes ou não rastreáveis
			} else {
				// --- 2. Conversão segura (após o tratamento de 'NA') ---
				try {
					valor_convertido = std::stod(valor_nutriente);
				} catch (const std::invalid_argument &e) {
					// Se falhar AGORA, o erro é um formato estranho (ex: "10,5" sem tratamento da vírgula)
					std::cerr << "ERRO FATAL: Falha na conversão de número. Detalhe: " << e.what() << std::endl;
					std::cerr << "Debug - valor_nutriente inesperado = " << valor_nutriente << std::endl;
					exit(0);
				} catch (const std::out_of_range& e) {
					// Se o número for muito grande ou pequeno para double
					std::cerr << "ERRO FATAL: Valor numérico fora do intervalo. Detalhe: " << e.what() << std::endl;
					exit(0);
				}
			}
    
			alimento_atual->adicionarNutriente(tipo_nutriente, valor_convertido);
		}

		// Transferência de posse do unique_ptr
		_vectorAlimentos.push_back(std::move(alimento_atual));
	}
	arquivo_entrada.close();
}

// Funcao adiciona um novo alimento ao csv, colocando-o junto aos alimentos da sua categoria,
// deixando o alimento em ordem alfabetica e atualizando o ID de todos os alimentos posteriores
void DiarioAlimentar::adicionarAlimentoAoDB(std::unique_ptr<Alimento> novo_alimento_ptr) {

    if (!novo_alimento_ptr) {
        std::cerr << "Erro: Ponteiro de novo alimento e nulo." << std::endl;
	return; // Retorna void em caso de erro
	}

	const Alimento& novo_alimento_ref = *novo_alimento_ptr;
	const std::string nome_alimento = novo_alimento_ref.getNome();
	
	const std::string nome_arquivo = "TabelaParaUsuario.csv";
	const std::string nome_arquivo_temp = "TabelaParaUsuario_temp.csv";

	// 1. Encontrar a categoria e a posição correta (ordem alfabetica) para inserir o alimento
	auto iterador_insercao = std::lower_bound(_vectorAlimentos.begin(), _vectorAlimentos.end(), novo_alimento_ptr,
		[](const std::unique_ptr<Alimento>& a_ptr, const std::unique_ptr<Alimento>& b_ptr) {
			
			const Alimento& a1 = *a_ptr;
			const Alimento& a2 = *b_ptr;
			
			if (a1.getCategoria() != a2.getCategoria()) {
				return a1.getCategoria() < a2.getCategoria();
			}
			
            std::string nome1 = a1.getNome();
            std::transform(nome1.begin(), nome1.end(), nome1.begin(), ::tolower);
            std::string nome2 = a2.getNome();
            std::transform(nome2.begin(), nome2.end(), nome2.begin(), ::tolower);
            
            return nome1 < nome2;
        });

    // 2. Inserção no Vetor Principal
    _vectorAlimentos.insert(iterador_insercao, std::move(novo_alimento_ptr));

    // 3. Obter o cabeçalho
    std::string linha_cabecalho = "";
    try {
        std::ifstream arquivo_cabecalho(nome_arquivo);
        if (arquivo_cabecalho.is_open()) {
            std::getline(arquivo_cabecalho, linha_cabecalho);
            arquivo_cabecalho.close();
        } else {
            throw std::runtime_error("Nao foi possivel ler o cabecalho do arquivo original.");
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro de leitura do cabecalho: " << e.what() << std::endl;
        return; // Retorna void em caso de erro
    }

    // 4. Reescrever o arquivo com IDs atualizados
    try {
        std::ofstream arquivo_temp(nome_arquivo_temp);
        if (!arquivo_temp.is_open()) {
            throw std::runtime_error("Falha ao abrir o arquivo temporario para escrita.");
        }

        arquivo_temp << linha_cabecalho << std::endl;
        
        int id_atual = 1;
        
        for (const auto& alimento_ptr : _vectorAlimentos) {
            
            Alimento& alimento = *alimento_ptr;
            
            alimento.setID(id_atual);
            id_atual++;
            
            arquivo_temp << "\"" << alimento.getID() << "\",";
            arquivo_temp << "\"" << alimento.getCategoria() << "\",";
            arquivo_temp << "\"" << alimento.getNome() << "\",";
            arquivo_temp << "\"" << alimento.getSubClasse() << "\"";
            
            const auto& nutrientes = alimento.getNutrientes();
			
            for (const auto& nutriente_ptr : nutrientes) {
                
                std::stringstream ss;
                ss << std::fixed << std::setprecision(1) << nutriente_ptr->getValor();
                
                arquivo_temp << "," << ss.str();
            }
            
            arquivo_temp << std::endl;
        }

        if (arquivo_temp.fail()) {
            throw std::runtime_error("Erro de escrita de dados no arquivo temporario (disco cheio?).");
        }
        
        arquivo_temp.close();

    } catch (const std::exception& e) {
        std::cerr << "Erro durante a reescrita do arquivo: " << e.what() << std::endl;
        std::remove(nome_arquivo_temp.c_str());
        return; // Retorna void em caso de erro
    }

    // 5. Substituir o arquivo original pelo temporário
    try {
        if (std::remove(nome_arquivo.c_str()) != 0) {
            throw std::runtime_error("Falha ao deletar o arquivo original (permissao ou inexistente).");
        }
        if (std::rename(nome_arquivo_temp.c_str(), nome_arquivo.c_str()) != 0) {
            throw std::runtime_error("Falha ao renomear o arquivo temporario. Renomeacao manual necessaria.");
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro durante a substituicao de arquivos: " << e.what() << std::endl;
        return; // Retorna void em caso de erro
    }

    std::cout << "Sucesso! Novo alimento '" << nome_alimento << "' adicionado e todos os IDs da tabela atualizados." << std::endl;
}

bool DiarioAlimentar::buscarAlimentoNoDiario(std::string nome_alimento) {
    for (const auto& alimento_ptr : _vectorAlimentosDoUsuario) {
        if (alimento_ptr->getNome() == nome_alimento) {
            return true;
        }
    }
    return false;
}

std::string DiarioAlimentar::getSubClasse(std::string nome_alimento) {
    for (const auto& alimento_ptr : _vectorAlimentos) {
        if (alimento_ptr->getNome() == nome_alimento) {
            return alimento_ptr->getSubClasse();
        }
    }
    return "";
}

void DiarioAlimentar::exibirInformacoes(std::string nome_alimento) {
	// Lista de tipos dos nutrientes para exibição formatada
    const std::vector<std::pair<std::string, std::string>> nutrientes_a_exibir = {
        {"Energia ou Calorias", "kcal"},
        {"Proteinas", "g"},
        {"Lipidios", "g"},
        {"Colesterol", "mg"},
        {"Carboidratos", "g"},
        {"FibraAlimentar", "g"},
        {"Calcio", "mg"},
        {"Magnesio", "mg"},
        {"Ferro", "mg"},
        {"Sodio", "mg"},
        {"Potassio", "mg"},
		{"VitaminaA", "mg"},
        {"VitaminaB1", "mg"},
        {"VitaminaC", "mg"}
    };
	
	std::cout << "\nInformacoes Nutricionais do Alimento\n";
	
	// Encontra o alimento
	for (const auto& alimento_ptr : _vectorAlimentosDoUsuario) {
        if (alimento_ptr->getNome() == nome_alimento) {
             // 1. Exibir Informações Básicas
            std::cout << "ID: " << alimento_ptr->getID() << std::endl;
            std::cout << "Nome: " << alimento_ptr->getNome() << std::endl;
            std::cout << "SubClasse: " << alimento_ptr->getSubClasse() << std::endl;
            std::cout << "Categoria: " << alimento_ptr->getCategoria() << std::endl;
            std::cout << "Refeicao: " << alimento_ptr->getRefeicao() << std::endl; // Assumindo método getRefeicao()
            std::cout << "Quantidade: " << alimento_ptr->getQuantidade() << " (em " << alimento_ptr->getUnidadeDeMedida() << ")\n"; 
            
            std::cout << "\nNutrientesn";

            // 2. Exibir Informações Nutricionais
            for (const auto& par : nutrientes_a_exibir) {
                const std::string& nome_nutriente = par.first;
                const std::string& unidade = par.second;
                
                // Formato de exibição: "Nome Nutriente (Unidade): Valor"
                // O método getNutriente(nome_nutriente) retorna um ponteiro para um objeto Nutriente 
                std::cout << nome_nutriente << " (" << unidade << "): " 
                          << alimento_ptr->getNutriente(nome_nutriente)->getValor() << std::endl;
            }

			return; // fim da funcao
        }
    }
}

void DiarioAlimentar::exibirNutrientesNoDiario(const std::string& refeicao_alvo, const std::string& tipo_nutriente_alvo) {
    
    // Lista  de Nutrientes 
    const std::vector<std::string> lista_tipos_nutrientes = 
	   {
		"Energia ou Calorias",
		"Proteinas",
		"Lipidios",
		"Colesterol",
		"Carboidratos",
		"FibraAlimentar",
		"Calcio",
		"Magnesio",
		"Ferro",
		"Sodio",
		"Potassio",
		"VitaminaA",
		"VitaminaB1",
		"VitaminaC"
		};

    // Mapa para acumular o total de cada nutriente.
    // Chave: Nome do Nutriente, Valor: {Total Acumulado, Unidade de Medida}
    std::map<std::string, std::pair<double, std::string>> totais_nutrientes;

    // 1. Iterar sobre todos os alimentos e acumular totais 
    for (const auto& alimento_ptr : _vectorAlimentosDoUsuario) {
        
        // 1.1. FILTRO POR REFEIÇÃO
        if (refeicao_alvo != "Todas" && alimento_ptr->getRefeicao() != refeicao_alvo) {
            continue; 
        }

        // 1.2. Iterar sobre a lista mestra para somar os nutrientes
        for (auto& tipo_buscado : lista_tipos_nutrientes) {

            // Obtém o ponteiro do nutriente específico, passando o tipo (que atua como nome/chave)
            Nutriente* nutriente_ptr = alimento_ptr->getNutriente(tipo_buscado);
            
            // Verifica se o nutriente existe no alimento
            if (nutriente_ptr == nullptr) {
                continue;
            }

            // 1.3. FILTRO POR TIPO DE NUTRIENTE (USANDO getSubClasse())
            const std::string sub_classe_nutriente = nutriente_ptr->getSubClasse();
            
            // Filtra por Macronutrientes
            if (tipo_nutriente_alvo == "Macronutrientes" && sub_classe_nutriente != "Macronutriente") {
                continue;
            }
            // Filtra por Micronutrientes
            if (tipo_nutriente_alvo == "Micronutrientes" && sub_classe_nutriente != "Micronutriente") {
                continue;
            }
            
            // 1.4. Acumular o valor
            // O nome do nutriente para exibição agora é obtido por getNome()
            const std::string nome_nutriente_exibicao = nutriente_ptr->getNome(); 
            double valor = nutriente_ptr->getValor();
            const std::string& unidade = nutriente_ptr->getUnidade();
            
            // Adiciona ou soma o valor no mapa, usando getNome() como chave
            if (totais_nutrientes.find(nome_nutriente_exibicao) == totais_nutrientes.end()) {
                // Primeira vez que o nutriente é encontrado
                totais_nutrientes[nome_nutriente_exibicao] = {valor, unidade};
            } else {
                // Já existe, apenas soma o valor
                totais_nutrientes[nome_nutriente_exibicao].first += valor;
            }
        }
    }

    // 2. Exibir os resultados 
    std::cout << "\n======================================================\n";
    std::cout << "  RELATÓRIO NUTRICIONAL\n";
    std::cout << "  Filtro de Refeição: " << refeicao_alvo << std::endl;
    std::cout << "  Filtro de Nutriente: " << tipo_nutriente_alvo << std::endl;
    std::cout << "======================================================\n";

    if (totais_nutrientes.empty()) {
        std::cout << "Nenhum dado nutricional encontrado com os filtros aplicados\n";
        std::cout << "------------------------------------------------------\n";
        return;
    }

    // Define precisão para duas casas decimais
    std::cout << std::fixed << std::setprecision(2);

    for (const auto& par : totais_nutrientes) {
        const std::string& nome = par.first;
        double total = par.second.first;
        const std::string& unidade = par.second.second;
        
        // Exibição formatada: Nome (Unidade): Valor Total
        std::cout << std::left << std::setw(25) << nome + " (" + unidade + "):" 
                  << std::right << std::setw(10) << total << std::endl;
    }

    std::cout << "======================================================\n";
}

void DiarioAlimentar::exibirAlimentosNoDiario(const std::string& refeicao_filtro, const std::string& categoria_filtro) {
    int contador_alimentos = 0;
	
    std::cout << "  \nALIMENTOS CADASTRADOS NO DIARIO\n";
    std::cout << "  Filtro de Refeição: " << refeicao_filtro << std::endl;
    std::cout << "  Filtro de Categoria: " << categoria_filtro << std::endl;

    // 1. Iterar sobre o vetor de alimentos
    for (const auto& alimento_ptr : _vectorAlimentosDoUsuario) {
        
        // 2. Aplicar filtro de Refeição
        // Se o filtro não for "Todas" E a refeição do alimento não for a desejada, pula o alimento.
        if (refeicao_filtro != "Todas" && alimento_ptr->getRefeicao() != refeicao_filtro) {
            continue; 
        }

        // 3. Aplicar filtro de Categoria
        // Se o filtro não for "Todas" E a categoria do alimento não for a desejada, pula o alimento.
        if (categoria_filtro != "Todas" && alimento_ptr->getCategoria() != categoria_filtro) {
            continue;
        }

        // 4. Se passou nos filtros, incrementa o contador e exibe o nome
        contador_alimentos++;
        
        // Exibição numerada: "1. Nome do Alimento"
        std::cout << std::setw(3) << std::left << contador_alimentos << ". " 
                  << alimento_ptr->getNome() << std::endl;
    }

    // 5. Exibir mensagem de finalização/ausência de resultados
    if (contador_alimentos == 0) {
        std::cout << "Nenhum alimento encontrado com os filtros aplicados\n";
    } else {
        std::cout << contador_alimentos << " alimentos exibidos\n";
    }
}

void DiarioAlimentar::exibirAlimentosNoDiario() {
    
    int contador_alimentos = 0;

    std::cout << "\n======================================================\n";
    std::cout << "  TODOS OS ALIMENTOS CADASTRADOS NO DIARIO\n";
    std::cout << "------------------------------------------------------\n";

    // 1. Iterar sobre o vetor de alimentos
    for (const auto& alimento_ptr : _vectorAlimentosDoUsuario) {
        
        contador_alimentos++;
        
        // Exibição numerada: "1. Nome do Alimento"
        std::cout << std::setw(3) << std::left << contador_alimentos << ". " 
                  << alimento_ptr->getNome() << std::endl;
    }

    std::cout << "------------------------------------------------------\n";
    if (contador_alimentos == 0) {
        std::cout << "Nenhum alimento encontrado no diario\n";
    } else {
        std::cout << contador_alimentos << " alimentos exibidos\n";
    }
    std::cout << "======================================================\n";
}

Alimento* DiarioAlimentar::getAlimento(const std::string& nome_alimento) const {
    
    // Itera sobre o vetor de unique_ptr
    for (const auto& alimento_ptr : _vectorAlimentos) {
        if (alimento_ptr->getNome() == nome_alimento) {
            // Retorna o ponteiro bruto para o objeto
            return alimento_ptr.get();
        }
    }
    
    // Retorna nullptr se o alimento não for encontrado
    return nullptr;
}

void DiarioAlimentar::removerAlimentoDoDiario(const std::string& nome_alimento) {
    
    // 1. Verificar a existência do alimento usando getAlimento
    if (getAlimento(nome_alimento) == nullptr) {
        std::cout << "Alimento \"" << nome_alimento << "\" nao encontrado no diario. Nenhuma remocao efetuada.\n";
        return; 
    }

    // 2. Usar std::remove_if para mover os elementos a serem removidos para o final
    auto novo_fim = std::remove_if(_vectorAlimentosDoUsuario.begin(), _vectorAlimentosDoUsuario.end(), 
        [&nome_alimento](const std::unique_ptr<Alimento>& alimento_ptr) {
            // Se o nome corresponder, retorna true (para ser movido para o final e apagado)
            return alimento_ptr->getNome() == nome_alimento;
        }
    );

    // 3. Apagar fisicamente os elementos.
    _vectorAlimentosDoUsuario.erase(novo_fim, _vectorAlimentosDoUsuario.end());

    std::cout << "Alimento \"" << nome_alimento << "\" removido com sucesso do diario.\n";
}