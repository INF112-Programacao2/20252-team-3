#include "Micronutriente.h"
#include "PerfilNutricional.h" // Necessário para getIdade(), getSexo()
#include <stdexcept>
#include <iostream>
#include <string>

// Construtor gigante que faz o "switch" da Opção A
// Define nome, unidade, categoria e limite (UL)
Micronutriente::Micronutriente(TipoMicro tipo) 
    : Nutriente(
        // 1. Define o NOME
        [](TipoMicro t) {
            switch (t) {
                case TipoMicro::VITAMINA_A: return "Vitamina A";
                case TipoMicro::B1_TIAMINA: return "B1 (Tiamina)";
                case TipoMicro::B2_RIBOFLAVINA: return "B2 (Riboflavina)";
                case TipoMicro::B3_NIACINA: return "B3 (Niacina)";
                case TipoMicro::B5_ACIDO_PANTOTENICO: return "B5 (Ácido Pantotênico)";
                case TipoMicro::B6_PIRIDOXINA: return "B6 (Piridoxina)";
                case TipoMicro::B7_BIOTINA: return "B7 (Biotina)";
                case TipoMicro::B9_FOLATO: return "B9 (Folato)";
                case TipoMicro::B12_COBALAMINA: return "B12 (Cobalamina)";
                case TipoMicro::VITAMINA_C: return "Vitamina C";
                case TipoMicro::VITAMINA_D: return "Vitamina D";
                case TipoMicro::VITAMINA_K: return "Vitamina K";
                case TipoMicro::CALCIO: return "Cálcio";
                case TipoMicro::FERRO: return "Ferro";
                case TipoMicro::MAGNESIO: return "Magnésio";
                case TipoMicro::ZINCO: return "Zinco";
                default: return "Desconhecido";
            }
        }(tipo),
        // 2. Define a UNIDADE (mg ou mcg)
        [](TipoMicro t) {
            switch (t) {
                case TipoMicro::VITAMINA_A: return "mcg";
                case TipoMicro::B7_BIOTINA: return "mcg";
                case TipoMicro::B9_FOLATO: return "mcg";
                case TipoMicro::B12_COBALAMINA: return "mcg";
                case TipoMicro::VITAMINA_D: return "mcg";
                case TipoMicro::VITAMINA_K: return "mcg";
                default: return "mg"; // A maioria é 'mg'
            }
        }(tipo)
    )
{
    // 3. Define os membros locais
    this->tipo = tipo;

    // Define o Limite Superior (UL) e a Categoria
    switch (tipo) {
        // (Valores de UL para Adultos, 0 = sem limite definido)
        case TipoMicro::VITAMINA_A: this->tipoCategoria = "Vitamina Lipossolúvel"; this->limiteSuperiorSeguro = 3000.0; break;
        case TipoMicro::B1_TIAMINA: this->tipoCategoria = "Vitamina Hidrossolúvel"; this->limiteSuperiorSeguro = 0; break;
        case TipoMicro::B2_RIBOFLAVINA: this->tipoCategoria = "Vitamina Hidrossolúvel"; this->limiteSuperiorSeguro = 0; break;
        case TipoMicro::B3_NIACINA: this->tipoCategoria = "Vitamina Hidrossolúvel"; this->limiteSuperiorSeguro = 35.0; break;
        case TipoMicro::B5_ACIDO_PANTOTENICO: this->tipoCategoria = "Vitamina Hidrossolúvel"; this->limiteSuperiorSeguro = 0; break;
        case TipoMicro::B6_PIRIDOXINA: this->tipoCategoria = "Vitamina Hidrossolúvel"; this->limiteSuperiorSeguro = 100.0; break;
        case TipoMicro::B7_BIOTINA: this->tipoCategoria = "Vitamina Hidrossolúvel"; this->limiteSuperiorSeguro = 0; break;
        case TipoMicro::B9_FOLATO: this->tipoCategoria = "Vitamina Hidrossolúvel"; this->limiteSuperiorSeguro = 1000.0; break;
        case TipoMicro::B12_COBALAMINA: this->tipoCategoria = "Vitamina Hidrossolúvel"; this->limiteSuperiorSeguro = 0; break;
        case TipoMicro::VITAMINA_C: this->tipoCategoria = "Vitamina Hidrossolúvel"; this->limiteSuperiorSeguro = 2000.0; break;
        case TipoMicro::VITAMINA_D: this->tipoCategoria = "Vitamina Lipossolúvel"; this->limiteSuperiorSeguro = 100.0; break;
        case TipoMicro::VITAMINA_K: this->tipoCategoria = "Vitamina Lipossolúvel"; this->limiteSuperiorSeguro = 0; break;
        
        case TipoMicro::CALCIO: this->tipoCategoria = "Mineral"; this->limiteSuperiorSeguro = 2500.0; break;
        case TipoMicro::FERRO: this->tipoCategoria = "Mineral"; this->limiteSuperiorSeguro = 45.0; break;
        case TipoMicro::MAGNESIO: this->tipoCategoria = "Mineral"; this->limiteSuperiorSeguro = 350.0; break; // (Apenas de suplementos)
        case TipoMicro::ZINCO: this->tipoCategoria = "Mineral"; this->limiteSuperiorSeguro = 40.0; break;
    }
}

