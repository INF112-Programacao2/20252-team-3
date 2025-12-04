#ifndef MICRONUTRIENTE_HPP
#define MICRONUTRIENTE_HPP

#include "perfil_nutricional.hpp"
#include "nutriente.hpp"

#include <string>

class MicroNutriente : public Nutriente {
public:
    // Construtor: Chama o construtor da classe base (Nutriente)
    MicroNutriente(std::string nome, double valor);

    ~MicroNutriente();

    // --- Implementações dos Métodos Virtuais Puros ---

    // Implementa a lógica de cálculo de meta ideal para Micronutrientes.
    virtual void calcularMetaIdeal(PerfilNutricional& perfil) override;
    
};

#endif