#ifndef NUTRIENTE_HPP
#define NUTRIENTE_HPP

#include <string>
#include <iostream>
#include <utility>

class PerfilNutricional;

class Nutriente {
protected:
    std::string _nome;
    std::string _unidade;
    double _valor; // que seria a quantidade de nutriente
    double _metaIdeal; // que seria a meta para quantidade de nutriente
    std::string _subClasse;

    Nutriente(std::string nome, double valor);

public:
    virtual ~Nutriente() = default;

    // --- MÉTODOS VIRTUAIS PUROS ---
    
    // Força a implementação da lógica de cálculo da meta ideal.
    virtual void calcularMetaIdeal(PerfilNutricional& perfil) = 0;
    
    // --- Getters e Setters Comuns ---

    std::string getNome() const;
    std::string getUnidade() const;
    double getMetaIdeal() const;
	std::string getSubClasse() const;	
    
    double getValor() const;
    void setValor(double valor);

};

#endif