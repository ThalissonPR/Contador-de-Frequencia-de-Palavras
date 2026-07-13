#ifndef AVLTREE_HPP
#define AVLTREE_HPP
#include "NodeAVL.hpp"
#include "IteratorAVL.hpp"
#include <string>
#include <iostream>

template<typename K, typename V, typename comparar = std::less<K>>
class AVLTree {
private:    
    NodeAVL<K, V>* root;
    comparar compare;
    int comparisons;
    int size;
    int rotations;

    int height(NodeAVL<K, V>* node) const {
        if(node == nullptr) {
            return 0;
        }
        return node->height;
    }

    int balance(NodeAVL<K, V>* node) const {
        return height(node->right) - height(node->left);
    }

    NodeAVL<K, V>* right_rotation(NodeAVL<K, V>* node) {
        rotations++;
        if(!node || !node->left) {
            return node;
        }

        NodeAVL<K, V>* u = node->left;
        node->left = u->right;
        u->right = node;

        node->height = 1 + std::max(height(node->left), height(node->right));
        u->height = 1 + std::max(height(u->left), height(u->right));

        return u;
    }

    NodeAVL<K, V>* left_rotation(NodeAVL<K, V>* node) {
        rotations++;
        if(!node || !node->right) {
            return node;
        }

        NodeAVL<K, V>* u = node->right;
        node->right = u->left;
        u->left = node;

        node->height = 1 + std::max(height(node->left), height(node->right));
        u->height = 1 + std::max(height(u->left), height(u->right));

        return u;
    }
    
    void _clear(NodeAVL<K, V>* node) {
        if(node == nullptr) {
            return;
        }
        _clear(node->left);
        _clear(node->right);
        delete node;
    }

    NodeAVL<K, V>* fixupNode(NodeAVL<K, V>* node, const K& key) {
        int bal = balance(node);

        comparisons++;
        if(bal < -1 && compare(key, node->left->key.first)) {
            return right_rotation(node);
        }

        comparisons++;
        if(bal < -1 && compare(node->left->key.first, key)) {
            node->left = left_rotation(node->left);
            return right_rotation(node);
        }

        comparisons++;
        if(bal > 1 && compare(node->right->key.first, key)) {
            return left_rotation(node);
        }

        comparisons++;
        if(bal > 1 && compare(key, node->right->key.first)) {
            node->right = right_rotation(node->right);
            return left_rotation(node);
        }

        node->height = 1 + std::max(height(node->left), height(node->right));
        return node;
    }

    // Insere um novo nó com a chave k e o valor v na subárvore node.
    // Caso a chave não exista, cria ela, insere na árvore, e incrementa o tamanho da árvore.
    // Se a chave já existir, não insere.
    NodeAVL<K, V>* _insert(NodeAVL<K, V>* node, const K& k, const V& v) {
        if(node == nullptr) {
            size++;
            return new NodeAVL<K, V>(k,v);
        }

        comparisons++;
        if(compare(k, node->key.first)) {
            node->left = _insert(node->left, k, v);
        } else {
            comparisons++;
            if(compare(node->key.first, k)) {
                node->right = _insert(node->right, k, v);
            } else {
                return node;
            }
        }

        return fixupNode(node, k);
    }

    // Remove o nó com a chave k da subárvore node.
    // Caso a chave possua apenas o filho esquerdo, substitui o nó diretamente.
    // Caso ela tenha os dois filhos, substituímos a chave pelo sucessor in-order.
    NodeAVL<K, V>* _remove(NodeAVL<K, V>* node, const K& k) {
        if(node == nullptr) {
            return nullptr;
        }

        comparisons++;
        if(compare(k, node->key.first)) {
            node->left = _remove(node->left, k);
        } else {
            comparisons++;
            if(compare(node->key.first, k)) {
                node->right = _remove(node->right, k);
            } else if(node->right == nullptr) {
                NodeAVL<K, V>* aux = node->left;
                delete node;
                size--;
                return aux;
            } else {
                node->right = remove_successor(node, node->right);
                size--;
            }
        }

        return fixupDeletion(node);
    }

