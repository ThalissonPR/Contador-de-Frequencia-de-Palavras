#ifndef ITERATORRB_HPP
#define ITERATORRB_HPP
#include "NodeRB.hpp"
#include <utility>
#include <string>
#include <iostream>

template <typename K, typename V>
class IteratorRB {
private:
    NodeRB<K, V>* atual;
    NodeRB<K, V>* nil;

public:

    // Construtor.
    IteratorRB(NodeRB<K, V>* inicio, NodeRB<K, V>* fim)
    : nil(fim), atual(inicio) {}

    // Operador de desigualdade.
    // Retorna verdadeiro caso os iteradores apontam para nós diferentes.
    bool operator!=(const IteratorRB& a) const {
        return atual != a.atual;
    }

    // Operador de desreferência.
    // Retorna uma referência ao par chave-valor atual.
    std::pair<K, V>& operator*() const {
        return atual->key;
    }

    // Operador de seta.
    // Permite acessar os membros do par chave-valor.
    std::pair<K, V>* operator->() const {
        return &(atual->key);
    }

    // Operador de incrementação.
    // Avança para o próximo nó em ordem crescente (in-order).
    IteratorRB& operator++() {
        if(atual == nil) {
            return *this;
        }

        // Se houver filho à direita, vai para o menor da subárvore direita.
        if(atual->right != nil) {
            atual = atual->right;
            while(atual->left != nil) {
                atual = atual->left;
            }
        } else { 
            // Sobe até encontrar um ancestral à esquerda.
            NodeRB<K, V>* a = atual->parent;
            while(a != nil && atual == a->right) {
                atual = a;
                a = a->parent;
            }
            atual = a;
        }

        return *this;
    }

};

#endif