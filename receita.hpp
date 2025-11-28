#include "alimento.hpp"
#include <vector>

class Receita {
	private:
		std::vector <Alimento> _alimentoReceita;
		std::string descricaoReceita;
	public:
		void criarReceita();
		//void editarReceita();
};