#pragma once
#include <bits/stdc++.h>
/**
 * @title Skew-Heap
 * @category データ構造
 * @brief マージできるヒープ
 * @brief top: O(1), pop, push, merge: O(logN)
 * @brief add(v): 全体に作用素vを適用
 */

// BEGIN CUT HERE

template <typename T>
struct Op_RaddQ {
  using E = T;
  static E ei() { return 0; }
  static T g(const T &l, const E &r) { return l + r; }
  static E h(const E &l, const E &r) { return l + r; }
};

template <typename T, typename Compare = less<T>, typename Op = Op_RaddQ<T>>
struct SkewHeap {
  using E = typename Op::E;
  struct Node {
    Node *ch[2];
    T key;
    E lazy;
    Node() {}
    Node(T k, E laz) : ch{nullptr, nullptr}, key(k), lazy(laz) {}
  } * root;

 private:
  void propagate(Node *a) {
    if (a->lazy != Op::ei()) {
      a->key = Op::g(a->key, a->lazy);
      if (a->ch[0]) a->ch[0]->lazy = Op::h(a->ch[0]->lazy, a->lazy);
      if (a->ch[1]) a->ch[1]->lazy = Op::h(a->ch[1]->lazy, a->lazy);
      a->lazy = Op::ei();
    }
  }
  Node *merge(Node *a, Node *b) {
    if (!a || !b) return a ? a : b;
    propagate(a);
    propagate(b);
    if (Compare()(a->key, b->key)) std::swap(a, b);
    a->ch[1] = merge(b, a->ch[1]);
    std::swap(a->ch[0], a->ch[1]);
    return a;
  }

 public:
  /* max heap */
  SkewHeap() : root(nullptr) {}
  void push(T key) {
    Node *n = new Node(key, Op::ei());
    root = merge(root, n);
  }
  T pop() {
    propagate(root);
    T ret = root->key;
    Node *temp = root;
    root = merge(root->ch[0], root->ch[1]);
    delete temp;
    return ret;
  }
  T top() {
    propagate(root);
    return root->key;
  }
  bool empty() { return !root; }
  void add(E v) { root->lazy = Op::h(root->lazy, v); }
  void merge(SkewHeap x) { root = merge(root, x.root); }
};