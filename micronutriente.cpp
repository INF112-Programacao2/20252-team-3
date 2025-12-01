#include "micronutriente.hpp"
#include "perfil_nutricional.hpp"
#include <iostream>

Micronutriente::Micronutriente(TipoMicro tipo) 
    : Nutriente("", "") // Inicializa vazio, o corpo do construtor preenche os dados
{
    this->tipo = tipo;
    
    // Configuração automática de Nome, Unidade e Categoria baseada no Tipo
    switch (tipo) {
        case TipoMicro::CALCIO: 
            this->nome = "Cálcio"; this->unidadeDeMedida = "mg"; 
            this->categoria = "Mineral"; this->limiteSuperior = 2500; break;
        case TipoMicro::FERRO: 
            this->nome = "Ferro"; this->unidadeDeMedida = "mg"; 
            this->categoria = "Mineral"; this->limiteSuperior = 45; break;
        case TipoMicro::MAGNESIO: 
            this->nome = "Magnésio"; this->unidadeDeMedida = "mg"; 
            this->categoria = "Mineral"; this->limiteSuperior = 350; break;
        case TipoMicro::SODIO: 
            this->nome = "Sódio"; this->unidadeDeMedida = "mg"; 
            this->categoria = "Mineral"; this->limiteSuperior = 2300; break;
        case TipoMicro::POTASSIO: 
            this->nome = "Potássio"; this->unidadeDeMedida = "mg"; 
            this->categoria = "Mineral"; this->limiteSuperior = 0; break; // Sem UL definido
        case TipoMicro::VITAMINA_A: 
            this->nome = "Vitamina A"; this->unidadeDeMedida = "mcg"; 
            this->categoria = "Vitamina"; this->limiteSuperior = 3000; break;
        case TipoMicro::VITAMINA_B1: 
            this->nome = "Vitamina B1"; this->unidadeDeMedida = "mg"; 
            this->categoria = "Vitamina"; this->limiteSuperior = 0; break;
        case TipoMicro::VITAMINA_C: 
            this->nome = "Vitamina C"; this->unidadeDeMedida = "mg"; 
            this->categoria = "Vitamina"; this->limiteSuperior = 2000; break;
        default:
            this->nome = "Desconhecido"; this->unidadeDeMedida = "-";
    }
}

std::string Micronutriente::getTipo() const {
    return "Micronutriente";
}

double Micronutriente::getLimiteSuperior() const {
    return this->limiteSuperior;
}

void Micronutriente::calcularMetaIdeal(PerfilNutricional& perfil) {
    // NOTA: Como a classe Usuario/Perfil atual não fornece Sexo ou Idade,
    // utilizamos valores de referência (RDA) médios para adultos.
    // Isso permite que o sistema funcione sem quebrar a compilação.
    
    switch (this->tipo) {
        case TipoMicro::CALCIO:      this->quantidadeIdeal = 1000.0; break; // Padrão adulto
        case TipoMicro::FERRO:       this->quantidadeIdeal = 14.0; break;   // Média Homem(8)/Mulher(18)
        case TipoMicro::MAGNESIO:    this->quantidadeIdeal = 360.0; break;  // Média H/M
        case TipoMicro::SODIO:       this->quantidadeIdeal = 1500.0; break; // Recomendação padrão
        case TipoMicro::POTASSIO:    this->quantidadeIdeal = 4700.0; break;
        case TipoMicro::VITAMINA_A:  this->quantidadeIdeal = 800.0; break;  // Média H/M
        case TipoMicro::VITAMINA_B1: this->quantidadeIdeal = 1.15; break;
        case TipoMicro::VITAMINA_C:  this->quantidadeIdeal = 82.5; break;   // Média H(90)/M(75)
    }
}
