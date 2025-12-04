#include "perfil_nutricional.hpp"
#include "nutriente.hpp"

#include <iostream>
#include <utility>

// Construtor inicializa os membros. Metas e consumo começam em zero.
Nutriente::Nutriente(std::string nome, double valor):
	_nome(nome),
	_valor(valor)
{}

std::string Nutriente::getNome() const {
    return _nome;
}

std::string Nutriente::getUnidade() const {
    return _unidade;
}

double Nutriente::getMetaIdeal() const {
    return _metaIdeal;
}

double Nutriente::getValor() const {
    return _valor;
}
std::string Nutriente::getSubClasse() const {
	return _subClasse;
}

void Nutriente::setValor(double valor) {
    _valor = valor;
}