#ifndef MICRONUTRIENTE_HPP
#define MICRONUTRIENTE_HPP

#include "nutriente.hpp"

// Enum com os principais micros que constam na tabela CSV
enum class TipoMicro {
    CALCIO, FERRO, MAGNESIO, SODIO, POTASSIO, // Minerais
    VITAMINA_A, VITAMINA_B1, VITAMINA_C       // Vitaminas
};

class Micronutriente : public Nutriente {
private:
    TipoMicro tipo;
    std::string categoria; // "Mineral" ou "Vitamina"
    double limiteSuperior; // Tolerable Upper Intake Level (UL)

public:
    Micronutriente(TipoMicro tipo);
    virtual ~Micronutriente() = default;

    // Implementação da lógica de cálculo baseada em RDA (Médias Gerais)
    void calcularMetaIdeal(PerfilNutricional& perfil) override;
    
    std::string getTipo() const override;
    double getLimiteSuperior() const;
};

#endif
