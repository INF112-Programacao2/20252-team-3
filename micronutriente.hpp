#ifndef MICRONUTRIENTE_H
#define MICRONUTRIENTE_H

#include "Nutriente.h"

// Enumeração corrigida para todos os Micronutrientes
enum class TipoMicro {
    // Vitaminas
    VITAMINA_A,
    B1_TIAMINA,
    B2_RIBOFLAVINA,
    B3_NIACINA,
    B5_ACIDO_PANTOTENICO,
    B6_PIRIDOXINA,
    B7_BIOTINA,
    B9_FOLATO,
    B12_COBALAMINA,
    VITAMINA_C,
    VITAMINA_D,
    VITAMINA_K,
    // Minerais
    CALCIO,
    FERRO,
    MAGNESIO,
    ZINCO
};

/**
 * @class Micronutriente
 * @brief Implementação de Nutriente para Micros.
 * Calcula a meta (quantidadeIdeal) com base na RDA (Idade/Sexo)
 * lida do PerfilNutricional.
 */
class Micronutriente : public Nutriente {
private:
    TipoMicro tipo;
    std::string tipoCategoria; // Ex: "Vitamina Hidrossolúvel"
    double limiteSuperiorSeguro; // UL (Tolerable Upper Intake Level)

public:
    /**
     * @brief Construtor que define o tipo de micro (Opção A).
     * @param tipo O tipo de micro (ex: TipoMicro::VITAMINA_A).
     */
    Micronutriente(TipoMicro tipo);

    // --- Sobrescrita (Override) dos Métodos Virtuais ---

    /**
     * @brief Calcula a meta (RDA) em mg/mcg.
     * Lê perfil.getIdade() e perfil.getSexo().
     */
    void calcularMetaIdeal(const PerfilNutricional& perfil) override;

    /**
     * @brief Retorna "Micronutriente".
     */
    std::string getTipo() const override;

    // --- Métodos Específicos ---
    double getLimiteSuperior() const;
    std::string getCategoria() const;
};

#endif // MICRONUTRIENTE_H
