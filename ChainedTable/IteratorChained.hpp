#ifndef ITERATORCHAINED_HPP
#define ITERATORCHAINED_HPP
#include <utility>
#include <vector>
#include <string>
#include <iostream>

template <typename K, typename V, typename Table>
class IteratorChained {
private:
    const Table* table; // Ponteiro constante para a tabela hash.
    size_t indice;      // Índice atual no vetor de pares ordenados.

public:

    // Construtor.
    IteratorChained(const Table* t, size_t i)
    : table(t), indice(i) {}

    // Operador de desigualdade.
    // Verifica se dois iteradores são diferentes.
    bool operator!=(const IteratorChained& a) const {
        return indice != a.indice;
    }

    // Operador de desreferência.
    // Retorna a referência ao par chave-valor atual.
    const std::pair<K, V>& operator*() const {
        return table->getOrdKeys()[indice];
    }

    // Operador de seta.
    // Permite acessar os membros do par chave-valor pelo ponteiro.
    const std::pair<K, V>* operator->() const {
        return &table->getOrdKeys()[indice];
    }

    // Operador de incrementação.
    // Avança o iterador para o próximo par chave-valor.
    IteratorChained& operator++() {
        indice++;
        return *this;
    }
    
};

#endif