#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Função para formatar o valor da célula com aspas, se necessário.
// Ele só adiciona aspas se o valor não começar e terminar com elas.
string quote_cell(const string& cell_value) {
    if (cell_value.empty() || (cell_value.front() == '"' && cell_value.back() == '"')) {
        return cell_value;
    }
    return "\"" + cell_value + "\"";
}

int main() {
    const string input_filename = "TabelaParaUsuario.csv";
    const string temp_filename = "TabelaParaUsuario_temp.csv"; // Arquivo temporário para escrita

    // 1. Abrir o arquivo de entrada para leitura
    ifstream input_file(input_filename);
    if (!input_file.is_open()) {
        cerr << "Erro: Nao foi possivel abrir o arquivo de entrada: " << input_filename << endl;
        return 1;
    }

    // 2. Abrir um arquivo temporário para escrita
    ofstream temp_file(temp_filename);
    if (!temp_file.is_open()) {
        cerr << "Erro: Nao foi possivel criar o arquivo temporario: " << temp_filename << endl;
        input_file.close();
        return 1;
    }

    string line;
    
    // Processar o arquivo linha por linha
    while (getline(input_file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> cells;

        // 3. Dividir a linha em células usando a vírgula como delimitador
        // Usamos uma lógica simples de busca de vírgulas, pois não é um parser CSV completo
        size_t start = 0;
        size_t end = line.find(',');
        
        // Loop para extrair todas as colunas
        while (end != string::npos) {
            cells.push_back(line.substr(start, end - start));
            start = end + 1;
            end = line.find(',', start);
        }
        cells.push_back(line.substr(start, end)); // Adiciona a última coluna

        // 4. Reformular a linha
        string new_line = "";

        for (size_t i = 0; i < cells.size(); ++i) {
            // As 3 primeiras colunas (índices 0, 1, 2) recebem o tratamento de aspas
            if (i < 3) {
                new_line += quote_cell(cells[i]);
            } else {
                // As demais colunas são adicionadas como estão
                new_line += cells[i];
            }

            // Adicionar o delimitador (vírgula), exceto após a última coluna
            if (i < cells.size() - 1) {
                new_line += ",";
            }
        }

        // 5. Escrever a nova linha no arquivo temporário
        temp_file << new_line << endl;
    }

    // 6. Fechar arquivos
    input_file.close();
    temp_file.close();

    // 7. Substituir o arquivo original pelo temporário (renomear)
    if (remove(input_filename.c_str()) != 0) {
        cerr << "Erro: Nao foi possivel deletar o arquivo original." << endl;
        return 1;
    }
    if (rename(temp_filename.c_str(), input_filename.c_str()) != 0) {
        cerr << "Erro: Nao foi possivel renomear o arquivo temporario." << endl;
        return 1;
    }

    cout << "Sucesso! As 3 primeiras colunas de " << input_filename << " foram formatadas com aspas." << endl;

    return 0;
}