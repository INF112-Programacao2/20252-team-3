#ifndef LIQUIDO_HPP
#define LIQUIDO_HPP

#include "alimento.hpp"

class Liquido : public Alimento {
public:
	// Construtor chama o construtor da classe mae
 	Liquido(int id, std::string categoria, std::string nome,std::string subclasse_tipo);
	Liquido();
	
	// Destrutor vazio
	~Liquido();

	virtual double getQuantidadeGramas() override;
	void converterQuantidadeParaGramas();
};

#endif