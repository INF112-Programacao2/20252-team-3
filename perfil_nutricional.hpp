#ifndef PERFILNUTRICIONAL_HPP
#define PERFILNUTRICIONAL_HPP

#include <string>
#include <map>
#include <memory>
#include <vector>

#include "alimento.hpp"
#include "nutriente.hpp"

class PerfilNutricional {
private:
    std::string _sexo;
    int _idade;
    double _pesoKg;
    double _alturaCm;
    std::string _nivelAtividade; 
    double _caloriasDiariasTotais; // Necessidade calórica total calculada (CDT)

    void calcularCaloriasDiariasTotais();

    // Tabela estática para Fatores de Atividade Física (FA)
    static const std::map<std::string, double> FATORES_ATIVIDADE;

public:
    // Construtor
    PerfilNutricional(std::string sexo, int idade, double pesoKg, double alturaCm, std::string nivelAtividade);
    PerfilNutricional();
	
    // Destrutor virtual padrão
    virtual ~PerfilNutricional() = default;

    // --- Getters ---

    std::string getSexo() const;
    int getIdade() const;
    double getPesoKg() const;
    double getAlturaCm() const;
    std::string getNivelAtividade() const;

    // Getter essencial para a classe Nutriente
    double getCaloriasDiariasTotais() const;

    // --- Setters (para permitir atualizações no perfil) ---
    
    void setSexo(const std::string& sexo);
    void setIdade(int idade);
    void setPesoKg(double pesoKg);
    void setAlturaCm(double alturaCm);
    void setNivelAtividade(const std::string& nivelAtividade);
	void setCaloriasDiariasTotais(double caloriasDiariasTotais);
    
    // Método para re-calcular (chamado após qualquer setter que afete a CDT)
    void atualizarPerfil();
	std::unique_ptr<Nutriente> criarNutrienteSoma(const std::string& nome, double valor) const;
	std::vector<std::unique_ptr<Nutriente>> somarNutrientesAlimentos(const std::vector<std::unique_ptr<Alimento>>& alimentos) const;
	void gerarRelatorioDeConsumo(const std::vector<std::unique_ptr<Alimento>>& alimentosConsumidos) const;
};

#endif