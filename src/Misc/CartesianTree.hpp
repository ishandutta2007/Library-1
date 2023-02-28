#pragma once
#include <vector>
#include <array>
class CartesianTree {
 std::vector<std::array<int, 2>> rg, ch;
 std::vector<int> par;
 int rt;
public:
 template <class T> CartesianTree(const std::vector<T>& a, bool is_min= 1): rg(a.size()), ch(a.size(), std::array{-1, -1}), par(a.size(), -1) {
  const int n= a.size();
  auto comp= [&](int l, int r) { return (is_min ? a[l] < a[r] : a[l] > a[r]) || (a[l] == a[r] && l < r); };
  std::vector<int> st;
  st.reserve(n);
  for (int i= 0; i < n; rg[i][0]= (st.empty() ? 0 : st.back() + 1), st.push_back(i++))
   for (; !st.empty() && comp(i, st.back()); st.pop_back()) ch[i][0]= st.back();
  st.clear();
  for (int i= n; i--; rg[i][1]= (st.empty() ? n : st.back()), st.push_back(i))
   for (; !st.empty() && comp(i, st.back()); st.pop_back()) ch[i][1]= st.back();
  for (int i= 0; i < n; ++i)
   for (int b= 2; b--;)
    if (ch[i][b] != -1) par[ch[i][b]]= i;
  for (int i= 0; i < n; ++i)
   if (par[i] == -1) rt= i;
 }
 std::array<int, 2> children(int i) const { return ch[i]; }
 int parent(int i) const { return par[i]; }
 int root() const { return rt; }
 // [l,r)
 std::array<int, 2> range(int i) const { return rg[i]; }
};