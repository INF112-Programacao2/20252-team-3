#include <iostream>
#include <algorithm>
#include <cctype>    
#include <fstream>   
#include <iomanip>   
#include <stdexcept> 
#include <map>
#include <memory>

#include "perfil_nutricional.hpp"
#include "macronutriente.hpp" 
#include "micronutriente.hpp" 
#include "nutriente.hpp" 
#include "alimento.hpp"

// 1. Inicialização do Mapa Estático de Fatores de Atividade
const std::map<std::string, double> PerfilNutricional::FATORES_ATIVIDADE = {
    {"Sedentario", 1.2},
    {"Leve", 1.375},
    {"Moderado", 1.55},
    {"Ativo", 1.725},
    {"Muito Ativo", 1.9}
};

void PerfilNutricional::calcularCaloriasDiariasTotais() {
    // 1. Calcular a Taxa Metabólica Basal (TMB) 
    double tmb = 0.0;
    std::string sexoUpper = _sexo;
    
    // Converte o sexo para maiúsculas para comparação robusta
    std::transform(sexoUpper.begin(), sexoUpper.end(), sexoUpper.begin(),
                   [](unsigned char c){ return static_cast<unsigned char>(std::toupper(c)); });

    if (sexoUpper == "MASCULINO" || sexoUpper == "M") {
        tmb = (10 * _pesoKg) + (6.25 * _alturaCm) - (5 * _idade) + 5;
    } else if (sexoUpper == "FEMININO" || sexoUpper == "F") {
        tmb = (10 * _pesoKg) + (6.25 * _alturaCm) - (5 * _idade) - 161;
    } else {
        std::cerr << "Aviso: Sexo não reconhecido ('" << _sexo << "'). Usando cálculo TMB neutro." << std::endl;
        tmb = (10 * _pesoKg) + (6.25 * _alturaCm) - (5 * _idade); 
    }

    // 2. Aplicar o Fator de Atividade Física (FA)
    double fa = 1.0;
    try {
        fa = FATORES_ATIVIDADE.at(_nivelAtividade);
    } catch (const std::out_of_range& e) {
        std::cerr << "Aviso: Nível de atividade '" << _nivelAtividade << "' não reconhecido. Usando fator 1.2 (Sedentario)." << std::endl;
        fa = FATORES_ATIVIDADE.at("Sedentario");
    }

    // Necessidade Calórica Diária Total = TMB * FA
    _caloriasDiariasTotais = tmb * fa;
}

// Construtor: Inicializa membros e calcula a CDT
PerfilNutricional::PerfilNutricional() {}
PerfilNutricional::PerfilNutricional(std::string sexo, int idade, double pesoKg, double alturaCm, std::string nivelAtividade):
	_sexo(sexo), 
	_idade(idade), 
	_pesoKg(pesoKg), 
	_alturaCm(alturaCm), 
	_nivelAtividade(nivelAtividade),
	_caloriasDiariasTotais(0.0) 
{
    calcularCaloriasDiariasTotais();
}

// --- Getters ---
std::string PerfilNutricional::getSexo() const {return _sexo;}
int PerfilNutricional::getIdade() const {return _idade;}
double PerfilNutricional::getPesoKg() const {return _pesoKg;}
double PerfilNutricional::getAlturaCm() const {return _alturaCm;}
std::string PerfilNutricional::getNivelAtividade() const {return _nivelAtividade;}
double PerfilNutricional::getCaloriasDiariasTotais() const {return _caloriasDiariasTotais;}

// --- Setters (incluem validação básica e chamada para re-cálculo) ---
void PerfilNutricional::setSexo(const std::string& sexo) { 
    _sexo = sexo; 
    atualizarPerfil(); 
}
void PerfilNutricional::setIdade(int idade) { 
    if (idade > 0) { _idade = idade; atualizarPerfil(); } 
}
void PerfilNutricional::setPesoKg(double pesoKg) { 
    if (pesoKg > 0) { _pesoKg = pesoKg; atualizarPerfil(); } 
}
void PerfilNutricional::setAlturaCm(double alturaCm) { 
    if (alturaCm > 0) { _alturaCm = alturaCm; atualizarPerfil(); } 
}
void PerfilNutricional::setNivelAtividade(const std::string& nivelAtividade) { 
    _nivelAtividade = nivelAtividade; 
    atualizarPerfil(); 
}
void PerfilNutricional::setCaloriasDiariasTotais(double caloriasDiariasTotais) { 
    _caloriasDiariasTotais = caloriasDiariasTotais; 
}

// Método para re-calcular (chamado após qualquer setter que afete os calculos das metas)
void PerfilNutricional::atualizarPerfil() {
    calcularCaloriasDiariasTotais();
}

