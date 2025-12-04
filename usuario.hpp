#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>
#include <vector>
#include "perfil_nutricional.hpp"
#include "diario_alimentar.hpp"
//#include "receita.hpp"

std::string montarCaminhoDiario(const std::string& caminho_base, const std::string& segmento);
std::string montarCaminhoDiario(const std::string& caminho_base, const std::string& segmento1, const std::string& segmento2);

class Usuario { 
private:
	std::string _nome;
	PerfilNutricional _perfilNutricional;
	DiarioAlimentar _diario;
	//std::vector<Receita> _receitas;
	std::string _caminhoDiretorioDiario;
	
public:
	Usuario();
	Usuario(std::string nome, int idade, int peso, double altura);
	~Usuario(); 
	
	// Getters
	std::string getNome();
	int getIdade();
	double getPeso();
	double getAltura();
	std::string getNivelAtividade();
	PerfilNutricional getPerfilNutricional();
	DiarioAlimentar& getDiarioAlimentar();
	std::string getCaminhoDiretorioDiario();
	
	// Setters
	void setNome(std::string nome);
	void setIdade(int idade);
	void setPeso(int peso);
	void setAltura(double altura);
	void setNivelAtividade(std::string nivel_atividade);
	
	// Outras funcoes
	bool lerCaminhoDiretorioDiario();
	bool cadastrarUsuario();
	void leituraDadosUsuario();
	void verDadosUsuario() const;
	void editarDadosUsuario();
	bool salvarDadosEmArquivo() const;
	bool buscarDadosEmDiretorio();
	void adicionarAlimentoAoDiario();
	std::string lerAlimento();
	std::string cadastrarNovoAlimentoDB();
	std::vector<std::string> buscarMedidas(std::string subclasse_alimento="");
	int buscarNumMedidas(std::string subclasse_alimento="");
	void verAlimentosAdicionados();
	void editarAlimentosAdicionados();
	void verNutrientesCalculados();
	// void montarReceita();

};


#endif