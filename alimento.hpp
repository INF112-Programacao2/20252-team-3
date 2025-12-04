#ifndef ALIMENTO_HPP
#define ALIMENTO_HPP

#include <memory>
#include <string>
#include <vector>
#include <iostream> 
#include <algorithm> 

#include "nutriente.hpp" 

//  Enums Auxiliares 
enum class CategoriaAlimento {
    Cereais,
    Verduras_hortaliças,
    Frutas,
    Leguminosas,
    Nozes_sementes,
    Gorduras_óleos,
    Pescados_frutos_do_mar,
    Carnes_derivados,
    Leite_derivados,
    Bebidas,
    Ovos_derivados,
    Miscelâneas,
	ProdutosAçucarados,
    Outros_alimentos_industrializados,
    Alimentos_preparados,
    CategoriaDesconhecida 
};

enum class Refeicao {
    Cafe_Manha,
    Almoco,
    Jantar,
    Lanche,
    RefeicaoDesconhecida 
};


class Alimento {
protected:
    // Membros de Dados
    int _ID;
    std::string _nome;
    std::string _subClasse;
    std::string _unidadeMedida;
    
    CategoriaAlimento _categoria; 
    Refeicao _refeicao;          

    double _quantidade;
    double _quantidadeGramas;

    std::vector<std::shared_ptr<Nutriente>> _nutrientes;
	
public:
    // Construtores e Destrutor
    Alimento();
	Alimento(const int ID, const std::string& categoria, const std::string& nome);
    ~Alimento();

    // Getters
    int getID() const;
    std::string getNome() const;
    const std::vector<std::shared_ptr<Nutriente>>& getNutrientes() const;
    std::string getSubClasse() const;
    std::string getCategoria() const; 
    std::string getRefeicao();  
	double getQuantidade(); 
	virtual double getQuantidadeGramas() = 0; // metodo que sera implementado nas classes filhas
	std::string getUnidadeDeMedida();
    
    // Setters
    void setID(int ID);
    void setNome(const std::string& nome);
    void setSubClasse(std::string subclasse);
    void setCategoria(CategoriaAlimento categoria);
	void setCategoria(std::string categoria);
    void setRefeicao(Refeicao refeicao);
	void setRefeicao(std::string refeicao);
    void setQuantidade(double quantidade);
    void setQuantidadeGramas(double quantidade_gramas);
    void setUnidadeDeMedida(std::string unidade_medida);
	
	std::string CategoriaAlimentoParaString(CategoriaAlimento categoria) const;
	CategoriaAlimento StringParaCategoriaAlimento(const std::string& categoria_str);
    
    // Métodos de Lógica
    void adicionarNutriente(std::string tipo_nutriente, double valor_nutriente);
    Nutriente* getNutriente(std::string tipo_nutriente);


};

#endif // ALIMENTO_HPP