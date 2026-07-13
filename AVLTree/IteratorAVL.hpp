#ifndef ITERATORAVL_HPP
#define ITERATORAVL_HPP
#include "NodeAVL.hpp"
#include <stack>
#include <utility>
#include <string>
#include <iostream>

// Iterador para percorrer uma árvore AVL em ordem crescente (in-order).
// Utiliza uma piha para simular a travessia recursiva.
// Permite o acesso aos pares chave-valor da árvore de forma ordenada.

template <typename K, typename V>
class IteratorAVL {
private:
    std::stack<NodeAVL<K, V>*> pilha;
    NodeAVL<K, V>* atual;

    // Função auxiliar para empilhar todos os nós à esquerda.
    void pushLeft(NodeAVL<K, V>* node) {
        while(node != nullptr) {
            pilha.push(node);
            node = node->left;
        }
    }

public:

    // Construtor.
    // Inicializa o iterador apontando para o menor elemento da árvore.
    IteratorAVL(NodeAVL<K, V>* root) : atual(nullptr) {
        pushLeft(root);
        if(!pilha.empty()) {
            atual = pilha.top();
            pilha.pop();
        }
    }

    // Operador de desigualdade.
    // Compara dois iteradores com base no ponteiro atual.
    bool operator!=(const IteratorAVL& aux) const {
        return atual != aux.atual;
    }

    // Operador de desreferência.
    // Retorna a referência ao par chave-valor do nó atual.
    const std::pair<K, V>& operator*() const {
        return atual->key;
    }

    // Operador da seta.
    // Permite acesso direto ao par chave-valor pelo ponteiro.
    const std::pair<K, V>* operator->() const {
        return &atual->key;
    }

    // Operador de incrementação.
    // Avança para o próximo nó em ordem crescente.
    IteratorAVL& operator++() {
        if(atual == nullptr) {
            return *this;
        }

        if(atual->right != nullptr) {
            pushLeft(atual->right);
        }

        if(!pilha.empty()) {
            atual = pilha.top();
            pilha.pop();
        } else {
            atual = nullptr;
        }

        return *this;
    }

};

#endif