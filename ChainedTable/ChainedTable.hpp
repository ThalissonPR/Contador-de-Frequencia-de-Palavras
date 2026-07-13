#ifndef CHAINEDTABLE_HPP
#define CHAINEDTABLE_HPP
#include "IteratorChained.hpp"
#include <utility>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cmath>

template <typename K, typename V, typename hashing = std::hash<K>, typename comparar = std::less<K>>
class ChainedTable {
private:
    size_t elements;
    size_t size;
    size_t comparisons;
    std::vector<std::list<std::pair<K, V>>>* table; // Lista de pares
    float fatorCarga; // Fator atual
    float fatorMax; // Fator máximo
    hashing hash;
    comparar compare;
    std::vector<std::pair<K, V>> ordKeys; // Vetor de chaves ordenadas
    bool dirtyKeys; // Indica se o vetor das chaves ordenadas está obsoleto

    // Retorna o menor número que é maior ou igual a x (caso x seja 2 retornamos o 3)
    size_t getNextPrime(size_t x) {
        if(x <= 2) {
            return 3;
        }

        if(x % 2 == 0) {
            x++;
        }

        while(true) {
            bool isPrime = true;
            for(size_t i = 3; i <= std::sqrt(x); i += 2) {
                if(x % i == 0) {
                    isPrime = false;
                    break;
                }
            }

            if(isPrime) {
                return x;
            }

            x += 2;
        }
    }

    // Retorna o índice da tabela hash de k 
    size_t hashCode(const K& k) const {
        return hash(k) % size; 
    }

    // Atualiza o vetor das chaves ordenadas
    void updateOrdKeys() {
        ordKeys.clear();
        ordKeys.reserve(elements);

        // Passa os pares da tabela para o vetor de ordenados
        for(size_t i = 0; i < size; i++) {
            for(const auto& pair : (*table)[i]) {
                ordKeys.push_back(pair);
            }
        }

        // Usa o comparador de chaves para ordenar o vetor com os pares
        std::sort(
            ordKeys.begin(), ordKeys.end(),
            [this](const std::pair<K, V>& x, const std::pair<K, V>& y) {
                comparisons++;
                return compare(x.first, y.first);
            }
        );

        dirtyKeys = false; // Atualiza as chaves ordenadas
    }
    
public:

    // Construtor (Criação).
    ChainedTable(size_t iSize = 19, const hashing& h = hashing())
    : elements(0), comparisons(0), size(getNextPrime(iSize)), table(new std::vector<std::list<std::pair<K, V>>>(size)),
    fatorCarga(0), fatorMax(0.75), hash(h), compare(comparar()), ordKeys(), dirtyKeys(true) {}

    // Destrutor.
    ~ChainedTable() {
        clear();
        delete table;
    }

    // Limpeza: Remover todos os pares chave-valor.
    void clear() {
        for(auto& bucket : *table) {
            bucket.clear();
        }
        elements = 0;
        dirtyKeys = true;
    }

    // Redimensiona o tamanho da tabela.
    // Todos os elementos da tabela atual são redistribuídos para a nova tabela.
    // Método muito útil quando o fator de carga ultrapassa o limite aceitável.
    void rehash(size_t n) {
        if(n <= size) {
            return;
        }

        n = getNextPrime(n);
        auto nova = new std::vector<std::list<std::pair<K, V>>>(n);

        for(const auto& bucket : *table) {
            for(const auto& pair : bucket) {
                size_t i = hash(pair.first) % n;
                (*nova)[i].push_back(pair);
            }
        }

        delete table;
        table = nova;
        size = n;
        dirtyKeys = true;
    }

    // Inserção; Adicionar um novo par chave-valor.
    // Caso a chave já exista, retorna falso e ignoramos a inserção.
    bool insert(const K& k, const V& v) {
        if(loadFactor() > fatorMax) {
            rehash(2 * size);
        }

        size_t slot = hashCode(k);
        for(auto& pair : (*table)[slot]) {
            comparisons++;
            if(pair.first == k) {
                return false;
            }
        }

        (*table)[slot].push_back(std::make_pair(k, v));
        elements++;
        dirtyKeys = true;
        return true;
    }

