#ifndef NUTRIENTE_HPP
#define NUTRIENTE_HPP

#include <string>
#include <iostream>

// Forward declaration: avisa que a classe existe sem precisar incluir o arquivo todo aqui.
// Evita erro de inclusão circular.
class PerfilNutricional;

class Nutriente {
protected:
    std::string nome;
    std::string unidadeDeMedida; // Ex: "g", "mg", "mcg"
    double quantidadeDiaria;     // Quanto o usuário consumiu (acumulado)
    double quantidadeIdeal;      // A meta calculada para o usuário

public:
    Nutriente(const std::string& nome, const std::string& unidade);
    virtual ~Nutriente() = default; // Destrutor virtual é obrigatório para herança

    // --- MÉTODOS VIRTUAIS PUROS (O Polimorfismo acontece aqui) ---
    // Cada filho (Macro/Micro) deve implementar sua própria lógica de cálculo.
    // Nota: Recebe PerfilNutricional sem 'const' para compatibilidade com o código atual do grupo.
    virtual void calcularMetaIdeal(PerfilNutricional& perfil) = 0;
    
    virtual std::string getTipo() const = 0;

    // --- Getters e Setters Comuns ---
    std::string getNome() const;
    std::string getUnidade() const;
    double getMetaIdeal() const;
    
    double getConsumoDiario() const;
    void setConsumoDiario(double consumo); // Usado pelo Diario para somar o consumo

    // Método prático para debug: mostra o estado atual do nutriente no console
    virtual void exibirInformacoes() const;
};

#endif
