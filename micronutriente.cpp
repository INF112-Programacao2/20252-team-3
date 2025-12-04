#include "perfil_nutricional.hpp"
#include "micronutriente.hpp"

#include <iostream>
#include <utility>

MicroNutriente::MicroNutriente(std::string nome, double valor):
	Nutriente(nome, valor)
{
	// Setando a unidade de medida do nutriente
	_unidade = "mg";
    // Setando a subclasse do nutriente e a unidade
    _subClasse = "MicroNutriente";
}

MicroNutriente::~MicroNutriente() {}

void MicroNutriente::calcularMetaIdeal(PerfilNutricional& perfil) {
    // A meta é definida 100% aqui, usando os dados do Perfil.
    
    const std::string& nome = getNome();
    const std::string& sexo = perfil.getSexo();
    int idade = perfil.getIdade();
    
    if (nome == "Calcio") {
        if (idade >= 19 && idade <= 50) {
            _metaIdeal = 1000.0; // mg
        } else if (idade > 50 && sexo == "Feminino") {
            _metaIdeal = 1200.0; // mg
        } else {
            _metaIdeal = 1000.0;
        }
    } else if (nome == "Ferro") {
        if (sexo == "Feminino" && idade >= 19 && idade <= 50) {
            _metaIdeal = 18.0; // mg (Maior devido à perda menstrual)
        } else {
            _metaIdeal = 8.0; // mg (Meta para homens e mulheres pós-menopausa)
        }
    } else if (nome == "Magnesio") {
        if (sexo == "Masculino") {
            _metaIdeal = 420.0; // mg
        } else {
            _metaIdeal = 320.0; // mg
        }
    } else if (nome == "Sodio") {
        _metaIdeal = 1500.0; // mg (Ingestão Adequada, IA, para adultos)
    } else if (nome == "Potassio") {
        _metaIdeal = 4700.0; // mg (IA para adultos)
    } else if (nome == "VitaminaA") {
        if (sexo == "Masculino") {
            _metaIdeal = 900.0; // mcg
        } else {
            _metaIdeal = 700.0; // mcg
        }
    } else if (nome == "VitaminaB1") {
        if (sexo == "Masculino") {
            _metaIdeal = 1.2; // mg
        } else {
            _metaIdeal = 1.1; // mg
        }
    } else if (nome == "VitaminaC") {
        if (sexo == "Masculino") {
            _metaIdeal = 90.0; // mg
        } else {
            _metaIdeal = 75.0; // mg
        }
    } else {
        _metaIdeal = 0.0; // Valor padrão para não mapeados
    }
}
