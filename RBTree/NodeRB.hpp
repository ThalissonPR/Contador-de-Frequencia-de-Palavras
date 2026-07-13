#ifndef NODERB_HPP
#define NODERB_HPP
#include <utility>
#include <iostream>

template<typename K, typename V>
struct NodeRB {
    std::pair<K, V> key;
    bool color;          // BLACK = false && RED = true
    NodeRB* left;
    NodeRB* right;
    NodeRB* parent;

    NodeRB(const K& k, const V& v, bool c = true, NodeRB* l = nullptr, NodeRB* r = nullptr, NodeRB* p = nullptr)
    : key(std::make_pair(k, v)), color(c), left(l), right(r), parent(p) {}

};

#endif