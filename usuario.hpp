#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>
#include <vector>
#include "perfil_nutricional.hpp"
#include "diario_alimentar.hpp"
#include "receita.hpp"

class Usuario { 
private:
	std::string _nome;
	int _idade, _peso;
	double _altura;
	PerfilNutricional _perfilNutricional;
	DiarioAlimentar _diario;
	std::vector <Receita> _receitas;
	std::string _caminhoDiretorioDiario;
	
	enum CategoriaAlimento {
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
		Outros_alimentos_industrializados,
		Alimentos_preparados
	};
	
	enum Refeicao {
		Cafe_Manha
		Almoco,
		Jantar,
		Lanche
	}

public:
	Usuario();
	Usuario(std::string nome, int idade, int peso, double altura);
	~Usuario(); 
	
	// Getters
	std::string getNome();
	int getIdade();
	double getPeso();
	double getAltura();
	PerfilNutricional getPerfilNutricional();
	DiarioAlimentar getDiarioAlimentar();
	std::string getCaminhoDiretorioDiario();
	void cadastrarNovoAlimentoDB();
	
	// Setters
	void setNome(std::string nome);
	void setIdade(int idade);
	void setPeso(int peso);
	void setAltura(double altura);
	
	
	// Outras funcoes
	bool lerCaminhoDiretorioDiario();
	bool cadastrarUsuario();
	void lerDadosPessoaisUsuario();
	void lerDadosNutricionaisUsuario();
	void verDadosUsuario();
	bool salvarDadosUsuario();
	bool buscarDadosEmDiretorio();
	void adicionarAlimentoAoDiario();
	std::string lerAlimento();
	std::string cadastrarNovoAlimentoDB();
	std::vector<std::string> buscarMedidas();
	int buscarNumMedidas();
	void verAlimentosAdicionados();
	void editarAlimentosAdicionados();
	void verNutrientesCalculados();
	void montarReceita();
	
	void gerarPastaUsuario();
	void visualizarInfomacoesPessoais();
	void lerFatorFisico();
	void lerMetaCalorias();
	void visualizarPerfilNutricional();
	void salvarPerfilNutricional();
	void adicionarAlimentoDiario();
	void visualizarDiario();
	void adicionarNovoAlimento();
	void gerarArquivoHistorico(); 
};


#endif