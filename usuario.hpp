#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>
//#include "perfil_nutricional.hpp"
//#include "diario_alimentar.hpp"

class Usuario { 
private:
	std::string _nome;
	int _idade, _peso;
	double _altura;
	//PerfilNutricional _perfilNutricional;
	//DiarioAlimentar _diario;
	std::string  _caminhoDiretorioDiario;

public:
	Usuario();
	Usuario(std::string nome, int idade, int peso, double altura);
	~Usuario(); 
	
	// Getters
	std::string getNome();
	int getIdade();
	double getPeso();
	double getAltura();
	//PerfilNutricional getPerfilNutricional();
	//DiarioAlimentar getDiarioAlimentar();
	std::string getCaminhoDiretorioDiario();
	
	
	// Setters
	//void setFatorFisico(std::string fator_fisico);
	//void setMetaCalorica(int meta_calorica);
	void setCaminhoDiretorioDiario(std::string caminho_diretorio);
	
	// Outras funcoes
	void lerCaminhoDiretorioDiario();
	void cadastrarUsuario();
	void lerDadosPessoaisUsuario();
	void lerDadosNutricionaisUsuario();
	void verDadosUsuario();
	bool salvarDadosUsuario();
	bool buscarDadosEmDiretorio();
	void lerAlimento();
	// void verAlimentosAdicionados();
	// void editarAlimentosAdicionados();
	// void verNutrientesCalculados();
	
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
