#ifndef SOLIDOSCONTAVEIS_HPP
#define SOLIDOSCONTAVEIS_HPP    
#include <string>
#include "alimentos.hpp"
class SolidosContaveis : public Alimentos{
    private:
    double gramasUnidade;

    public:
    SolidosContaveis(std::string _nome,int _id, double g_uni);
    virtual ~SolidosContaveis();
    virtual void lerUnidadeMedida();
    virtual void converterQuantidade();
};

#endif