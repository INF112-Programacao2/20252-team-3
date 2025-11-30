#ifndef MACRONUTRIENTE_H
#define MACRONUTRIENTE_H

#include "Nutriente.h"

// Enumeração de tipo "classe" (modern C++) para os macros.
// Este é o "número inteiro" que você mencionou.
enum class TipoMacro {
    PROTEINA,
    CARBOIDRATO,
    GORDURA
};

/**
 * @class Macronutriente
 * @brief Implementação de Nutriente para Macros.
 * Calcula a meta (quantidadeIdeal) com base nas calorias e percentuais
 * lidos do PerfilNutricional.
 */
class Macronutriente : public Nutriente {
private:
    TipoMacro tipo;
    double caloriasPorGrama;

public:
    /**
     * @brief Construtor que define o tipo de macro.
     * Este construtor faz o trabalho da "Opção A".
     * @param tipo O tipo de macro (ex: TipoMacro::PROTEINA).
     */
    Macronutriente(TipoMacro tipo);

    // --- Sobrescrita (Override) dos Métodos Virtuais ---

    /**
     * @brief Calcula a meta em gramas.
     * Lê perfil.getMetaCalorica() e perfil.getPctProteina() etc.
     */
    void calcularMetaIdeal(const PerfilNutricional& perfil) override;

    /**
     * @brief Retorna "Macronutriente".
     */
    std::string getTipo() const override;

    // --- Métodos Específicos ---
    double getCaloriasPorGrama() const;
};

#endif // MACRONUTRIENTE_H