    // Remove o sucessor in-order de node, que está na subárvore successor.
    // Se o successor não tiver filho à esquerda, significa que ele é o menor da subárvore.
    // Copia a chave para node, remove o successor real e retorna o filho direito (caso exista) para manter a árvore conectada.
    NodeAVL<K, V>* remove_successor(NodeAVL<K, V>* node, NodeAVL<K, V>* successor) {
        if(successor->left == nullptr) {
            node->key = successor->key;
            NodeAVL<K, V>* aux = successor->right;
            delete successor;
            return aux;
        } else {
            successor->left = remove_successor(node, successor->left);
        }

        return fixupDeletion(successor);
    }

    NodeAVL<K, V>* fixupDeletion(NodeAVL<K, V>* node) {
        int bal = balance(node);
        if(node == nullptr) {
            return nullptr;
        }

        if(bal < -1 && balance(node->left) <= 0) {
            return right_rotation(node);
        }
        if(bal < -1 && balance(node->left) > 0) {
            node->left = left_rotation(node->left);
            return right_rotation(node);
        }
        if(bal > 1 && balance(node->right) >= 0) {
            return left_rotation(node);
        }
        if(bal > 1 && balance(node->right) < 0) {
            node->right = right_rotation(node->right);
            return left_rotation(node);
        }

        return node;
    }

    // Busca o nó com chave k na subárvore node.
    // Retorna o nó encontrado, ou nullptr caso a chave não exista.
    NodeAVL<K, V>* _search(NodeAVL<K, V>* node, const K& k) {
        if(node == nullptr) {
            return nullptr;
        }

        comparisons++;
        if(compare(k, node->key.first)) {
            return _search(node->left, k);
        }

        comparisons++;
        if(compare(node->key.first, k)) {
            return _search(node->right, k);
        }

        return node;
    }

public:

    // Construtor (Criação).
    AVLTree()
    : root(nullptr), compare(comparar()), comparisons(0), size(0), rotations(0) {}

    // Destrutor.
    ~AVLTree() {
        _clear(root);
    }

    // Inserção: Adicionar um novo par chave-valor.
    void insert(const K& k, const V& v) {
        root = _insert(root, k, v);
    }

    // Remoção: Remover um par chave-valor usando a chave.
    void remove(const K& k) {
        root = _remove(root, k);
    }

    // Acesso: Recuperar o valor associado a uma chave específica.
    // Acesso == Busca.
    // Retorna uma referência para o valor caso a chave exista.
    // Caso não exista, lança uma exceção.
    V& search(const K& k) {
        NodeAVL<K, V>* node = _search(root, k);
        if(node == nullptr) {
            throw std::out_of_range("Chave não encontrada!");
        }
        return node->key.second;
    }

    // Verificação de existência: Verificar se uma chave existe.
    bool contains(const K& k) {
        return _search(root, k) != nullptr;
    }

    // Atualização: Modificar o valor associado a uma chave existente.
    // Se a chave for encontrada, substituímos o valor correspondente.
    // Caso a chave não exista, lançamos uma exceção.
    void update(const K& k, V& v) {
        NodeAVL<K, V>* node = _search(root, k);
        if(node == nullptr) {
            return;
        }
        node->key.second = v;
    }

    // Limpeza: Remover todos os pares chave-valor.
    void clear() {
        _clear(root);
        root = nullptr;
        size = 0;
    }

    // Tamanho: Obter o número de pares chave-valor.
    int getSize() const {
        return size;
    }

    int getComparisons() const {
        return comparisons;
    }

    int getRotations() const {
        return rotations;
    }

    bool isEmpty() const {
        return size == 0;
    }

    // Iteração: Percorrer os pares de chave-valor.

    using iterator = IteratorAVL<K, V>;

    // Retorna um iterador apontando para o primeiro par ordenado.
    iterator begin() {
        return iterator(root);
    }

    // Retorna um iterador para o sentinela.
    iterator end() {
        return iterator(nullptr);
    }

};

#endif
