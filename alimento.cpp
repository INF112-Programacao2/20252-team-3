#include <memory> // Para std::unique_ptr
#include <string>
#include <vector>
#include <algorithm>

#include "alimento.hpp"
#include "perfil_nutricional.hpp"
#include "macronutriente.hpp" 
#include "micronutriente.hpp" 

Alimento::Alimento() {}
Alimento::Alimento(const int ID, const std::string& categoria, const std::string& nome):
	_ID(ID), _categoria(StringParaCategoriaAlimento(categoria)), _nome(nome) 
	{
		_quantidade = 0.0; // ja inicializo as variaveis de quantidade
		_quantidadeGramas = 0.0;
	}
Alimento::~Alimento() {}

int Alimento::getID() const {return _ID;}
std::string Alimento::getNome() const {return _nome;}
const std::vector<std::shared_ptr<Nutriente>>& Alimento::getNutrientes() const {return _nutrientes;}
std::string Alimento::getSubClasse() const {return _subClasse;}
std::string Alimento::getCategoria() const{
    return CategoriaAlimentoParaString(_categoria);
}
std::string Alimento::getRefeicao() {
    switch (_refeicao) {
        case Refeicao::Cafe_Manha:
            return "Cafe da Manha";
        case Refeicao::Almoco:
            return "Almoco";
        case Refeicao::Jantar:
            return "Jantar";
        case Refeicao::Lanche:
            return "Lanche";
        default:
            return "Refeicao Desconhecida";
    }
}
double Alimento::getQuantidade() {return _quantidade;}
std::string Alimento::getUnidadeDeMedida() {return _unidadeMedida;}

void Alimento::setID(int ID) {_ID = ID;}
void Alimento::setNome(const std::string& nome) {_nome = nome;}
void Alimento::setSubClasse(std::string subclasse) {_subClasse = subclasse;}
void Alimento::setCategoria(CategoriaAlimento categoria) {_categoria = categoria;}
void Alimento::setCategoria(std::string categoria) {_categoria = StringParaCategoriaAlimento(categoria);}
void Alimento::setRefeicao(Refeicao refeicao) {_refeicao = refeicao;}
void Alimento::setRefeicao(std::string refeicao) {
    if (refeicao == "Cafe da Manha") {
        _refeicao = Refeicao::Cafe_Manha;
    } else if (refeicao == "Almoco") {
        _refeicao = Refeicao::Almoco;
    } else if (refeicao == "Jantar") {
        _refeicao = Refeicao::Jantar;
    } else if (refeicao == "Lanche") {
        _refeicao = Refeicao::Lanche;
    } else {
        // Define um valor padrão se a string não corresponder a nenhuma opção
        _refeicao = Refeicao::RefeicaoDesconhecida;
    }
}
void Alimento::setQuantidade(double quantidade) {_quantidade = quantidade;}
void Alimento::setQuantidadeGramas(double quantidade_gramas) {_quantidadeGramas = quantidade_gramas;}
void Alimento::setUnidadeDeMedida(std::string unidade_medida) {_unidadeMedida = unidade_medida;}

