#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

// Define o delimitador do CSV. Normalmente é vírgula (',')
const char DELIMITADOR = ','; 

std::string lerProximaCelula(std::stringstream& ss) {
    std::string celula;
    char c;

    // Tenta ignorar espaços iniciais (boa prática de CSV)
    while (ss.get(c) && isspace(c) && c != DELIMITADOR);

    // Se encontramos um caractere, voltamos a ele para re-processar
    if (ss.good()) {
        ss.unget();
    }

    // 1. Verifica se a célula começa com aspas
    if (ss.peek() == '"') {
        // Campo está entre aspas, consome aspas iniciais
        ss.get(); 
        
        // Lê até encontrar a aspa final
        while (ss.get(c)) {
            if (c == '"') {
                // Aspa encontrada. Verifica se é aspa de escape ("") ou aspa de fechamento.
                if (ss.peek() == '"') {
                    // É aspa dupla de escape (""). Adiciona uma aspa e consome a segunda aspa.
                    celula += c;
                    ss.get(); // Consome a segunda aspa (de escape)
                } else {
                    // É a aspa de fechamento do campo. Paramos a leitura aqui.
                    break;
                }
            } else {
                // Caractere normal, adiciona
                celula += c;
            }
        }
        
        // Após fechar aspas, ignora o restante até o delimitador ou fim da linha.
        while (ss.peek() != DELIMITADOR && ss.get(c) && !ss.eof());

    } else {
        // 2. Campo NÃO está entre aspas. Usa a leitura simples até o delimitador.
        std::getline(ss, celula, DELIMITADOR);
    }

    return celula;
}

// Funcao le o csv e o formata, deixando todas as celulas entre ""
void processarCSV(const std::string& nomeArquivoEntrada, const std::string& nomeArquivoSaida) {
    std::ifstream arquivoEntrada(nomeArquivoEntrada);
    std::ofstream arquivoSaida(nomeArquivoSaida);
    std::string linha;
    int contadorLinhas = 0;

    if (!arquivoEntrada.is_open() || !arquivoSaida.is_open()) {
        std::cerr << "ERRO ao abrir/criar arquivos." << std::endl;
        return;
    }

    // --- Leitura e Processamento Linha por Linha ---
    while (std::getline(arquivoEntrada, linha)) {
        contadorLinhas++;
        std::stringstream ssLinha(linha); 
        std::string celula;
        std::string linhaProcessada = "";
        bool primeiroCampo = true; 

        // Enquanto houver conteúdo na linha para ler...
        while (ssLinha.peek() != EOF) {
            
            // Lê a próxima célula usando a função robusta
            celula = lerProximaCelula(ssLinha);
            
            // Adiciona o delimitador
            if (!primeiroCampo) {
                linhaProcessada += DELIMITADOR;
            }
            primeiroCampo = false;

            // --- 4. Controle de Aspas (Aplicação) ---
            
            // Se a célula lida JÁ começou com aspas, a função lerProximaCelula removeu-as.
            // Para decidir se colocamos aspas, vamos olhar se o valor *originalmente*
            // continha vírgulas.
            if (celula.empty()) {
                // Células vazias
                linhaProcessada += "";
            } else {
                // Coloca o valor entre aspas, garantindo que seja um texto único no CSV de saída.
                // Se a célula lida tem aspas internas, elas já estão escapadas pela função de leitura.
                linhaProcessada += "\"" + celula + "\"";
            }
            
            // Consome o delimitador após o campo
            if (ssLinha.peek() == DELIMITADOR) {
                ssLinha.get();
            }
        }
        
        arquivoSaida << linhaProcessada << std::endl;
    }

    std::cout << "Processamento concluído!" << std::endl;
    std::cout << contadorLinhas << " linhas processadas." << std::endl;
    std::cout << "Resultado salvo em: " << nomeArquivoSaida << std::endl;

    arquivoEntrada.close();
    arquivoSaida.close();
}

// Exemplo de como usar a função
int main() {
    std::string nomeEntrada = "TabelaParaUsuario.csv"; 
    std::string nomeSaida = "TabelaFormatada.csv"; 
    
    processarCSV(nomeEntrada, nomeSaida);
    
    return 0;
}