#ifndef OPENTABLE_HPP
#define OPENTABLE_HPP
#include "IteratorOpen.hpp"
#include <functional>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream>

template <typename K, typename V, typename hashing = std::hash<K>, typename comparing = std::less<K>> 
class OpenTable {
private:
    static constexpr int empty = 0; 
    static constexpr int active = 1;
    static constexpr int deleted = 2;

    struct entrada {
        K key{};
        V value{};
        int state{empty};
    };

    size_t elements;
    size_t size;
    size_t comparisons;
    float fatorCarga;
    float fatorMax;
    hashing hash;
    comparing compare;
    std::vector<entrada> table;
    bool dirtyKeys;
    std::vector<std::pair<K, V>> ordKeys;
    
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

    // Utiliza double hashing para tratar as colisões.
    // Primeira função: função hash padrão.
    // Segunda função: função para gerar o incremento.
    size_t hashCode(const K& key, const size_t index = 0) const {
        size_t aux = hash(key);
        size_t aux2 = 1 + (aux % (size - 1));
        return (aux + index * aux2) % size;
    }

    void updateOrdKeys() {
        // Limpa e reserva espaço para o vetor de pares ordenados.
        ordKeys.clear();
        ordKeys.reserve(elements);

        // Copia todos os pares válidos para o vetor de ordenação.
        for(const auto& x : table) {
            if(x.state == active) {
                ordKeys.emplace_back(x.key, x.value);
            }
        }

        // Usa o comparador de chaves para ordenar o vetor com os pares chave-valor.
        std::sort(
            ordKeys.begin(), ordKeys.end(),
            [this](const std::pair<K, V>& x, const std::pair<K, V>& y) {
                comparisons++;
                return compare(x.first, y.first);
            }
        );

        // Atualiza as chaves ordenadas.
        dirtyKeys = false;
    }

public:

    // Construtor (Criação).
    OpenTable(size_t tamanho = 19, const hashing& h = hashing())
    : elements(0), comparisons(0), size(getNextPrime(tamanho)), table(std::vector<entrada>(this->size)),
      fatorCarga(0), fatorMax(0.75), hash(h), compare(comparing()), dirtyKeys(true), ordKeys() {}
    
    // Destrutor
    ~OpenTable() {
        clear();
    }

    // Limpeza: Remover todos os pares chave-valor da estrutura.
    void clear() {
        table.clear();
        table.resize(size);
        elements = 0;
        dirtyKeys = true;
    }

    // Redimensiona a tabela para um novo primo maior ou igual a n, realocando os elementos.
    // Utiliza o hashing duplo para a inserção desses elementos.
    void rehash(size_t n) {
        if(n <= size) {
            return;
        }

        n = getNextPrime(n);
        std::vector<entrada> nova(n);

        for(size_t i = 0; i < size; i++) {
            if(table[i].state == active) {
                size_t index;
                for(size_t j = 0; ;j++) {
                    index = (hash(table[i].key) + j * (1 + (hash(table[i].key) % (n - 1)))) % n;
                    if(nova[index].state != active) {
                        break;
                    }
                }
                nova[index].key = table[i].key;
                nova[index].value = table[i].value;
                nova[index].state = active;
            }
        }

        size = n;
        table = std::move(nova);
        dirtyKeys = true;
    }

    // Inserção: Adicionar um novo par chave-valor.
    // Retorna falso caso a chave já exista na tabela.
    bool insert(const K& key, const V& value) {
        if(loadFactor() > fatorMax) {
            rehash(2 * size);
        }

        for(size_t i = 0; i < size; ++i) {
            size_t index = hashCode(key, i);
            if(table[index].state != active) {
                table[index].key = key;
                table[index].value = value;
                table[index].state = active;
                elements++;
                dirtyKeys = true;
                return true;
            }

            comparisons++;
            if(table[index].key == key) {
                return false;
            }
        }

        return false;
    }

    // Remoção: Remover um par chave-valor usando a chave key.
    // Marca o local da chave removida como deletado.
    bool remove(const K& key) {
        for(size_t i = 0; i < size; ++i) {
            size_t index = hashCode(key, i);
            if(table[index].state == empty) {
                return false;
            }

            comparisons++;
            if(table[index].state == active && table[index].key == key) {
                table[index].state = deleted;
                elements--;
                dirtyKeys = true;
                return true;
            } 
        }

        return false;
    }

    // Acesso: Recuperar o valor associado a uma chave específica.
    // Acesso == Busca.
    // Caso a chave esteja presente na tabela, ela é retornada.
    // Caso a chave não esteja na estrutura, lança uma exceção.
    V& search(const K& key) {
        for(size_t i = 0; i < size; ++i) {
            size_t index = hashCode(key, i);
            if(table[index].state == empty) {
                break;
            }

            comparisons++;
            if(table[index].state == active && table[index].key == key) {
                return table[index].value;
            }
        }

        throw std::out_of_range("Chave não encontrada!");
    }

    // Verificação de existência: Verificar se uma chave existe na estrutura.
    // Retorna true caso a chave seja encontrada, e false caso contrário.
    bool contains(const K& key) {
        for(size_t i = 0; i < size; ++i) {
            size_t index = hashCode(key, i);
            if(table[index].state == empty) {
                return false;
            }

            comparisons++;
            if(table[index].state == active && table[index].key == key) {
                return true;
            }
        }

        return false;
    }

    // Atualização: Modificar o valor associado a uma chave existente.
    void update(const K& key, V& value) {
        for(size_t i = 0; i < size; ++i) {
            size_t index = hashCode(key, i);

            comparisons++;
            if(table[index].state == active && table[index].key == key) {
                table[index].value = value;
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

    // Retorna o número de slots da tabela.
    size_t bucketCount() const {
        return size;
    }

    // Retorna o fator de carga atual da tabela.
    float loadFactor() const {
        return static_cast<float>(elements) / size;
    }

    // Retorna o fator de carga máximo permitido.
    float maxLoadFactor() const {
        return fatorMax;
    }

    // Reserva capacidade suficiente para armazenar n elementos com base no fator de carga máximo.
    void reserva(size_t n) {
        if(n > size * fatorMax) {
            rehash(static_cast<size_t>(n / fatorMax));
        }
    }

    // Define o fator de carga máximo e faz a reserva de espaço.
    void setMaxLoadFactor(float load) {
        if(load <= 0 || load < fatorCarga) {
            throw std::out_of_range("Fator de carga máximo fora do alcance!");
        }
        fatorMax = load;
        reserva(elements);
    }

    // Retorna o vetor das chaves ordenadas.
    const std::vector<std::pair<K, V>>& getOrdKeys() const {
        return ordKeys;
    }

    // Iteração: Percorrer os pares de chave-valor da estrutura.

    using iterator = IteratorOpen<K, V, OpenTable<K, V, hashing, comparing>>;

    // Retorna um iterador apontando para o primeiro par chave-valor ordenado.
    iterator begin() {
        if(dirtyKeys) {
            updateOrdKeys();
        }
        return iterator(this, 0);
    }

    // Retorna um iterador para a posição após o último elemento válido.
    iterator end() {
        if(dirtyKeys) {
            updateOrdKeys();
        }
        return iterator(this, ordKeys.size());
    }

};

#endif