    // Remoção: Remover um par chave-valor usando a chave k.
    // Retorna true caso a remoção seja bem-sucedida, e false caso contrário.
    bool remove(const K& k) {
        size_t slot = hashCode(k);
        for(auto it = (*table)[slot].begin(); it != (*table)[slot].end(); ++it) {
            if(it->first == k) {
                (*table)[slot].erase(it);
                elements--;
                dirtyKeys = true;
                return true;
            }
        }
        return false;
    }

    // Retorna o número de slots presentes na tabela.
    size_t bucketCount() const {
        return size;
    }

    // Retorna o número de elementos em um slot da tabela.
    size_t bucketSize(size_t n) const {
        if(n >= table->size()) {
            throw std::out_of_range("Índice inválido!");
        }
        return (*table)[n].size();
    }

    // Retorna o índice do slot de uma chave k.
    size_t bucket(const K& k) const {
        return hashCode(k);
    }

    // Retorna o fator de carga atual.
    float loadFactor() const {
        return static_cast<float>(elements) / size;
    }

    // Retorna o fator de carga máximo.
    float maxLoadFactor() const {
        return fatorMax;
    }

    // Redimensiona a tabela para garantir que ela possa armazenar pelo menos n elementos.
    // Esse método evita múltiplos redimensionamentos futuros em grandes inserções.
    void reserva(size_t n) {
        if(n > size * fatorMax) {
            rehash(static_cast<size_t>(n / fatorMax));
        }
    }

    // Define o fator de carga máximo.
    void setMaxLoadFactor(float load) {
        if(load <= 0 || load < fatorCarga) {
            throw std::out_of_range("Fator de carga máximo fora do alcance!");
        }
        fatorMax = load;
        reserva(elements);
    }

    // Acesso: Recuperar o valor associado a uma chave específica.
    // Acesso == Busca.
    V& search(const K& k) {
        size_t slot = hashCode(k);
        for(auto& pair : (*table)[slot]) {
            comparisons++;
            if(pair.first == k) {
                return pair.second;
            }
        }
        throw std::out_of_range("Chave não encontrada!");
    }

    // Verificação de existência: Verificar se uma chave existe.
    bool contains(const K& k) {
        size_t slot = hashCode(k);
        for(auto& pair : (*table)[slot]) {
            comparisons++;
            if(pair.first == k) {
                return true;
            }
        }
        return false;
    }

    // Atualização: Modificar o valor associado a uma chave existente.
    void update(const K& k, V& v) {
        size_t slot = hashCode(k);
        for(auto& pair : (*table)[slot]) {
            if(pair.first == k) {
                pair.second = v;
                dirtyKeys = true;
                return;
            }
        }
    }

    // Tamanho: Obter o número de pares chave-valor na estrutura.
    size_t getSize() const {
        return elements;
    }

    bool isEmpty() const {
        return elements == 0;
    }

    size_t getComparisons() const {
        return comparisons;
    }

    // Retorna o vetor de pares chave-valor ordenados.
    // Importante para o uso do iterador.
    const std::vector<std::pair<K, V>>& getOrdKeys() const {
        return ordKeys;
    }

    // Iteração: Percorrer os pares chave-valor da estrutura.

    using iterator = IteratorChained<K, V, ChainedTable>;

    // Retorna um iterador para o início do vetor das chaves ordenadas.
    iterator begin() {
        if(dirtyKeys) {
            updateOrdKeys();
        }
        return iterator(this, 0);
    }

    // Retorna um iterador para o fim do vetor das chaves ordenadas.
    iterator end() {
        if(dirtyKeys) {
            updateOrdKeys();
        }
        return iterator(this, ordKeys.size());
    }

};

#endif
