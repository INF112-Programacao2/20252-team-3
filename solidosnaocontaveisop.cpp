#include "solidosnaocontaveis.hpp"
#include <iostream>
#include <string>

SolidosNaoContaveis::SolidosNaoContaveis(std::string _nome, int _id, double g_xicara, double g_sopa):
Alimentos(_nome, _id){
    gramasXicara = g_xicara;
    gramasColherSopa = g_sopa;
}

SolidosNaoContaveis::~SolidosNaoContaveis(){
    
}

void SolidosNaoContaveis::lerUnidadeMedida(){
    int escolha;
    std::cout<< "Selecione um dos numeros abaixo para definir a unidade de medida do alimento." << std::endl;
    std::cout << "1 - Xicara." << std::endl;
    std::cout << "2 - Meia xicara." << std::endl;
    std::cout << "3 - Quarto de xicara." << std::endl;
    std::cout << "4 - Copo americano." << std::endl;
    std::cout << "5 - Colher de sopa." << std::endl;
    std::cout << "6 - Colher de sobremesa." << std::endl;
    std::cout << "7 - Colher de cha." << std::endl;
    std::cout << "8 - Gramas." << std::endl;
    std::cout << "Digite o numero da sua escolha: ";
    std::cin >> escolha;

    if(escolha == 1){
        unidadeDeMedida = "xicara";
    }
    else if(escolha == 2){
        unidadeDeMedida = "meia xicara";
    }
    else if(escolha == 3){
        unidadeDeMedida = "quarto de xicara";
    }
    else if(escolha == 4){
        unidadeDeMedida = "copo americano";
    }
    else if(escolha == 5){
        unidadeDeMedida = "colher de sopa";
    }
    else if(escolha == 6){
        unidadeDeMedida = "colher de sobremesa";
    }
    else if(escolha == 7){
        unidadeDeMedida = "colher de cha";
    }
    else if(escolha == 8){
        unidadeDeMedida = "gramas";
    }
    else {
        std::cout << "Escolha invalida. Assumindo gramas(g)." << std::endl;
        unidadeDeMedida = "gramas";
    }
}

void SolidosNaoContaveis::converterQuantidade(){
    if(unidadeDeMedida == "xicara"){
        quantidadeGramas = quantidade * gramasXicara;
    }
    else if(unidadeDeMedida == "meia xicara"){
        quantidadeGramas = quantidade * 0.5 * gramasXicara;
    }
    else if(unidadeDeMedida == "quarto de xicara"){
        quantidadeGramas = quantidade * 0.25 * gramasXicara;
    }
    else if(unidadeDeMedida == "copo americano"){
        quantidadeGramas = quantidade * 0.8 * gramasXicara; 
    }
    else if(unidadeDeMedida == "colher de sopa"){
        quantidadeGramas = quantidade * gramasColherSopa;
    }
    else if(unidadeDeMedida == "colher de sobremesa"){
        quantidadeGramas = quantidade * 0.66 * gramasColherSopa;
    }
    else if(unidadeDeMedida == "colher de cha"){
        quantidadeGramas = quantidade * 0.33 * gramasColherSopa;
    }
    else if(unidadeDeMedida == "gramas"){
        quantidadeGramas = quantidade;
    }
}