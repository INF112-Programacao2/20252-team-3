#include "macronutriente.hpp"
#include "perfil_nutricional.hpp" // Necessário para acessar getMetaCalorica()
#include <iostream>

Macronutriente::Macronutriente(TipoMacro tipo)
    : Nutriente(
        // Define o nome (String) baseado no Enum recebido
        (tipo == TipoMacro::PROTEINA ? "Proteínas" : 
        (tipo == TipoMacro::CARBOIDRATO ? "Carboidratos" : "Lipídios")), 
        "g" // Unidade sempre gramas
      ),
      tipo(tipo) 
{
    // Configura calorias automaticamente
    this->caloriasPorGrama = (tipo == TipoMacro::GORDURA) ? 9.0 : 4.0;
}

std::string Macronutriente::getTipo() const {
    return "Macronutriente";
}

double Macronutriente::getCaloriasPorGrama() const {
    return this->caloriasPorGrama;
}

void Macronutriente::calcularMetaIdeal(PerfilNutricional& perfil) {
    // Obtém a meta calórica total do usuário (único dado disponível no Perfil)
    double metaCaloricaTotal = perfil.getMetaCalorica(); 
    
    // Proteção contra divisão por zero ou valores inválidos
    if (metaCaloricaTotal <= 0) {
        this->quantidadeIdeal = 0;
        return;
    }

    // --- DEFINIÇÃO DE PERCENTUAIS PADRÃO ---
    // Como a classe PerfilNutricional não armazena a divisão da dieta (Low Carb, etc),
    // adotamos uma distribuição padrão balanceada para adultos saudáveis:
    // 50% Carbo, 20% Proteína, 30% Gordura.
    double pctAlvo = 0.0;

    switch (this->tipo) {
        case TipoMacro::PROTEINA:    pctAlvo = 0.20; break;
        case TipoMacro::CARBOIDRATO: pctAlvo = 0.50; break;
        case TipoMacro::GORDURA:     pctAlvo = 0.30; break;
    }

    // Fórmula: (Calorias Totais * Porcentagem) / Calorias por Grama
    this->quantidadeIdeal = (metaCaloricaTotal * pctAlvo) / this->caloriasPorGrama;
}
