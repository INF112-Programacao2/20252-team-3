#include <iostream>
#include "alimentos.hpp"
#include <string>
#include <vector>
#include "macronutriente.hpp"
#include "micronutriente.hpp"

Alimentos::Alimentos(std::string _nome, int _id){
    nome = _nome;
    ID = _id;
    quantidade = 0.0;
    quantidadeGramas = 0.0;
    unidadeDeMedida = "";
}

Alimentos::~Alimentos(){
    for (Nutrientes* nutriente : nutrientes) {
        delete nutriente;
    }
}

void Alimentos::lerQuantidade(){
    double valor_quantidade;
    std::cout << "Quantidade de " << nome << " (em" << unidadeDeMedida << ") que voce consumiu: ";
    std::cin >> valor_quantidade;
    quantidade = valor_quantidade;
}

void Alimentos::adicionarNutriente(std::string tipo_nutriente, double quantidade_gramas){
    const std::vector<std::string> macros = {"Calorias", "Proteinas", "Carboidratos", "Proteinas", "Lipidios", "Colesterol", "FibraAlimentar"};
    const std::vector<std::string> micros = {"Calcio", "Magnesio", "Sodio", "Potassio", "Ferro", "VitaminaA", "VitaminaB1", "VitaminaC"};
    bool ehMacro = false;
    for(const auto& macro : macros){
        if(tipo_nutriente == macro){
            ehMacro = true;
            break;
        }
    }

    if(ehMacro){
        nutrientes.push_back(new MacroNutriente(tipo_nutriente, quantidade_gramas));
    }
    else{
        nutrientes.push_back(new MicroNutriente(tipo_nutriente, quantidade_gramas));
    }
}

