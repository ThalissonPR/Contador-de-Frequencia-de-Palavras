#ifndef RBTREE_HPP
#define RBTREE_HPP
#include "NodeRB.hpp"
#include "IteratorRB.hpp"
#include <string>
#include <iostream>
#include <utility>

template <typename K, typename V, typename comparar = std::less<K>>
class RBTree {
private:
    NodeRB<K, V>* root;
    NodeRB<K, V>* nil;
    comparar compare;
    int comparisons;
    int size;
    int rotations;

    void left_rotation(NodeRB<K, V>* node) {
        rotations++;
        NodeRB<K, V>* y = node->right;
        node->right = y->left;

        if(y->left != nil) {
            y->left->parent = node;
        }

        y->parent = node->parent;
        if(node->parent == nil) {
            root = y;
        } else if(node == node->parent->left) {
            node->parent->left = y;
        } else {
            node->parent->right = y;
        }

        y->left = node;
        node->parent = y;
    }

    void right_rotation(NodeRB<K, V>* node) {
        rotations++;
        NodeRB<K, V>* y = node->left;
        node->left = y->right;

        if(y->right != nil) {
            y->right->parent = node;
        }

        y->parent = node->parent;
        if(node->parent == nil) {
            root = y;
        } else if(node == node->parent->left) {
            node->parent->left = y;
        } else {
            node->parent->right = y;
        }

        y->right = node;
        node->parent = y;
    }

    // Caso 1: tio vermelho.
    // Caso 2: tio preto e node forma uma espécie de zig-zag com o seu pai.
    // Caso 3: tio preto e node forma uma linha reta com o pai.
    void fixupInsert(NodeRB<K, V>* node) {
        while(node->parent->color == true) {
            NodeRB<K, V>* parent = node->parent;
            NodeRB<K, V>* grandparent = node->parent->parent;

            if(parent == grandparent->left) {
                NodeRB<K, V>* uncle = grandparent->right;
                if(uncle->color == true) { // Caso 1
                    parent->color = false;
                    uncle->color = false;
                    grandparent->color = true;
                    node = grandparent;
                } else {
                    if(node == parent->right) { // Caso 2
                        node = parent;
                        left_rotation(node);
                        parent = node->parent;
                        grandparent = parent->parent;
                    }
                    // Caso 3
                    parent->color = false;
                    grandparent->color = true;
                    right_rotation(grandparent);
                }
            } else { // Espelho
                NodeRB<K, V>* uncle = grandparent->left;
                if(uncle->color == true) { // Caso 1
                    parent->color = false;
                    uncle->color = false;
                    grandparent->color = true;
                    node = grandparent;
                } else {
                    if(node == parent->left) { // Caso 2
                        node = parent;
                        right_rotation(node);
                        parent = node->parent;
                        grandparent = parent->parent;
                    }
                    // Caso 3
                    parent->color = false;
                    grandparent->color = true;
                    left_rotation(grandparent);
                }
            }
        }
        root->color = false;
    }

    // Insere um novo nó na árvore rubro-negra.
    // Se a chave já existir, o novo nó é descartado sem inserir nada.
    void _insert(NodeRB<K, V>* node) {
        NodeRB<K, V>* aux = root;
        NodeRB<K, V>* auxPai = nil;

        while(aux != nil) {
            auxPai = aux;
            comparisons++;
            if(compare(node->key.first, aux->key.first)) {
                aux = aux->left;
            } else {
                comparisons++;
                if(compare(aux->key.first, node->key.first)) {
                    aux = aux->right;
                } else {
                    delete node;
                    return;
                }
            }
        }

        node->parent = auxPai;
        if(auxPai == nil) {
            root = node;
        } else {
            comparisons++;
            if(compare(node->key.first, auxPai->key.first)) {
                auxPai->left = node;
            } else {
                auxPai->right = node;
            }
        }

        size++;
        fixupInsert(node);
    }

    // Retorna o ponteiro para o nó que contém a chave, ou nil caso não seja encontrado.
    NodeRB<K, V>* _search(NodeRB<K, V>* node, const K& key) {
        if(node == nil) {
            return node;
        }

        comparisons += 2;
        if(!compare(key, node->key.first) && !compare(node->key.first, key)) {
            return node;
        }

        comparisons++;
        if(compare(key, node->key.first)) {
            return _search(node->left, key);
        }

        return _search(node->right, key);
    }

    NodeRB<K, V>* minimum(NodeRB<K, V>* node) {
        while(node->left != nil) {
            node = node->left;
        }
        return node;
    }