std::unique_ptr<Nutriente> PerfilNutricional::criarNutrienteSoma(const std::string& nome, double valor) const{
	 const std::vector<std::string> macronutrientes {
        "Energia ou Calorias",
        "Proteinas",
        "Lipidios",
        "Colesterol",
        "Carboidratos",
        "FibraAlimentar",
    };
    
    const std::vector<std::string> micronutrientes {
        "Calcio",
        "Magnesio",
        "Ferro",
        "Sodio",
        "Potassio",
        "VitaminaA",
		"VitaminB1",
        "VitaminaC"
    };
    
    if (std::find(macronutrientes.begin(), macronutrientes.end(), nome) != macronutrientes.end()) {
        // Cria um objeto MacroNutriente dinamicamente
        return std::make_unique<MacroNutriente>(nome, valor); 
    }
    else if (std::find(micronutrientes.begin(), micronutrientes.end(), nome) != micronutrientes.end()) {
        // Cria um objeto MicroNutriente dinamicamente
        return std::make_unique<MicroNutriente>(nome, valor); 
    }
    // Retorna um ponteiro nulo ou lança uma exceção se o nutriente for desconhecido
    return nullptr;
}

std::vector<std::unique_ptr<Nutriente>> PerfilNutricional::somarNutrientesAlimentos(const std::vector<std::unique_ptr<Alimento>>& alimentos) const { // Ajuste da assinatura
	
    std::map<std::string, std::unique_ptr<Nutriente>> somaPorNutriente;

    // Itera sobre os unique_ptr<Alimento>
    for (const auto& unique_alimento_ptr : alimentos) {
        
        // Verifica se o ponteiro é válido antes de desreferenciar
        if (unique_alimento_ptr == nullptr) continue;
		
        const auto& alimento = *unique_alimento_ptr; 
        const auto& nutrientesDoAlimento = alimento.getNutrientes();
		
        for (const auto& ponteiro_nutriente : nutrientesDoAlimento) {
            if (ponteiro_nutriente == nullptr) continue;
            
            std::string nome = ponteiro_nutriente->getNome();
            double valor = ponteiro_nutriente->getValor();
            
            if (somaPorNutriente.find(nome) == somaPorNutriente.end()) {
                // Cria um novo Nutriente para a soma (método auxiliar deve retornar unique_ptr)
                std::unique_ptr<Nutriente> novo_nutriente = criarNutrienteSoma(nome, valor);
                
                if (novo_nutriente) {
                    // Transfere a posse para o mapa
                    somaPorNutriente.emplace(nome, std::move(novo_nutriente)); 
                }
            } else {
                // Acumula o valor no Nutriente já existente no mapa
                double valorAtual = somaPorNutriente.at(nome)->getValor();
                somaPorNutriente.at(nome)->setValor(valorAtual + valor);
            }
        }
    }

    // 3. Converte o mapa em um vetor de unique_ptr's para retorno
    std::vector<std::unique_ptr<Nutriente>> resultado;
    
    for (auto& pair : somaPorNutriente) {
        resultado.push_back(std::move(pair.second)); 
    }
    
    return resultado;
}

void PerfilNutricional::gerarRelatorioDeConsumo(const std::vector<std::unique_ptr<Alimento>>& alimentosConsumidos) const {
	
	const std::string nomeArquivo = "relatorio_consumo.txt";

    // 1. CALCULA O CONSUMO TOTAL DE CADA NUTRIENTE USANDO A FUNÇÃO.
    std::vector<std::unique_ptr<Nutriente>> consumoTotalCalculado = somarNutrientesAlimentos(alimentosConsumidos);
    
    
    std::ofstream arquivo(nomeArquivo);

	try {
		if (!arquivo.is_open()) {
			throw std::runtime_error("Erro ao abrir o arquivo para escrita: " + nomeArquivo);
		}
	} catch (const std::runtime_error& e) {
		std::cerr << "ERRO FATAL: Falha na geração do relatório. " << e.what() << std::endl;
		return;
	}
    
    
    // --- Dados Nutricionais ---
    arquivo << "\n--- RELATORIO DO CONSUMO DIÁRIO ---\n";
    arquivo << std::left << std::setw(20) << "Nutriente"
            << std::right << std::setw(10) << "Meta"
            << std::right << std::setw(10) << "Consumo"
            << std::right << std::setw(10) << "Unidade" << std::endl;
    arquivo << "---------------------------------------------------" << std::endl;

    // 3. Itera sobre o vetor que contém os totais calculados.
    for (const auto& uniquePtrNutriente : consumoTotalCalculado) {	
    
        
        std::string nomeNutriente = uniquePtrNutriente->getNome();
        double consumoAcumulado = uniquePtrNutriente->getValor(); // O valor no objeto consolidado é a SOMA

        // Define a precisão
        // Usa o ponteiro para acessar getMetaIdeal()
        int precisao = (uniquePtrNutriente->getMetaIdeal() < 10) ? 2 : 1;
        
        arquivo << std::left << std::setw(20) << nomeNutriente
                << std::right << std::setw(10) << std::setprecision(precisao) << uniquePtrNutriente->getMetaIdeal()
                << std::right << std::setw(10) << std::setprecision(precisao) << consumoAcumulado
                << std::right << std::setw(10) << uniquePtrNutriente->getUnidade() << std::endl;
    }

    arquivo << "========================================" << std::endl;
    arquivo.close();
	
	std::cout << "Relatório de consumo gerado com sucesso!" << std::endl;
	
}