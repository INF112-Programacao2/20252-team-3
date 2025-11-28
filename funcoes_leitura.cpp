//Funcoes de leitura de int, char, string, entre outros
// Funcoes usadas em diferentes classes e em main

#include <exception>
#include <limits>
#include <iostream>
#include <filesystem>
#include "funcoes_leitura.hpp"

namespace fs = std::filesystem; // namespace para filesystem

char ler_S_ou_N() {
	char resposta_char;
	try {
		if (!(std::cin >> resposta_char)) { // le o primeiro caracter 
			std::cin.clear(); // limpo o estado do cin
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignoro o valor informado
			throw std::logic_error("Erro na leitura do S ou N");
		}
		else // mesmo caso a leitura esteja correta, ignoro o resto da linha para nao afetar leituras futuras
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignoro o valor informado
		if (resposta_char != 'S' && resposta_char != 'N' && resposta_char != 's' && resposta_char != 'n') // se for diferente de todos esses...
			throw std::invalid_argument("Valor digitado precisa comecar com S/s ou N/n");
	} catch (std::invalid_argument &e) {
		std::cerr << "Erro: " << e.what() << std::endl;
		return '\0';
	} catch (std::logic_error &e) {
		std::cerr << "Erro: " << e.what() << std::endl;
		return '\0';
	} catch (std::exception &e) {
		std::cerr << "Erro inesperado: " << e.what() << std::endl;
		return '\0';
	}
	if (resposta_char == 'S' || resposta_char == 's')
		return 'S';
	else
		return 'N';
}

int ler_num_no_intervalo(int num_inicial, int num_final) {
	int inteiro;
	try {
		inteiro = ler_numero();
		if (inteiro < num_inicial || inteiro > num_final)
			throw std::invalid_argument("Valor precisa estar entre 1 e 4\n");
	} catch (std::invalid_argument &e) {
		std::cerr << "Erro: " << e.what() << std::endl;
		return 0;
	}
	 
	return inteiro;
}

std::string ler_caminho_dir(std::string dir_especifico) {
	std::string caminho_diretorio;
	try {
		if (!(std::getline(std::cin, caminho_diretorio))) { // Leio a linha inteira. comando retorna true quando valor e valido
			std::cin.clear(); // limpo o estado do cin
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignoro o valor informado
			throw std::exception();
		}
		else { // se foi possivel fazer a leitura, retiro as "" no começo e fim do caminho (algo comum de haver)
			// 1. Remove aspas do início (se existir)
			if (!caminho_diretorio.empty() && caminho_diretorio.front() == '"') {
				caminho_diretorio.erase(0, 1);
			}
			// 2. Remove aspas do final (se existir)
			if (!caminho_diretorio.empty() && caminho_diretorio.back() == '"') {
				caminho_diretorio.pop_back(); // Remove o último caractere
			}
		}
		if (dir_especifico != "") // significa que foi especificado uma Pasta
			if (fs::path(caminho_diretorio).filename() != fs::path(dir_especifico)) // metodo .filename() vem da classe path
				throw std::invalid_argument("Caminho informado nao pertence ao diretorio solicitado");
		if (!(fs::exists(caminho_diretorio))) // verifica se diretorio existe
			throw std::runtime_error("Caminho informado nao encontrado");
		if (!(fs::is_directory(caminho_diretorio))) // verifica se o caminho se refere a um diretorio
			throw std::logic_error("Caminho informado nao se configura como diretorio (pasta)");
	} catch (std::invalid_argument &e){
		std::cerr << "Erro: " << e.what() << std::endl;
		return "";
	} catch (std::runtime_error &e) {
		std::cerr << "Erro: " << e.what() << std::endl;
		return "";
	} catch (std::logic_error &e) {
		std::cerr << "Erro: " << e.what() << std::endl;
		return "";
	} catch (std::exception &e) {
		std::cerr << "Erro inesperado: " << e.what() << std::endl;
		return "";
	}
	return caminho_diretorio; // converte para string
}

std::string ler_nome() {
	std::string resposta_str;
	
	try {
		std::getline(std::cin, resposta_str);
		for (unsigned char caracter : resposta_str) {
			if (!(std::isalpha(caracter)) && caracter != ' ') // se algum dos caracteres nao for letra e nao for espaco (eu conto como letra))
				throw std::invalid_argument("Nome precisa ter apenas letras (sem numeros ou caracteres especiais");
		}
	} catch (std::invalid_argument &e) {
			std::cerr << "Erro: " << e.what() << std::endl;
			return "";
	}
	return resposta_str;
}

int ler_numero() {
	int inteiro;
	try {
		if (!(std::cin >> inteiro)) { // cin retorna true quando valor e valido
			std::cin.clear(); // limpo o estado do cin
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignoro o valor informado
			throw std::logic_error("Valor informado precisa ser inteiro");
		}
		else // mesmo caso a leitura esteja correta, ignoro o resto da linha para nao afetar leituras futuras
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignoro o valor informado
	} catch (std::logic_error &e) {
		std::cerr << "Erro: " << e.what() << std::endl;
		return 0;
	} catch (std::exception &e) {
		std::cerr << "Erro inesperado: " << e.what() << std::endl;
		return 0;
	}
	 
	return inteiro;
}