std::string Micronutriente::getTipo() const {
    return "Micronutriente";
}

double Micronutriente::getLimiteSuperior() const {
    return this->limiteSuperiorSeguro;
}

std::string Micronutriente::getCategoria() const {
    return this->tipoCategoria;
}

/**
 * @brief Calcula e define a 'quantidadeIdeal' (Meta/RDA)
 * Lógica baseada em idade e sexo (RDAs para Adultos 19-50 anos).
 */
void Micronutriente::calcularMetaIdeal(const PerfilNutricional& perfil) {
    try {
        // Assume que PerfilNutricional encaminha as chamadas para Usuario
        int idade = perfil.getIdade(); 
        std::string sexo = perfil.getSexo(); // "M" ou "F"

        double metaRDA = 0.0;

        // Lógica de RDA simplificada (Adultos 19-50 anos)
        if (idade >= 19 && idade <= 50) {
            switch (this->tipo) {
                // Vitaminas
                case TipoMicro::VITAMINA_A: metaRDA = (sexo == "M" ? 900.0 : 700.0); break; // mcg
                case TipoMicro::B1_TIAMINA: metaRDA = (sexo == "M" ? 1.2 : 1.1); break;     // mg
                case TipoMicro::B2_RIBOFLAVINA: metaRDA = (sexo == "M" ? 1.3 : 1.1); break; // mg
                case TipoMicro::B3_NIACINA: metaRDA = (sexo == "M" ? 16.0 : 14.0); break;   // mg
                case TipoMicro::B5_ACIDO_PANTOTENICO: metaRDA = 5.0; break; // mg
                case TipoMicro::B6_PIRIDOXINA: metaRDA = 1.3; break;        // mg
                case TipoMicro::B7_BIOTINA: metaRDA = 30.0; break;         // mcg
                case TipoMicro::B9_FOLATO: metaRDA = 400.0; break;        // mcg
                case TipoMicro::B12_COBALAMINA: metaRDA = 2.4; break;       // mcg
                case TipoMicro::VITAMINA_C: metaRDA = (sexo == "M" ? 90.0 : 75.0); break;   // mg
                case TipoMicro::VITAMINA_D: metaRDA = 15.0; break;         // mcg
                case TipoMicro::VITAMINA_K: metaRDA = (sexo == "M" ? 120.0 : 90.0); break;  // mcg
                // Minerais
                case TipoMicro::CALCIO: metaRDA = 1000.0; break; // mg
                case TipoMicro::FERRO: metaRDA = (sexo == "M" ? 8.0 : 18.0); break; // mg
                case TipoMicro::MAGNESIO: metaRDA = (sexo == "M" ? 420.0 : 320.0); break; // mg
                case TipoMicro::ZINCO: metaRDA = (sexo == "M" ? 11.0 : 8.0); break;  // mg
            }
        } else {
            // Lógica para outras idades (crianças, idosos > 50) iria aqui.
            // Por enquanto, usamos a de 19-50 como padrão se for > 50.
            if (idade > 50) {
                // Exemplo de lógica para idosos:
                if (this->tipo == TipoMicro::CALCIO) {
                     metaRDA = (sexo == "M" ? 1000.0 : 1200.0);
                } else {
                    // Recai na lógica de 19-50 (simplificado)
                     metaRDA = calcularMetaIdeal(perfil); // Cuidado, pode ser recursivo se não tratar
                }
                //... (Implementação de RDA para idosos)
            }
             // Implementação de RDA para crianças < 19
        }
        
        // Define o atributo protegido da classe base
        this->quantidadeIdeal = metaRDA;

    } catch (const std::exception& e) {
        std::cerr << "Erro ao calcular meta de " << this->nome << ": " << e.what() << std::endl;
        this->quantidadeIdeal = 0.0;
    }
}
