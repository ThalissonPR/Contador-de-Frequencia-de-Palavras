#include "../AVLTree/AVLTree.hpp"
#include "../RBTree/RBTree.hpp"
#include "../ChainedTable/ChainedTable.hpp"
#include "../OpenTable/OpenTable.hpp"
#include "dictionary.hpp"
#include "comparing.hpp"
#include "helpers.hpp"
#include <iostream>
#include <string>

// Exibe no terminal como usar o programa e as opções disponíveis de estruturas de dados.
void instructions(const std::string& programa) {
    std::cerr << "\nInstruções: " << programa << " 'estrutura' 'arquivo'\n\n";
    std::cerr << "Estruturas disponíveis:\n" 
              << " avl     -> Árvore AVL\n" 
              << " rb      -> Árvore Rubro-Negra\n" 
              << " chained -> Tabela Hash Com Encadeamento\n" 
              << " open    -> Tabela Hash Com Endereçamento Aberto\n\n";
    std::cerr << "Exemplo de uso: " << programa << " avl exemplo.txt\n\n";
}

int main(int argc, char* argv[]) {
    // O usuário pediu ajuda.
    if(argc == 2 && std::string(argv[1]) == "--help") {
        instructions(argv[0]);
        return 0;
    }

    // Verifica se o programa recebeu exatamente 3 argumentos:
    // [nome do executável] [modo] [arquivo]. 
    if(argc != 3) {
        // Caso contrário, mostra como chamar as instruções de uso e retorna um erro.
        std::cerr << "Erro: Número de argumentos inválidos!\n";
        std::cerr << "Use '" << argv[0] << " --help' para ver como utilizar o programa!\n";
        return 1;
    }

    // Extrai o modo de operação e o nome do arquivo a partir dos argumentos.
    std::string modo = argv[1];
    std::string arquivo = argv[2];

    // Seleciona a estrutura de dados com base no modo escolhido.
    if(modo == "avl") {
        dictionary<AVLTree<icu::UnicodeString, int, comparing>> dicionario;
        helpers::processarArquivo(dicionario, arquivo, modo);
    } else if(modo == "rb") {
        dictionary<RBTree<icu::UnicodeString, int, comparing>> dicionario;
        helpers::processarArquivo(dicionario, arquivo, modo);
    } else if(modo == "chained") {
        dictionary<ChainedTable<icu::UnicodeString, int, hashUni, comparing>> dicionario;
        helpers::processarArquivo(dicionario, arquivo, modo);
    } else if(modo == "open") {
        dictionary<OpenTable<icu::UnicodeString, int, hashUni, comparing>> dicionario;
        helpers::processarArquivo(dicionario, arquivo, modo);
    } else {
        // Caso o modo seja inválido, mostra o comando para acessar as instruções e retorna o erro.
        std::cerr << "Erro: Essa estrutura não foi reconhecida!\n";
        std::cerr << "Use '" << argv[0] << " --help' para ver as opções disponíveis!\n";
        return 1; 
    }

}