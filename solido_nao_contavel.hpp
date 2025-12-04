#ifndef SOLIDONAOCONTAVEL_HPP
#define SOLIDONAOCONTAVEL_HPP

#include "alimento.hpp"

class SolidoNaoContavel : public Alimento {
public:
	// Construtor chama o construtor da classe mae
 	SolidoNaoContavel(int id, std::string categoria, std::string nome,std::string subclasse_tipo);
	SolidoNaoContavel();
	
	// Destrutor vazio
	~SolidoNaoContavel();

	virtual double getQuantidadeGramas() override;
	void converterQuantidadeParaGramas();
};


#endif