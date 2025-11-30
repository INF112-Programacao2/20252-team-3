#ifndef ALIMENTOS_HPP
#define ALIMENTOS_HPP
#include <string>
#include <vector>
#include "nutrientes.hpp"

class Alimentos{
    private:
    std::string nome;
    std::vector<Nutrientes*> nutrientes;
    int ID;

    protected:
    double quantidade;
    double quantidadeGramas;
    std::string unidadeDeMedida;

    public:
    Alimentos(std::string _nome, int _id);
    virtual ~Alimentos();
    void lerQuantidade();
    void adicionarNutriente(std::string tipo_nutriente, double quantidade_gramas);
    virtual void converterQuantidade() = 0;
    virtual void lerUnidadeMedida() = 0;
};

#endif