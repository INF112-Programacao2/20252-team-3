#include "alimentosliquidosOK.hpp"
#include <string>
#include <iostream>

AlimentosLiquidos::AlimentosLiquidos(std::string _nome, int _id):
Alimentos(_nome, _id){}

AlimentosLiquidos::~AlimentosLiquidos(){
    
}

void AlimentosLiquidos::lerUnidadeMedida(){
    int escolha;
    std::cout<< "Selecione um dos numeros abaixo para definir a unidade de medida do alimento." << std::endl;
    std::cout << "1-  Em mililitros." << std::endl;
    std::cout << "2 - Em litros." << std::endl;
    std::cout << "3 - Em xicaras." << std::endl;
    std::cout << "Digite o numero da sua escolha: ";
    std::cin >> escolha;

    if(escolha == 1){
        unidadeDeMedida = "ml";
    }
    else if(escolha == 2){
        unidadeDeMedida = "litro";
    }
    else if(escolha == 3){
        unidadeDeMedida ="xicara";
    }
     else{
        std::cout << "Escolha invalida. Assumindo mililitros (ml)." << std::endl;
        unidadeDeMedida = "ml";
     }
}

void AlimentosLiquidos::converterQuantidade(){
    double gramasML = 1.0;
    double gramasLitro = 1000.0;
    double gramasXicara = 240.0;

    if(unidadeDeMedida == "ml"){
        quantidadeGramas = quantidade *gramasML;
    }

    else if(unidadeDeMedida == "litro"){
        quantidadeGramas = quantidade * gramasLitro;
    }

    else if(unidadeDeMedida == "xicara"){
        quantidadeGramas = quantidade * gramasXicara;
    }
}