/**
 * @title 永続配列
 * @category データ構造
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <typename T, int LOG = 3>
struct PersistentArray {
  struct Node {
    T data;
    Node *child[1 << LOG] = {};
    Node() {}
    Node(const T &data) : data(data) {}
  };

 private:
  Node *root;

 public:
  PersistentArray() : root(nullptr) {}
  PersistentArray(size_t n, const T &val = T()) { build(vector<T>(n, val)); }
  PersistentArray(const vector<T> &v) { build(v); }
  const T get(const int &k) const { return get(root, k); }
  T &operator[](const int &k) { return reference_get(k, false); }
  void build(const vector<T> &v) {
    root = nullptr;
    for (int i = 0; i < v.size(); i++) reference_get(i, true) = v[i];
  }

 private:
  T get(Node *t, int k) const {
    if (!t) return T();
    if (k == 0) return t->data;
    return get(t->child[k & ((1 << LOG) - 1)], k >> LOG);
  }
  pair<Node *, T &> reference_get(Node *t, int k, bool destruct = false) {
    t = t ? (destruct ? t : new Node(*t)) : new Node();
    if (k == 0) return {t, t->data};
    auto p = reference_get(t->child[k & ((1 << LOG) - 1)], k >> LOG, destruct);
    t->child[k & ((1 << LOG) - 1)] = p.first;
    return {t, p.second};
  }
  T &reference_get(const int &k, bool destruct = false) {
    auto ret = reference_get(root, k, destruct);
    root = ret.first;
    return ret.second;
  }
};
