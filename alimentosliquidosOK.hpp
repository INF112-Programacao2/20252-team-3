#ifndef ALIMENTOSLIQUIDOS_HPP
#define ALIMENTOSLIQUIDOS_HPP
#include <string>
#include "alimentos.hpp"

class AlimentosLiquidos : public Alimentos{
    public:
    AlimentosLiquidos(std::string _nome, int _id);
    virtual ~AlimentosLiquidos();
    virtual void lerUnidadeMedida();
    virtual void converterQuantidade();
};

#endif