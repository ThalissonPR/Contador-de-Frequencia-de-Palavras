#include <iostream>
#include <string>
#include "../RBTree/RBTree.hpp"

/*
Para rodar esse teste utilize os seguintes comandos:
1) g++ -std=c++17 mainRB.cpp -I../RBTree -o rb
2) ./rb
*/

int main() {
    RBTree<std::string, int> rb;

    std::cout << "====== Testes da Árvore Rubro-Negra Genérica ======\n";

    // Inserção de palavras
    rb.insert("banana", 1);
    rb.insert("abacate", 2);
    rb.insert("laranja", 3);
    rb.insert("uva", 4);
    rb.insert("manga", 5);

    std::cout << "Tamanho atual: " << rb.getSize() << "\n";

    // Teste contains
    std::cout << "\n=== contains ===\n";
    std::cout << "Existe 'banana'? " << (rb.contains("banana") ? "Sim" : "Não") << "\n";
    std::cout << "Existe 'melancia'? " << (rb.contains("melancia") ? "Sim" : "Não") << "\n";

    // Teste search
    std::cout << "\n=== search ===\n";
    try {
        std::cout << "'laranja' => " << rb.search("laranja") << "\n";
    } catch (std::out_of_range& e) {
        std::cout << "Erro: " << e.what() << "\n";
    }

    // Teste update
    std::cout << "\n=== update ===\n";
    std::string chave = "laranja";
    int novoValor = 10;
    rb.update(chave, novoValor);
    std::cout << "'laranja' (atualizado) => " << rb.search("laranja") << "\n";

    // Teste iterador (in-order)
    std::cout << "\n=== Iterando (in-order) ===\n";
    for (auto it = rb.begin(); it != rb.end(); ++it) {
        std::cout << it->first << ": " << it->second << "\n";
    }

    // Teste remoção
    std::cout << "\n=== Remoção ===\n";
    rb.remove("abacate");
    std::cout << "Após remover 'abacate', contém? " << (rb.contains("abacate") ? "Sim" : "Não") << "\n";

    // Métricas
    std::cout << "\nTamanho final: " << rb.getSize() << "\n";
    std::cout << "Comparações: " << rb.getComparisons() << "\n";
    std::cout << "Rotações: " << rb.getRotations() << "\n";

    // Limpeza
    std::cout << "\n=== clear ===\n";
    rb.clear();
    std::cout << "Tamanho após clear: " << rb.getSize() << "\n";
    std::cout << "Está vazia? " << (rb.isEmpty() ? "Sim" : "Não") << "\n";

    return 0;
}