    // Caso 1: irmão é vermelho.
    // Caso 2: irmão é preto e seus dois filhos são pretos
    // Caso 3: irmão é preto e seu filho mais distante do nó é preto
    // Caso 4: irmão é preto e seu filho mais distante do nó é vermelho
    void fixupRemove(NodeRB<K, V>* node) {
        while(node != root && node->color == false) {
            if(node == node->parent->left) {
                NodeRB<K, V>* irmao = node->parent->right;

                if(irmao->color == true) { // Caso 1
                    irmao->color = false;
                    node->parent->color = true;
                    left_rotation(node->parent);
                    irmao = node->parent->right;
                }

                if(irmao->left->color == false && irmao->right->color == false) { // Caso 2
                    irmao->color = true;
                    node = node->parent;
                } else {
                    if(irmao->right->color == false) { // Caso 3
                        irmao->left->color = false;
                        irmao->color = true;
                        right_rotation(irmao);
                        irmao = node->parent->right;
                    }
                    // Caso 4
                    irmao->color = node->parent->color;
                    node->parent->color = false;
                    irmao->right->color = false;
                    left_rotation(node->parent);
                    node = root;
                }
            } else { // Espelho
                NodeRB<K, V>* irmao = node->parent->left;

                if(irmao->color == true) { // Caso 1
                    irmao->color = false;
                    node->parent->color = true;
                    right_rotation(node->parent);
                    irmao = node->parent->left;
                }

                if(irmao->right->color == false && irmao->left->color == false) { // Caso 2
                    irmao->color = true;
                    node = node->parent;
                } else {
                    if(irmao->left->color == false) { // Caso 3
                        irmao->right->color = false;
                        irmao->color = true;
                        left_rotation(irmao);
                        irmao = node->parent->left;
                    }
                    // Caso 4
                    irmao->color = node->parent->color;
                    node->parent->color = false;
                    irmao->left->color = false;
                    right_rotation(node->parent);
                    node = root;
                }
            }
        }
        node->color = false;
    }
    
    // Remove um nó na árvore rubro-negra.
    // Se o nó a ser removido tem dois filhos, troca ele pelo sucessor in-order.
    void _remove(NodeRB<K, V>* node) {
        NodeRB<K, V>* remover;
        NodeRB<K, V>* filho;

        if(node->right == nil || node->left == nil) { // Caso com até 1 filho
            remover = node;
        } else { // Caso com 2 filhos
            remover = minimum(node->right);
        }

        if(remover->right != nil) {
            filho = remover->right;
        } else {
            filho = remover->left;
        }

        filho->parent = remover->parent;
        if(remover->parent == nil) {
            root = filho;
        } else if(remover == remover->parent->right) {
            remover->parent->right = filho;
        } else {
            remover->parent->left = filho;
        }

        if(remover != node) { // Copia o valor do nó caso seja necessário
            node->key = remover->key;
        }

        if(remover->color == false) { // Corrigir caso o nó seja preto
            fixupRemove(filho);
        }

        delete remover;
    }

    void _clear(NodeRB<K, V>* node) {
        if(node == nil) {
            return;
        }
        _clear(node->left);
        _clear(node->right);
        delete node;
    }


public:

    // Construtor (Criação).
    RBTree()
    : size(0), comparisons(0), rotations(0), root(nullptr), nil(new NodeRB<K, V>(K(), V(), false)) {
        root = nil;
    }

    // Destrutor.
    ~RBTree() {
        _clear(root);
        delete nil;
    }

    // Inserção: Adicionar um novo par chave-valor.
    void insert(const K& key, const V& value) {
        NodeRB<K, V>* node = new NodeRB<K, V>(key, value, true, nil, nil, nil);
        _insert(node);
    }

    // Remoção: Remover um par chave-valor usando a chave key.
    void remove(const K& key) {
        NodeRB<K, V>* node = _search(root, key);
        if(node != nil) {
            _remove(node);
            size--;
        }
    }

    // Acesso: Recuperar o valor associado a uma chave k.
    // Acesso == Busca.
    V& search(const K& k) {
        NodeRB<K, V>* node = _search(root, k);
        if(node != nil) {
            return node->key.second;
        }
        throw std::out_of_range("Chave não encontrada!");
    }

    // Verificação de existência: Verificar se uma chave existe na estrutura.
    bool contains(const K& k) {
        return _search(root, k) != nil;
    }

    // Atualização: Modificar o valor associado a uma chave existente.
    void update(const K& k, V& v) {
        NodeRB<K, V>* node = _search(root, k);
        if(node != nil) {
            node->key.second = v;
        }
    }

    // Limpeza: Remover todos os pares chave-valor.
    void clear() {
        _clear(root);
        root = nil;
        size = 0;
    }

    bool isEmpty() const {
        return size == 0;
    }

    // Tamanho: Obter o número de pares chave-valor.
    int getSize() const {
        return size;
    }

    int getRotations() const {
        return rotations;
    }

    int getComparisons() const {
        return comparisons;
    }

    // Iteração: Percorrer os pares de chave-valor.

    using iterator = IteratorRB<K, V>;

    // Retorna um iterador para o menor elemento da árvore.
    iterator begin() {
        NodeRB<K, V>* node = root;
        while(node->left != nil) {
            node = node->left;
        }
        return iterator(node, nil);
    }

    // Retorna um iterador para o fim da árvore.
    iterator end() {
        return iterator(nil, nil);
    }

};

#endif
