#ifndef SOLIDOSNAOCONTAVEIS_HPP
#define SOLIDOSNAOCONTAVEIS_HPP    
#include <string>
#include "alimentos.hpp"

class SolidosNaoContaveis : public Alimentos{
    private:
    double gramasXicara;
    double gramasColherSopa;

    public:
    SolidosNaoContaveis(std::string _nome, int _id, double g_xicara, double g_sopa);
    virtual ~SolidosNaoContaveis();
    virtual void lerUnidadeMedida();
    virtual void converterQuantidade();
};

#endif