#ifndef MACRONUTRIENTE_HPP
#define MACRONUTRIENTE_HPP

#include <string>

#include "perfil_nutricional.hpp"
#include "nutriente.hpp"


class MacroNutriente : public Nutriente {
public:
    MacroNutriente(std::string nome, double valor);
    
    ~MacroNutriente();

    // Implementa de cálculo de Calorias Diárias Totais 
    virtual void calcularMetaIdeal(PerfilNutricional& perfil) override;

    double getCaloriasPorGrama() const;
    double getPercentualMetaBase() const;
	
};

#endif