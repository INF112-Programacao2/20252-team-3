#include "perfil_nutricional.hpp"

PerfilNutricional(std::string fator_fisico, double meta_calorica):
	_fatorFisico(fator_fisico), _metaCalorica(meta_calorica) {}
~PerfilNutricional();

std::string PerfilNutricional::getFatorFisico() {return _fatorFisico;}
double PerfilNutricional::getMetaCalorica() {return _metaCalorica;}

void PerfilNutricional::setFatorFisico(std::string fator_fisico) {_fatorFisico = fator_fisico;}
void PerfilNutricional::setMetaCalorica(double meta_calorica) {_metaCalorica = meta_calorica;}