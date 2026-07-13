#include <iostream>
#include <string>
#include "../AVLTree/AVLTree.hpp"

/*
Para rodar esse teste utilize os seguintes comandos:
1) g++ -std=c++17 mainAVL.cpp -I../AVLTree -o avl
2) ./avl
*/

int main() {
    AVLTree<std::string, int> avl;

    std::cout << "====== Testes da AVL Genérica ======\n";

    // Inserção de palavras
    std::string p1 = "banana", p2 = "abacate", p3 = "laranja", p4 = "uva", p5 = "manga";
    int v1 = 1, v2 = 2, v3 = 3, v4 = 4, v5 = 5;

    avl.insert(p1, v1);
    avl.insert(p2, v2);
    avl.insert(p3, v3);
    avl.insert(p4, v4);
    avl.insert(p5, v5);

    std::cout << "Tamanho atual: " << avl.getSize() << "\n";

    // Teste contains
    std::cout << "\n=== contains ===\n";
    std::cout << "Existe 'banana'? " << (avl.contains(p1) ? "Sim" : "Não") << "\n";
    std::string inexistente = "melancia";
    std::cout << "Existe 'melancia'? " << (avl.contains(inexistente) ? "Sim" : "Não") << "\n";

    // Teste search
    std::cout << "\n=== search ===\n";
    try {
        std::cout << "'laranja' => " << avl.search(p3) << "\n";
    } catch (std::out_of_range& e) {
        std::cout << "Erro: " << e.what() << "\n";
    }

    // Teste update
    std::cout << "\n=== update ===\n";
    int novoValor = 10;
    avl.update(p3, novoValor);
    std::cout << "'laranja' (atualizado) => " << avl.search(p3) << "\n";

    // Teste iterador
    std::cout << "\n=== Iterando (in-order) ===\n";
    for (auto it = avl.begin(); it != avl.end(); ++it) {
        std::cout << it->first << ": " << it->second << "\n";
    }

    // Teste remove
    std::cout << "\n=== Remoção ===\n";
    avl.remove(p2); // remove abacate
    std::cout << "Após remover 'abacate', contém? " << (avl.contains(p2) ? "Sim" : "Não") << "\n";

    // Tamanho e métricas
    std::cout << "\nTamanho final: " << avl.getSize() << "\n";
    std::cout << "Comparações: " << avl.getComparisons() << "\n";
    std::cout << "Rotações: " << avl.getRotations() << "\n";

    // Teste clear
    std::cout << "\n=== Clear ===\n";
    avl.clear();
    std::cout << "Tamanho após clear: " << avl.getSize() << "\n";
    std::cout << "Árvore está vazia? " << (avl.isEmpty() ? "Sim" : "Não") << "\n";

    return 0;
}
