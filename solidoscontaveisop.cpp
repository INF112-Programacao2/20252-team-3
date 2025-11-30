#include "solidoscontaveis.hpp"
#include <iostream>
#include <string>

SolidosContaveis::SolidosContaveis(std::string _nome,int _id, double g_uni):
Alimentos(_nome, _id){
    gramasUnidade = g_uni;
}

SolidosContaveis::~SolidosContaveis(){

}

void SolidosContaveis::lerUnidadeMedida(){
    int escolha;
    std::cout<< "Selecione um dos numeros abaixo para definir a unidade de medida do alimento." << std::endl;
    std::cout << "1 - Unidades." << std::endl;
    std::cout << "2 - Gramas." << std::endl;
    std::cout << "3 - Meia duzia" << std::endl;
    std::cout << "4 - Duzia" << std::endl;
    std::cout << "Digite o numero da sua escolha: ";
    std::cin >> escolha;
    if(escolha == 1){
        unidadeDeMedida = "unidade";
    }
    else if (escolha == 2) { 
        unidadeDeMedida = "gramas";
    }
    else if(escolha == 3){
        unidadeDeMedida = "meia duzia";
    }
    else if(escolha == 4){
        unidadeDeMedida = "duzia";
    }
    else {
        std::cout << "Escolha invalida. Assumindo gramas(g)." << std::endl;
        unidadeDeMedida = "gramas";
    }
}

void SolidosContaveis::converterQuantidade(){
    if(unidadeDeMedida == "unidade"){
        quantidadeGramas = quantidade * gramasUnidade;
    }

    else if(unidadeDeMedida == "gramas"){
        quantidadeGramas = quantidade;
    }
    else if(unidadeDeMedida == "meia duzia"){
        quantidadeGramas = quantidade * 6.0 * gramasUnidade;
    }
    else if(unidadeDeMedida == "Meia Duzia"){
        quantidadeGramas = quantidade * 6.0 * gramasUnidade;
    }
}