/**
 * @title UnionFindTree
 * @brief union_set(x,y) x が入っている集合と y が入っている集合を併合する．
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

struct UnionFind {
    vector<int> par;
    UnionFind(int size) : par(size, -1) {}
    bool unionSet(int x, int y) {
        x = root(x);
        y = root(y);
        if(x != y) {
            if(par[y] < par[x])
                swap(x, y);
            par[x] += par[y];
            par[y] = x;
        }
        return x != y;
    }
    bool same(int x, int y) { return root(x) == root(y); }
    int root(int x) { return par[x] < 0 ? x : par[x] = root(par[x]); }
    int size(int x) { return -par[root(x)]; }
};