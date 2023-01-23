#pragma once
#include "src/FFT/sample_points_shift.hpp"
// M(0)*M(1)*...*M(k)
template <class mod_t, std::size_t LM= 1 << 20> std::vector<std::vector<mod_t>> polynomial_matrix_prod(const std::vector<std::vector<std::vector<mod_t>>> &m, std::uint64_t k) {
 using Mat= std::vector<std::vector<mod_t>>;
 using Poly= std::vector<mod_t>;
 const int n= m.size();
 assert(n > 0), assert(n == (int)m[0].size());
 if (!k) {
  Mat ret(n, std::vector<mod_t>(n, mod_t(0)));
  for (int i= n; i--;) ret[i][i]= mod_t(1);
  return ret;
 }
 auto shift= [n](const std::vector<Mat> &G, mod_t x, int d) {
  std::vector<Mat> H(d, Mat(n, std::vector<mod_t>(n)));
  for (int i= n; i--;)
   for (int j= n; j--;) {
    Poly g(G.size());
    for (int l= G.size(); l--;) g[l]= G[l][i][j];
    g= sample_points_shift<mod_t, LM>(g, x, d);
    for (int l= d; l--;) H[l][i][j]= g[l];
   }
  return H;
 };
 auto mult= [n](const Mat &A, const Mat &B) {
  Mat ret(n, std::vector<mod_t>(n, mod_t(0)));
  for (int i= n; i--;)
   for (int k= n; k--;)
    for (int j= n; j--;) ret[i][j]+= A[i][k] * B[k][j];
  return ret;
 };
 auto eval= [](const Poly &f, mod_t x) {
  mod_t ret= mod_t(0);
  for (int l= f.size(); l--;) ret*= x, ret+= f[l];
  return ret;
 };
 int deg= 1, w;
 for (int i= n; i--;)
  for (int j= n; j--;) deg= std::max(deg, (int)m[i][j].size() - 1);
 std::vector<Mat> G(deg + 1, Mat(n, std::vector<mod_t>(n)));
 const int v= pw2((std::sqrt(std::max<std::uint64_t>(1, (k + deg - 1) / deg))));
 mod_t tmp, iv= mod_t(1) / v;
 for (int d= deg + 1, i; d--;)
  for (tmp= mod_t(v) * d, i= n; i--;)
   for (int j= n, l; j--;) G[d][i][j]= eval(m[i][j], tmp);
 for (tmp= v, w= 1; w < v; w<<= 1) {
  auto G0= shift(G, (tmp * deg * w + v) * iv, w * deg);
  auto G1= shift(G, (tmp * deg * w + v + w) * iv, w * deg);
  for (int i= w * deg; i--;) G0[i]= mult(G1[i], G0[i]);
  G1= shift(G, iv * w, w * deg + 1);
  for (int i= w * deg + 1; i--;) G[i]= mult(G1[i], G[i]);
  std::copy(G0.begin(), G0.end(), std::back_inserter(G));
 }
 Mat ret= G[0];
 const int e= std::min<int>(k / v, G.size());
 for (int i= 1; i < e; i++) ret= mult(G[i], ret);
 for (long long i= e * v; i < k; i++) {
  Mat mt(n, std::vector<mod_t>(n, mod_t(0)));
  for (int j= n; j--;)
   for (int l= n, p; l--;)
    for (tmp= eval(m[j][l], i), p= n; p--;) mt[j][p]+= tmp * ret[l][p];
  ret.swap(mt);
 }
 return ret;
}