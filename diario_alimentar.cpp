#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> 
#include <cstdio> 
#include <memory>


#include "diario_alimentar.hpp"
#include "alimento.hpp"

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

void DiarioAlimentar::adicionarAlimentoAoVetor(std::unique_ptr<Alimento> alimentoPtr) {
    _vectorAlimentos.push_back(std::move(alimentoPtr));
}

void DiarioAlimentar::exibirAlimentos(){
    return;
}

std::string lerCampoCSV(std::stringstream& ss){
    std::string campo;

    // Se começa com aspas, ler até aspas final
    if (ss.peek() == '"') {
        ss.get(); // remove a primeira aspa

        std::getline(ss, campo, '"'); // lê até a próxima aspa

        ss.get(); // consome a vírgula que vem depois da aspa final
    } else {
        std::getline(ss, campo, ','); // campo sem aspas
    }

    return campo;
}


void DiarioAlimentar::preencherAlimentosNoVetor() {
    
	const std::string& nome_arquivo = "TabelaParaUsuario.csv"
	
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
		Alimento* alimento_atual = nullptr;
		std::vector<std::string> linha_nutricional;

		std::string id_string = lerCampoCSV(leitor_stream); // ID_Alimento (1ª coluna)
		std::string categoria = lerCampoCSV(leitor_stream); // Categoria_Alimento (2ª coluna)
		std::string nome = lerCampoCSV(leitor_stream); // Nome_Alimento (3ª coluna)
		
		std::string subclasse = lerCampoCSV(leitor_stream); // Subclasse_Alimento (4ª coluna)
		// Converter o tipo Alimento para a sua SubClasse
		try {
			mudarTipo(alimento_atual, subclasse);
		} catch (const std::invalid_argument& e) {
			std::cerr << "ERRO FATAL AO CRIAR ALIMENTO: " << e.what() << std::endl;
			exit(0);
		}
		
		// Setando os parametros no alimento
		alimento_atual->setID(std::stoi(id_string));
		alimento_atual->setCategoria(categoria);
		alimento_atual->setNome(nome);
		alimento_atual->setSubClasse(subclasse);

		// Nutrientes (resto das colunas)
		std::vector<std::string> tipos_nutrientes =
		{
		"Energia ou Calorias (kcal)",
		"Proteina (g)",
		"Lipidios (g)",
		"Colesterol (mg)",
		"Carboidrato (g)",
		"FibraAlimentar (g)",
		"Calcio (mg)",
		"Magnesio (mg)",
		"Ferro (mg)",
		"Sodio (mg)",
		"Potassio (mg)",
		"Tiamina (mg)",
		"VitaminaC (mg)"
		};
		for (std::string tipo_nutriente : tipos_nutrientes) {
			std::string valor_nutriente = lerCampoCSV(leitor_stream);
			try {
				if (!(alimento_atual->adicionarNutriente(tipo_nutriente, valor_nutriente)))
					throw std::invalid_argument("Nao foi possivel ler os nutrientes da tabela");
			} catch (std::invalid_argument &e) {
				std::cerr << "ERRO FATAL: " << e.what() << std::endl;
				exit(0);
			}
		}

		_vectorAlimentos.push_back(std::unique_ptr<Alimento>(alimento_atual)); // conversao do ponteiro bruto para ponteiro inteligente
    }
    arquivo_entrada.close();
}

