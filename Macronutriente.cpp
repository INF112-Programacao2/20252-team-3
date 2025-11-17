#include "Macronutriente.h"
#include "PerfilNutricional.h" // Necessário para getMetaCalorica(), etc.
#include <stdexcept>
#include <iostream>

/**
 * @brief Construtor usa um switch para configurar o objeto (Opção A).
 * Chama o construtor da classe base (Nutriente) na lista de inicialização.
 */
Macronutriente::Macronutriente(TipoMacro tipo)
    : Nutriente(
        // 1. Define o nome (baseado no tipo)
        (tipo == TipoMacro::PROTEINA ? "Proteína" : 
        (tipo == TipoMacro::CARBOIDRATO ? "Carboidrato" : "Gordura")),
        // 2. Define a unidade (sempre "g")
        "g"
      ),
      // 3. Define os membros locais
      tipo(tipo),
      caloriasPorGrama(
        (tipo == TipoMacro::GORDURA ? 9.0 : 4.0)
      ) 
{
    // O corpo do construtor fica vazio,
    // pois a lista de inicialização fez todo o trabalho.
}

std::string Macronutriente::getTipo() const {
    return "Macronutriente";
}

double Macronutriente::getCaloriasPorGrama() const {
    return this->caloriasPorGrama;
}

/**
 * @brief Calcula e define a 'quantidadeIdeal' (Meta).
 * Esta é a lógica de negócio principal desta classe.
 */
void Macronutriente::calcularMetaIdeal(const PerfilNutricional& perfil) {
    try {
        double metaCaloricaTotal = perfil.getMetaCalorica();
        double metaEmGramas = 0.0;

        // Calcula a meta em gramas baseado no percentual definido no perfil
        switch (this->tipo) {
            case TipoMacro::PROTEINA:
                metaEmGramas = (metaCaloricaTotal * perfil.getPctProteina()) / this->caloriasPorGrama;
                break;
            case TipoMacro::CARBOIDRATO:
                metaEmGramas = (metaCaloricaTotal * perfil.getPctCarboidrato()) / this->caloriasPorGrama;
                break;
            case TipoMacro::GORDURA:
                metaEmGramas = (metaCaloricaTotal * perfil.getPctGordura()) / this->caloriasPorGrama;
                break;
        }
        
        // Define o atributo protegido da classe base
        this->quantidadeIdeal = metaEmGramas;

    } catch (const std::exception& e) {
        std::cerr << "Erro ao calcular meta de " << this->nome << ": " << e.what() << std::endl;
        this->quantidadeIdeal = 0.0; // Define 0 em caso de falha
    }
}