std::string Alimento::CategoriaAlimentoParaString(CategoriaAlimento categoria) const {
    switch (categoria) {
        case CategoriaAlimento::Cereais:
            return "Cereais e Derivados";
        case CategoriaAlimento::Verduras_hortaliças:
            return "Verduras, hortalicas e derivados";
        case CategoriaAlimento::Frutas:
            return "Frutas e derivados";
        case CategoriaAlimento::Leguminosas:
            return "Leguminosas e derivados";
        case CategoriaAlimento::Nozes_sementes:
            return "Nozes e sementes";
        case CategoriaAlimento::Gorduras_óleos:
            return "Gorduras e oleos";
        case CategoriaAlimento::Pescados_frutos_do_mar:
            return "Pescados e frutos do mar";
        case CategoriaAlimento::Carnes_derivados:
            return "Carnes e derivados";
        case CategoriaAlimento::Leite_derivados:
            return "Leite e derivados";
        case CategoriaAlimento::Bebidas:
            return "Bebidas";
        case CategoriaAlimento::Ovos_derivados:
            return "Ovos e derivados";
        case CategoriaAlimento::Miscelâneas:
            return "Miscelaneas";
		case CategoriaAlimento::ProdutosAçucarados:
			return "Produtos açucarados";
        case CategoriaAlimento::Outros_alimentos_industrializados:
            return "Outros alimentos industrializados"; 
        case CategoriaAlimento::Alimentos_preparados:
            return "Alimentos preparados";
        default:
            return "Categoria Desconhecida"; 
    }
}
CategoriaAlimento Alimento::StringParaCategoriaAlimento(const std::string& categoria_str) {
    if (categoria_str == "Cereais e Derivados") {
        return CategoriaAlimento::Cereais;
    } else if (categoria_str == "Verduras, hortalicas e derivados") {
        return CategoriaAlimento::Verduras_hortaliças;
    } else if (categoria_str == "Frutas e derivados") {
        return CategoriaAlimento::Frutas;
    } else if (categoria_str == "Leguminosas e derivados") {
        return CategoriaAlimento::Leguminosas;
    } else if (categoria_str == "Nozes e sementes") {
        return CategoriaAlimento::Nozes_sementes;
    } else if (categoria_str == "Gorduras e oleos") {
        return CategoriaAlimento::Gorduras_óleos;
    } else if (categoria_str == "Pescados e frutos do mar") {
        return CategoriaAlimento::Pescados_frutos_do_mar;
    } else if (categoria_str == "Carnes e derivados") {
        return CategoriaAlimento::Carnes_derivados;
    } else if (categoria_str == "Leite e derivados") {
        return CategoriaAlimento::Leite_derivados;
    } else if (categoria_str == "Bebidas") {
        return CategoriaAlimento::Bebidas;
    } else if (categoria_str == "Ovos e derivados") {
        return CategoriaAlimento::Ovos_derivados;
    } else if (categoria_str == "Miscelaneas") {
        return CategoriaAlimento::Miscelâneas;
    } else if (categoria_str == "Outros alimentos industrializados") {
        return CategoriaAlimento::Outros_alimentos_industrializados;
	} else if (categoria_str == "Produtos açucarados" || "Produtos acucarados") {
		return CategoriaAlimento::ProdutosAçucarados;
    } else if (categoria_str == "Alimentos preparados") {
        return CategoriaAlimento::Alimentos_preparados;
    } 
    else {
        return CategoriaAlimento::CategoriaDesconhecida; 
       
    }
}

void Alimento::adicionarNutriente(std::string tipo_nutriente, double valor_nutriente) {
    // 1. Converte o valor
    // RETIRAMOS
    
    // 2. Listas de nutrientes
    const std::vector<std::string> macronutrientes {
        "Energia ou Calorias",
        "Proteinas",
        "Lipidios",
        "Colesterol",
        "Carboidratos",
        "FibraAlimentar",
    };
    
    const std::vector<std::string> micronutrientes {
        "Calcio",
        "Magnesio",
        "Ferro",
        "Sodio",
        "Potassio",
        "VitaminaA",
		"VitaminaB1",
        "VitaminaC"
    };
    
    // 3. Ponteiro para o objeto Nutriente (ou derivado)
    std::unique_ptr<Nutriente> novo_nutriente = nullptr;
    
    // 4. Teste e Criação do Objeto Derivado
    
    // Testa se é um MacroNutriente
    if (std::find(macronutrientes.begin(), macronutrientes.end(), tipo_nutriente) != macronutrientes.end()) {
        // Cria um objeto MacroNutriente dinamicamente
        novo_nutriente = std::make_unique<MacroNutriente>(tipo_nutriente, valor_nutriente); // chamo o construtor da classe
    } 
    // Testa se é um MicroNutriente
    else if (std::find(micronutrientes.begin(), micronutrientes.end(), tipo_nutriente) != micronutrientes.end()) {
        // Cria um objeto MicroNutriente dinamicamente
        novo_nutriente = std::make_unique<MicroNutriente>(tipo_nutriente, valor_nutriente); // chamo o construtor da classe
	} 
    // Erro: Tipo não reconhecido
    else {
		std::cerr << "Debug: " << tipo_nutriente << " nao e reconhecido como um tipo de nutriente\n";
        throw std::invalid_argument("Nao foi possivel ler os nutrientes da tabela");
    }

    // 5. Adiciona o ponteiro único ao vetor
    // O ponteiro é movido para dentro do vetor
    _nutrientes.push_back(std::move(novo_nutriente)); // std::move é necessario para que o unique_ptr aceite o novo objeto
    
}
Nutriente* Alimento::getNutriente(std::string tipo_nutriente) {
    // 1. Itera sobre o vetor de ponteiros unicos (_nutrientes)
    for (const auto& nutriente_ptr : _nutrientes) {
        // 2. Compara o tipo do nutriente armazenado com o tipo buscado
        if (nutriente_ptr->getNome() == tipo_nutriente) {
            // 3. Se encontrar, retorna o ponteiro bruto para o objeto.
            // O metodo .get() retorna o ponteiro Nutriente* contido no unique_ptr.
            return nutriente_ptr.get();
        }
    }
    // 4. Retorna nullptr se nao encontrar o nutriente
    return nullptr; 
}


