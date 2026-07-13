#ifndef NODEAVL_HPP
#define NODEAVL_HPP
#include <utility>

template <typename K, typename V>
struct NodeAVL {
    std::pair<K, V> key;
    NodeAVL* left;
    NodeAVL* right;
    int height;

    NodeAVL(K k, V v)
    : key(std::make_pair(k, v)), left(nullptr), right(nullptr), height(1) {}

};

#endif