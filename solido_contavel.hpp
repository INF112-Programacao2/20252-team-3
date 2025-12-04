#ifndef SOLIDOCONTAVEL_HPP
#define SOLIDOCONTAVEL_HPP

#include "alimento.hpp"

class SolidoContavel : public Alimento {
public:
	// Construtor chama o construtor da classe mae
 	SolidoContavel(int id, std::string categoria, std::string nome,std::string subclasse_tipo);
	SolidoContavel();

	// Destrutor vazio
	~SolidoContavel();

	virtual double getQuantidadeGramas() override;
	void converterQuantidadeParaGramas();

};


#endif