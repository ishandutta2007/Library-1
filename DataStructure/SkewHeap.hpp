/**
 * @title Skew-Heap
 * @category データ構造
 * @brief マージできるヒープ
 * @brief top: O(1), pop, push, merge: O(logN)
 * @brief add(v): 全体に作用素vを適用
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <typename T, typename E = T>
struct SkewHeap {
  using C = function<bool(T, T)>;
  using G = function<T(T, E)>;
  using H = function<E(E, E)>;
  C comp;
  G g;
  H h;
  E ei;
  struct Node {
    Node *ch[2];
    T key;
    E lazy;
    Node() {}
    Node(T k, E laz) : ch{nullptr, nullptr}, key(k), lazy(laz) {}
  } * root;

 private:
  void propagate(Node *a) {
    if (a->lazy != ei) {
      a->key = g(a->key, a->lazy);
      if (a->ch[0]) a->ch[0]->lazy = h(a->ch[0]->lazy, a->lazy);
      if (a->ch[1]) a->ch[1]->lazy = h(a->ch[1]->lazy, a->lazy);
      a->lazy = ei;
    }
  }
  Node *merge(Node *a, Node *b) {
    if (!a || !b) return a ? a : b;
    propagate(a);
    propagate(b);
    if (comp(a->key, b->key)) swap(a, b);
    a->ch[1] = merge(b, a->ch[1]);
    swap(a->ch[0], a->ch[1]);
    return a;
  }

 public:
  SkewHeap(
      C c = [](const T &a, const T &b) { return a < b; } /* max heap */,
      G g = [](const T &a, const E &b) { return a + b; },
      H h = [](const E &a, const E &b) { return a + b; }, E ei = 0)
      : comp(c), g(g), h(h), ei(ei), root(nullptr) {}
  void push(T key) {
    Node *n = new Node(key, ei);
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
  void add(E v) { root->lazy = h(root->lazy, v); }
  void merge(SkewHeap x) { root = merge(root, x.root); }
};