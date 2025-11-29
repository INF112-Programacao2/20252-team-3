#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include "diario_alimentar.hpp"
#include "alimento.hpp"

int* DiarioAlimentar::buscarAlimentoPeloNome(std::string nome_alimento){
    std::ifstream arquivo("informacoes.csv");
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
    std::ifstream arquivo("informacoes.csv");
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

void DiarioAlimentar::adicionarAlimentoAoVetor(Alimento alimento){
    _vectorAlimentos.push_back(alimento);
}

void DiarioAlimentar::exibirAlimentos(){
    return;
}

std::string DiarioAlimentar::lerCampoCSV(std::stringstream& ss){
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
