#ifndef MACRONUTRIENTE_HPP
#define MACRONUTRIENTE_HPP

#include "nutriente.hpp"

// Enum class evita conflitos de nomes globais
enum class TipoMacro {
    PROTEINA,
    CARBOIDRATO,
    GORDURA
};

class Macronutriente : public Nutriente {
private:
    TipoMacro tipo;
    double caloriasPorGrama; // Proteína/Carbo = 4kcal, Gordura = 9kcal

public:
    // Construtor recebe apenas o tipo, o resto ele configura sozinho
    Macronutriente(TipoMacro tipo);
    virtual ~Macronutriente() = default;

    // Implementação da lógica de cálculo baseada em calorias
    void calcularMetaIdeal(PerfilNutricional& perfil) override;
    
    std::string getTipo() const override;
    double getCaloriasPorGrama() const;
};

#endif
