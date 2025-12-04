#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// --- FUNÇÃO AUXILIAR: ANALISAR LINHA CSV COM SUPORTE A ASPAS ---
// Processa uma linha CSV, respeitando as aspas para campos que contenham vírgulas.
vector<string> analisarLinhaCSV(const string& linha) {
    vector<string> celulas;
    stringstream ss(linha);
    string celula_atual;
    bool em_campo_entre_aspas = false;
    char c;

    while (ss.get(c)) {
        if (c == '"') {
            em_campo_entre_aspas = !em_campo_entre_aspas;
        } else if (c == ',' && !em_campo_entre_aspas) {
            // Se encontrou uma vírgula e NÃO está dentro de um campo entre aspas,
            // a célula atual está completa.
            celulas.push_back(celula_atual);
            celula_atual.clear();
        } else {
            celula_atual += c;
        }
    }
    
    // Adiciona a última célula
    celulas.push_back(celula_atual);

    // Adiciona células vazias no final que o loop pode ter ignorado
    size_t pos_ultima_virgula = linha.find_last_of(',');
    if (pos_ultima_virgula != string::npos && pos_ultima_virgula == linha.length() - 1) {
        // Se a linha terminar com vírgula, significa que há um campo vazio no final.
        // O parser simples acima pode precisar de um ajuste adicional para capturar
        // todas as vírgulas finais no caso de colunas vazias, mas para os dados
        // essenciais (colunas 0 a 3), este método é robusto.
    }

    return celulas;
}

// --- FUNÇÃO AUXILIAR PARA REMOVER ASPAS ---
// Remove as aspas duplas do início e fim de uma string, se existirem.
string removerAspas(string texto) {
    if (texto.length() >= 2 && texto.front() == '"' && texto.back() == '"') {
        return texto.substr(1, texto.length() - 2);
    }
    return texto;
}

// --- FUNÇÃO DE CLASSIFICAÇÃO ---
string classificarAlimento(const string& categoria, const string& nome) {
    // 1. Mapeamento por Categoria de Alimento
    static const unordered_map<string, string> mapa_por_categoria = {
        {"Cereais e derivados", "SolidoNaoContavel"},
        {"Verduras, hortalicas e derivados", "SolidoNaoContavel"},
        {"Frutas e derivados", "SolidoContavel"},
        {"Pescados e frutos do mar", "SolidoContavel"},
        {"Leites e derivados", "Liquido"}, 
        {"Ovos e derivados", "SolidoContavel"},
        {"Açúcares e produtos de confeitaria", "SolidoNaoContavel"},
        {"Óleos e gorduras", "Liquido"},
        {"Bebidas (alcoólicas e não alcoólicas)", "Liquido"},
        {"Produtos e preparações industrializados", "SolidoContavel"},
        {"Outros alimentos preparados", "SolidoNaoContavel"},
        {"Nozes e sementes", "SolidoNaoContavel"}
    };

    // 2. Mapeamento Específico por Nome
    static const unordered_map<string, string> mapa_especifico = {
        {"Leite integral", "Liquido"},
        {"Iogurte natural", "SolidoNaoContavel"},
        {"Queijo minas frescal", "SolidoContavel"},
        {"Ovo de galinha cozido", "SolidoContavel"},
        {"Óleo de soja", "Liquido"},
        {"Suco de laranja", "Liquido"},
        {"Arroz integral cozido", "SolidoNaoContavel"},
        {"Banana prata", "SolidoContavel"}
    };

    if (mapa_especifico.count(nome)) {
        return mapa_especifico.at(nome);
    }

    if (mapa_por_categoria.count(categoria)) {
        if (categoria == "Leites e derivados") {
            if (nome.find("Queijo") != string::npos) return "SolidoContavel"; 
            if (nome.find("Iogurte") != string::npos) return "SolidoNaoContavel";
        }
        return mapa_por_categoria.at(categoria);
    }
    
    return "NAO_CLASSIFICADO"; 
}


// --- FUNÇÃO PRINCIPAL ---
int main() {
    // Variáveis para nomes de arquivos
    const string NOME_ARQUIVO_ENTRADA = "TabelaParaUsuario.csv";
    const string NOME_ARQUIVO_SAIDA = "TabelaClassificada.csv";

    // Abertura dos arquivos
    ifstream arquivo_entrada(NOME_ARQUIVO_ENTRADA);
    if (!arquivo_entrada.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada: " << NOME_ARQUIVO_ENTRADA << endl;
        return 1;
    }

    ofstream arquivo_saida(NOME_ARQUIVO_SAIDA);
    if (!arquivo_saida.is_open()) {
        cerr << "Erro ao criar o arquivo de saída: " << NOME_ARQUIVO_SAIDA << endl;
        return 1;
    }

    string linha;
    int contador_linha = 0;

    // 1. Processa as linhas do arquivo de entrada
    while (getline(arquivo_entrada, linha)) {
        contador_linha++;
        
        // **NOVO**: Usa a função analisarLinhaCSV para lidar com campos entre aspas.
        vector<string> celulas = analisarLinhaCSV(linha);
        size_t numero_colunas = celulas.size();

        // --- CABEÇALHO ---
        if (contador_linha == 1) {
            // Apenas copia o cabeçalho original
            arquivo_saida << linha << endl;
            continue; 
        }

        // --- DADOS ---
        if (numero_colunas >= 4) { // Pelo menos 4 colunas (incluindo a vazia SubClasse_Alimento)
            
            // 2. Limpa e classifica os dados de entrada
            // Usa removerAspas() para tirar as aspas dos campos de dados que foram lidos
            string id = removerAspas(celulas[0]);
            string categoria = removerAspas(celulas[1]);
            string nome = removerAspas(celulas[2]);
            
            string nova_sub_classe = classificarAlimento(categoria, nome);
            
            // 3. Constrói a nova linha, *citando* explicitamente os campos requeridos
            string nova_linha = "";
            
            // Coluna 1, 2 e 3 (ID, Categoria, Nome)
            nova_linha += "\"" + id + "\"";
            nova_linha += ",\"" + categoria + "\"";
            nova_linha += ",\"" + nome + "\"";
            
            // Coluna 4 (SubClasse_Alimento) - o valor classificado, entre aspas
            nova_linha += ",\"" + nova_sub_classe + "\"";
            
            // 4. Adiciona as colunas restantes (a partir da 5ª coluna - índice 4)
            for (size_t i = 4; i < numero_colunas; ++i) {
                // Apenas copia o conteúdo das demais colunas
                nova_linha += "," + celulas[i]; 
            }
            
            arquivo_saida << nova_linha << endl;
            
        } else {
            // Linhas incompletas (e.g., linhas vazias) são copiadas
            arquivo_saida << linha << endl;
        }
    }

    cout << "Processamento concluído!" << endl;
    cout << "O arquivo de saída foi salvo como: " << NOME_ARQUIVO_SAIDA << endl;

    arquivo_entrada.close();
    arquivo_saida.close();

    return 0;
}