// Funcao adiciona um novo alimento ao csv, colocando-o junto aos alimentos da sua categoria,
// deixando o alimento em ordem alfabetica e atualizando o ID de todos os alimentos posteriores
bool DiarioAlimentar::adicionarAlimentoAoDB(Alimento novo_alimento) {
    // Definindo nomes de arquivos
    const std::string nome_arquivo = "TabelaParaUsuario.csv"; // Ajuste conforme a definição real
    const std::string nome_arquivo_temp = "TabelaParaUsuario_temp.csv";

    // 1. Encontrar a categoria e a posição correta (ordem alfabetica) para inserir o alimento
    
    // Encontra o iterador para o primeiro alimento que deve vir DEPOIS
    auto iterador_insercao = std::lower_bound(_vectorAlimentos.begin(), _vectorAlimentos.end(), novo_alimento,
        [](const Alimento& a1, const Alimento& a2) {
            // Logica principal: Ordenar por Categoria, depois por Nome (USANDO GETTER)
            if (a1.getCategoria() != a2.getCategoria()) {
                return a1.getCategoria() < a2.getCategoria();
            }
            
            // Se as categorias sao iguais, ordena por nome (case-insensitive)
            std::string nome1 = a1.getNome();
            std::transform(nome1.begin(), nome1.end(), nome1.begin(), ::tolower);
            std::string nome2 = a2.getNome();
            std::transform(nome2.begin(), nome2.end(), nome2.begin(), ::tolower);
            
            return nome1 < nome2;
        });

    // 2. Inserção no Vetor Principal
    
    // O índice de inserção (apenas para referência, pois não há vetor paralelo a ser sincronizado)
    size_t indice_insercao = std::distance(_vectorAlimentos.begin(), iterador_insercao);
    // Atualiza o _vectorAlimentos com o novo alimento
    _vectorAlimentos.insert(iterador_insercao, novo_alimento);

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
        return false;
    }

    // 4. Reescrever o arquivo com IDs atualizados
    try {
        std::ofstream arquivo_temp(nome_arquivo_temp);
        if (!arquivo_temp.is_open()) {
            throw std::runtime_error("Falha ao abrir o arquivo temporario para escrita.");
        }

        // Escreve o cabecalho
        arquivo_temp << linha_cabecalho << std::endl;
        
        int id_atual = 1;
        
        for (size_t i = 0; i < _vectorAlimentos.size(); ++i) {
			Alimento& alimento = *(_vectorAlimentos[i]);
            
            // Atualiza o ID do alimento no vetor
            alimento.setID(id_atual);
            id_atual++;
			
            // Escrevendo as 3 primeiras colunas com aspas (ID, Categoria, Nome)
            arquivo_temp << "\"" << alimento.getID() << "\",";
            arquivo_temp << "\"" << alimento.getCategoria() << "\",";
            arquivo_temp << "\"" << alimento.getNome() << "\"";
			arquivo_temp << "\"" << alimento.getSubClasse() << "\"";
            
            // Acessa o vetor de unique_ptr de Nutrientes
            const auto& nutrientes = alimento.getNutrientes();
            
            // Iteração sobre os ponteiros inteligentes
            for (const auto& nutriente_ptr : nutrientes) {
                
                // Formatação do valor (double) para string
                std::stringstream ss;
                // Configurado para: 1 casa decimal (setprecision(1)) e formato fixo
                ss << std::fixed << std::setprecision(1) << nutriente_ptr->getValor(); 
                
                // Adiciona a vírgula e o valor formatado
                arquivo_temp << "," << ss.str();
            }
            
            arquivo_temp << std::endl;
        }

        // Verifica se houve falha de escrita
        if (arquivo_temp.fail()) {
            throw std::runtime_error("Erro de escrita de dados no arquivo temporario (disco cheio?).");
        }
        
        arquivo_temp.close();

    } catch (const std::exception& e) {
        std::cerr << "Erro durante a reescrita do arquivo: " << e.what() << std::endl;
        // Tenta remover o arquivo temp para limpeza, se ele tiver sido criado
        std::remove(nome_arquivo_temp.c_str());
        return false;
    }

    // 5. Substituir o arquivo original pelo temporário (I/O com tratamento de exceção)
    try {
        if (std::remove(nome_arquivo.c_str()) != 0) {
            // remove() falhou
            throw std::runtime_error("Falha ao deletar o arquivo original (permissao ou inexistente).");
        }
        if (std::rename(nome_arquivo_temp.c_str(), nome_arquivo.c_str()) != 0) {
            // rename() falhou
            throw std::runtime_error("Falha ao renomear o arquivo temporario. Renomeacao manual necessaria.");
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro durante a substituicao de arquivos: " << e.what() << std::endl;
        return false;
    }

    std::cout << "Sucesso! Novo alimento '" << novo_alimento.getNome() << " adicionado e todos os IDs da tabela atualizados." << std::endl;
           
    return true;
}

bool DiarioAlimentar::buscarAlimentoNoDiario(std::string nome_alimento) {
    for (const auto& alimento_ptr : _vectorAlimentos) {
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
        {"Energia", "kcal"},
        {"Proteina", "g"},
        {"Lipidios", "g"},
        {"Colesterol", "mg"},
        {"Carboidrato", "g"},
        {"FibraAlimentar", "g"},
        {"Calcio", "mg"},
        {"Magnesio", "mg"},
        {"Ferro", "mg"},
        {"Sodio", "mg"},
        {"Potassio", "mg"},
        {"Tiamina", "mg"},
        {"VitaminaC", "mg"}
    };
	
	std::cout << "\n--- Informacoes Nutricionais do Alimento ---\n";
	
	// Encontra o alimento
	for (const auto& alimento_ptr : _vectorAlimentos) {
        if (alimento_ptr->getNome() == nome_alimento) {
             // 1. Exibir Informações Básicas
            std::cout << "ID: " << alimento_ptr->getID() << std::endl;
            std::cout << "Nome: " << alimento_ptr->getNome() << std::endl;
            std::cout << "SubClasse: " << alimento_ptr->getSubClasse() << std::endl;
            std::cout << "Categoria: " << alimento_ptr->getCategoria() << std::endl;
            std::cout << "Refeicao: " << alimento_ptr->getRefeicao() << std::endl; // Assumindo método getRefeicao()
            std::cout << "Quantidade: " << alimento_ptr->getQuantidade() 
                      << " " << alimento_ptr->getUnidadeMedida() << std::endl; // Adicionando Unidade e Quantidade
            
            std::cout << "\n--- Nutrientes por Porcao ---\n";

            // 2. Exibir Informações Nutricionais
            for (const auto& par : nutrientes_a_exibir) {
                const std::string& nome_nutriente = par.first;
                const std::string& unidade = par.second;
                
                // Formato de exibição: "Nome Nutriente (Unidade): Valor"
                // O método getNutriente(nome_nutriente) retorna o valor.
                std::cout << nome_nutriente << " (" << unidade << "): " 
                          << alimento_ptr->getNutriente(nome_nutriente) << std::endl;
            }

            std::cout << "-------------------------------------------\n";
			return; // fim da funcao
        }
    }
}