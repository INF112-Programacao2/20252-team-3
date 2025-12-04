#include "perfil_nutricional.hpp"
#include "macronutriente.hpp"

#include <iostream>
#include <utility>

MacroNutriente::MacroNutriente(std::string nome, double valor): 
	Nutriente(nome, valor)
{
	// Setando a unidade de medida do nutriente
	if (nome ==  "Energia ou Calorias")
		_unidade = "kcal";
	else if (nome ==  "Colesterol")
		_unidade = "mg";
	else
		_unidade = "g";
    // Setando a subclasse do nutriente
    _subClasse = "MacroNutriente";
}

MacroNutriente::~MacroNutriente() {}


void MacroNutriente::calcularMetaIdeal(PerfilNutricional& perfil) {
    const std::string& nome = getNome();
    
    if (nome == "FibraAlimentar") {
        // AI para adultos (AI =  Ingestão Adequada)
        _metaIdeal = 25.0; // Gramas por dia 
        return; 
    } else if (nome == "Colesterol") {
        // Meta de Colesterol (limite máximo)
        _metaIdeal = 300.0; 
        return;
    } else if (nome == "Energia ou Calorias") {
        _metaIdeal = perfil.getCaloriasDiariasTotais(); // kcal
        return;
    }

    // --- Valores Baseados em Percentual Calórico ---
    
    double caloriasTotais = perfil.getCaloriasDiariasTotais(); 
    double percentualAlvo = 0.0;
    double caloriasPorG = 0.0;

    if (nome == "Proteinas") {
        percentualAlvo = 25.0; 
        caloriasPorG = 4.0;
        // Exemplo de ajuste: aumento para pessoas muito ativas
        if (perfil.getNivelAtividade() == "Ativo" || perfil.getNivelAtividade() == "Muito Ativo") {
             percentualAlvo = 30.0; 
        }
    } else if (nome == "Carboidratos") {
        percentualAlvo = 50.0; 
        caloriasPorG = 4.0;
    } else if (nome == "Lipidios") {
        percentualAlvo = 25.0; 
        caloriasPorG = 9.0;
    }

    // Cálculo e Conversão para Gramas
    if (percentualAlvo > 0.0 && caloriasPorG > 0.0) {
        double caloriasDoMacro = caloriasTotais * (percentualAlvo / 100.0);
        _metaIdeal = caloriasDoMacro / caloriasPorG;
    } else {
        _metaIdeal = 0.0;
    }
}