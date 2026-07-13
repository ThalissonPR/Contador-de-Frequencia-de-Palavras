#include <iostream>
#include <string>
#include "../ChainedTable/ChainedTable.hpp"

/*
Para rodar esse teste utilize os seguintes comandos:
1) g++ -std=c++17 mainChained.cpp -I../ChainedTable -o chained
2) ./chained
*/

int main() {
    ChainedTable<std::string, int> tabela;

    std::cout << "====== Testes da Tabela Hash com Encadeamento ======\n";

    // Inserção de palavras
    tabela.insert("banana", 1);
    tabela.insert("abacate", 2);
    tabela.insert("laranja", 3);
    tabela.insert("uva", 4);
    tabela.insert("manga", 5);

    std::cout << "Tamanho atual: " << tabela.getSize() << "\n";

    // Teste contains
    std::cout << "\n=== contains ===\n";
    std::cout << "Existe 'banana'? " << (tabela.contains("banana") ? "Sim" : "Não") << "\n";
    std::cout << "Existe 'melancia'? " << (tabela.contains("melancia") ? "Sim" : "Não") << "\n";

    // Teste search
    std::cout << "\n=== search ===\n";
    try {
        std::cout << "'laranja' => " << tabela.search("laranja") << "\n";
    } catch (std::out_of_range& e) {
        std::cout << "Erro: " << e.what() << "\n";
    }

    // Teste update
    std::cout << "\n=== update ===\n";
    std::string chave = "laranja";
    int novoValor = 10;
    tabela.update(chave, novoValor);
    std::cout << "'laranja' (atualizado) => " << tabela.search("laranja") << "\n";

    // Teste iterador (ordem de chaves)
    std::cout << "\n=== Iterando (ordem das chaves) ===\n";
    for (auto it = tabela.begin(); it != tabela.end(); ++it) {
        std::cout << it->first << ": " << it->second << "\n";
    }

    // Teste remoção
    std::cout << "\n=== Remoção ===\n";
    tabela.remove("abacate");
    std::cout << "Após remover 'abacate', contém? " << (tabela.contains("abacate") ? "Sim" : "Não") << "\n";

    // Métricas
    std::cout << "\nTamanho final: " << tabela.getSize() << "\n";
    std::cout << "Comparações: " << tabela.getComparisons() << "\n";
    std::cout << "Fator de carga: " << tabela.loadFactor() << "\n";
    std::cout << "Buckets totais: " << tabela.bucketCount() << "\n";

    // Limpeza
    std::cout << "\n=== clear ===\n";
    tabela.clear();
    std::cout << "Tamanho após clear: " << tabela.getSize() << "\n";
    std::cout << "Está vazia? " << (tabela.isEmpty() ? "Sim" : "Não") << "\n";

    return 0;
}
