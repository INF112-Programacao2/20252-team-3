#ifndef NUTRIENTE_H
#define NUTRIENTE_H

#include <string>

// Declaração antecipada (Forward Declaration)
// Evita incluir "PerfilNutricional.h" aqui, melhorando a compilação.
class PerfilNutricional;

/**
 * @class Nutriente
 * @brief Classe base abstrata para todos os nutrientes.
 * Define a interface comum para cálculo de metas e armazenamento de consumo.
 */
class Nutriente {
protected:
    std::string nome;
    std::string unidadeDeMedida; // "g", "mg", "mcg"
    
    // quantidadeDiaria: O CONSUMO (será atualizado pelo DiarioAlimentar)
    double quantidadeDiaria;
    
    // quantidadeIdeal: A META (será calculada pelo nosso método)
    double quantidadeIdeal;

public:
    /**
     * @brief Construtor base.
     * @param nome Nome do nutriente (ex: "Proteína").
     * @param unidade Unidade de medida (ex: "g").
     */
    Nutriente(const std::string& nome, const std::string& unidade);

    /**
     * @brief Destrutor virtual padrão. Essencial para polimorfismo.
     */
    virtual ~Nutriente() = default;

    // --- MÉTODOS VIRTUAIS PUROS (O "Contrato") ---

    /**
     * @brief Calcula e define a meta (quantidadeIdeal) deste nutriente.
     * Esta é a principal função polimórfica. Ela lê os dados do perfil
     * (calorias, percentuais, idade, sexo) para definir a meta.
     * @param perfil O perfil nutricional do usuário.
     */
    virtual void calcularMetaIdeal(const PerfilNutricional& perfil) = 0;

    /**
     * @brief Retorna o tipo de nutriente.
     * @return "Macronutriente" ou "Micronutriente".
     */
    virtual std::string getTipo() const = 0;

    // --- Getters e Setters ---
    std::string getNome() const;
    std::string getUnidade() const;

    /**
     * @brief Obtém a meta ideal (RDA) calculada.
     */
    double getMetaIdeal() const;

    /**
     * @brief Obtém o consumo diário (calculado pelo DiarioAlimentar).
     */
    double getConsumoDiario() const;

    /**
     * @brief Define o consumo diário.
     * Este método será chamado por outra classe (ex: DiarioAlimentar)
     * após somar o consumo de todas as refeições.
     * @param consumo O valor total consumido.
     */
    void setConsumoDiario(double consumo);
};

#endif // NUTRIENTE_H
