#ifndef ITERATOROPEN_HPP
#define ITERATOROPEN_HPP
#include <utility>
#include <vector>
#include <iostream>
#include <string>

template <typename K, typename V, typename Table>
class IteratorOpen {
private:
    const Table* table;
    size_t indice;

public:

    // Construtor.
    IteratorOpen(const Table* t, size_t i)
    : table(t), indice(i) {}

    // Operador de desigualdade.
    // Verifica se dois iteradores são diferentes.
    bool operator!=(const IteratorOpen& a) const {
        return indice != a.indice;
    }

    // Operador de seta.
    // Acessa o conteúdo do par chave-valor.
    const std::pair<K, V>* operator->() const {
        return &table->getOrdKeys()[indice];
    }

    // Operador de desreferência.
    // Acesso pela desreferenciação.
    const std::pair<K, V>& operator*() const {
        return table->getOrdKeys()[indice];
    }

    // Operador de incrementação.
    // Avança o iterador para o próximo elemento.
    IteratorOpen& operator++() {
        ++indice;
        return *this;
    }

};

#endif