#include "perfil_nutricional.hpp"

std::string PerfilNutricional::getFatorFisico() {return _fatorFisico;}
double PerfilNutricional::getMetaCalorica() {return _metaCalorica;}

void PerfilNutricional::setFatorFisico(std::string fator_fisico) {_fatorFisico = fator_fisico;}
void PerfilNutricional::setMetaCalorias(double meta_calorias) {_metaCalorica = meta_calorica;}