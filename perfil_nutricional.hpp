#ifndef PERFIL_NUTRCIONAL_HPP
#define PERFIL_NUTRCIONAL_HPP

#include "nutriente.hpp"
#include <string>

class PerfilNutricional {
private:
	double _metaCalorias;
	std::string _fatorFisico; 
	std:vector<Nutriente> _vectorMetaNutrientes;
public:
	PerfilNutricional(std::string fator_fisico, double meta_calorica);
	~PerfilNutricional();
	
	std::string getFatorFisico();
	double getMetaCalorica();
	
	void setFatorFisico(std::string fator_fisico);
	void setMetaCalorias(double meta_calorias);
	
	void calcularMetaCalorias(int idade, double peso, double altura, std::string fator_fisico);
	void calcularMetaNutrientes();
}


#endif