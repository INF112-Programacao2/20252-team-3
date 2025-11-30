#include "Nutriente.h"

// Construtor inicializa os membros. Metas e consumo começam em zero.
Nutriente::Nutriente(const std::string& nome, const std::string& unidade)
    : nome(nome), 
      unidadeDeMedida(unidade), 
      quantidadeDiaria(0.0), 
      quantidadeIdeal(0.0) {}

// --- Getters e Setters ---

std::string Nutriente::getNome() const {
    return this->nome;
}

std::string Nutriente::getUnidade() const {
    return this->unidadeDeMedida;
}

double Nutriente::getMetaIdeal() const {
    return this->quantidadeIdeal;
}

double Nutriente::getConsumoDiario() const {
    return this->quantidadeDiaria;
}

void Nutriente::setConsumoDiario(double consumo) {
    this->quantidadeDiaria = consumo;
}
