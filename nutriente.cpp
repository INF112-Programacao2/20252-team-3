#include "nutriente.hpp"

Nutriente::Nutriente(const std::string& nome, const std::string& unidade)
    : nome(nome), 
      unidadeDeMedida(unidade), 
      quantidadeDiaria(0.0), 
      quantidadeIdeal(0.0) {}

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

// Útil para verificar se os cálculos estão rodando sem precisar abrir a interface completa
void Nutriente::exibirInformacoes() const {
    std::cout << "Nutriente: " << nome 
              << " | Consumo Atual: " << quantidadeDiaria << " " << unidadeDeMedida
              << " | Meta: " << quantidadeIdeal << " " << unidadeDeMedida << std::endl;
}
