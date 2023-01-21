#pragma once
#include <vector>
#include <type_traits>
#include <algorithm>
#include <numeric>
#include <cassert>
template <class K> class LUDecomposition {
 using Mat= std::vector<std::vector<K>>;
 Mat dat;
 std::vector<std::size_t> perm, piv;
 bool sgn;
 static bool is_zero(K x) {
  if constexpr (std::is_floating_point_v<K>) return std::abs(x) < 1e-8;
  else return x == K(0);
 }
public:
 LUDecomposition(const Mat &A): dat(A), perm(A.size()), sgn(false) {
  std::size_t rows= A.size(), cols= A[0].size();
  std::iota(perm.begin(), perm.end(), 0);
  for (std::size_t c= 0, psz= 0; c != cols && psz != rows; c++) {
   auto pos= psz;
   if constexpr (std::is_floating_point_v<K>) {
    for (std::size_t r= psz + 1; r < rows; r++)
     if (std::abs(dat[pos][c]) < std::abs(dat[r][c])) pos= r;
   } else if (is_zero(dat[pos][c])) {
    for (std::size_t r= psz + 1; r < rows; r++)
     if (!is_zero(dat[r][c])) pos= r, r= rows;
   }
   if (is_zero(dat[pos][c])) continue;
   if (pos != psz) sgn= !sgn, std::swap(perm[pos], perm[psz]), dat[pos].swap(dat[psz]);
   for (std::size_t r= psz + 1; r < rows; r++) {
    auto m= dat[r][c] / dat[psz][c];
    dat[r][c]= 0, dat[r][psz]= m;
    for (std::size_t i= c + 1; i < cols; i++) dat[r][i]-= dat[psz][i] * m;
   }
   piv.emplace_back(c), psz++;
  }
 }
 std::size_t rank() const { return piv.size(); }
 bool isregular() const { return rank() == dat.size() && rank() == dat[0].size(); }
 K det() const {
  assert(dat.size() == dat[0].size());
  K d= sgn ? -1 : 1;
  for (std::size_t i= 0; i != dat.size(); i++) d*= dat[i][i];
  return d;
 }
 std::vector<std::vector<K>> kernel() const {
  std::size_t cols= dat[0].size();
  std::vector<std::vector<K>> ker(cols - rank(), std::vector<K>(cols));
  for (std::size_t c= 0, i= 0; c < cols; c++) {
   if (i < piv.size() && piv[i] == c) {
    i++;
    continue;
   }
   ker[c - i][c]= 1;
   for (std::size_t r= i; r--;) ker[c - i][r]= -dat[r][c];
   for (std::size_t j= i, k; j--;) {
    auto x= ker[c - i][j] / dat[j][k= piv[j]];
    ker[c - i][j]= 0, ker[c - i][k]= x;
    for (std::size_t r= j; r--;) ker[c - i][r]-= dat[r][k] * x;
   }
  }
  return ker;
 }
 std::vector<K> linear_equations(const std::vector<K> &b) const {
  std::size_t rows= dat.size(), cols= dat[0].size();
  assert(rows <= b.size());
  std::vector<K> y(rows), x(cols);
  for (std::size_t c= 0; c < rows; c++)
   if (y[c]+= b[perm[c]]; c < cols)
    for (std::size_t r= c + 1; r < rows; r++) y[r]-= y[c] * dat[r][c];
  for (std::size_t i= rank(); i < rows; i++)
   if (!is_zero(y[i])) return {};  // no solution
  for (std::size_t i= rank(); i--;) {
   x[piv[i]]= y[i] / dat[i][piv[i]];
   for (std::size_t r= i; r--;) y[r]-= x[piv[i]] * dat[r][piv[i]];
  }
  return x;
 }
 Mat inverse_matrix() const {
  if (!isregular()) return {};  // no solution
  assert(dat.size() == dat[0].size());
  std::vector<K> b(dat.size());
  Mat ret;
  for (std::size_t i= 0; i < dat.size(); b[i++]= 0) b[i]= 1, ret.emplace_back(linear_equations(b));
  for (std::size_t i= 0; i < dat.size(); i++)
   for (std::size_t j= 0; j < i; j++) std::swap(ret[i][j], ret[j][i]);
  return ret;
 }
};
template <> class LUDecomposition<bool> {
public:
 using Mat= std::vector<std::vector<bool>>;
 using u128= __uint128_t;
 const std::size_t rows, cols, row128, col128;
 std::vector<std::vector<u128>> tdat;
 std::vector<std::size_t> perm, piv;
public:
 LUDecomposition(const Mat &A): rows(A.size()), cols(A[0].size()), row128((rows + 127) >> 7), col128((cols + 127) >> 7), tdat(cols, std::vector<u128>(row128)), perm(rows) {
  std::vector dat(rows, std::vector<u128>(col128));
  for (std::size_t i= rows; i--;)
   for (std::size_t j= cols; j--;) dat[i][j >> 7]|= u128(A[i][j]) << (j & 127);
  std::iota(perm.begin(), perm.end(), 0);
  for (std::size_t c= 0, psz= 0; c < cols && psz < rows; c++) {
   auto pos= psz, a= c >> 7, b= c & 127;
   for (std::size_t r= psz; r < rows; r++)
    if ((dat[r][a] >> b) & 1) pos= r, r= rows;
   if (!((dat[pos][a] >> b) & 1)) continue;
   if (pos != psz) std::swap(perm[pos], perm[psz]), dat[pos].swap(dat[psz]);
   for (std::size_t r= psz + 1; r < rows; r++) {
    if ((dat[r][a] >> b) & 1) {
     for (auto i= a + 1; i < col128; i++) dat[r][i]^= dat[psz][i];
     dat[r][a]^= dat[psz][a] & u128(-(u128(1) << b));
     dat[r][psz >> 7]|= u128(1) << (psz & 127);
    }
   }
   piv.push_back(c), psz++;
  }
  for (std::size_t j= cols; j--;)
   for (std::size_t i= rows; i--;) tdat[j][i >> 7]|= u128((dat[i][j >> 7] >> (j & 127)) & 1) << (i & 127);
 }
 std::size_t rank() const { return piv.size(); }
 bool isregular() const { return rank() == rows && rank() == cols; }
 bool det() const { return isregular(); }
 std::vector<std::vector<bool>> kernel() const {
  std::vector ker(cols - rank(), std::vector<u128>(col128));
  for (std::size_t c= 0, i= 0; c < cols; c++) {
   if (i < piv.size() && piv[i] == c) {
    i++;
    continue;
   }
   ker[c - i][c >> 7]|= u128(1) << (c & 127);
   for (std::size_t r= i >> 7; r--;) ker[c - i][r]= tdat[c][r];
   ker[c - i][i >> 7]= tdat[c][i >> 7] & ((u128(1) << (i & 127)) - 1);
   for (std::size_t j= i; j--;) {
    if (std::size_t k= piv[j]; (ker[c - i][j >> 7] >> (j & 127)) & 1) {
     for (std::size_t r= j >> 7; r--;) ker[c - i][r]^= tdat[k][r];
     ker[c - i][j >> 7]^= tdat[k][j >> 7] & ((u128(1) << (j & 127)) - 1);
     ker[c - i][k >> 7]|= u128(1) << (k & 127);
    }
   }
  }
  std::vector ret(cols - rank(), std::vector<bool>(cols));
  for (std::size_t i= cols - rank(); i--;)
   for (std::size_t j= cols; j--;) ret[i][j]= ker[i][j >> 7] << (j & 127);
  return ret;
 }
 std::vector<bool> linear_equations(const std::vector<bool> &b) const {
  assert(rows <= b.size());
  std::vector<u128> y(row128);
  std::vector<bool> x(cols);
  for (std::size_t c= 0; c < rows; c++) {
   y[c >> 7]^= u128(b[perm[c]]) << (c & 127);
   if ((c < cols) & (y[c >> 7] >> (c & 127)))
    if (std::size_t a= (c + 1) >> 7; a < row128) {
     for (std::size_t r= a + 1; r < row128; r++) y[r]^= tdat[c][r];
     y[a]^= tdat[c][a] & u128(-(u128(1) << ((c + 1) & 127)));
    }
  }
  if ((rank() >> 7) < row128 && (y[rank() >> 7] >> (rank() & 127))) return {};  // no solution
  for (std::size_t r= (rank() >> 7) + 1; r < row128; r++)
   if (y[r]) return {};  // no solution
  for (std::size_t i= rank(); i--;)
   if (std::size_t k= piv[i]; (x[k]= (y[i >> 7] >> (i & 127)) & 1)) {
    for (std::size_t r= i >> 7; r--;) y[r]^= tdat[k][r];
    y[i >> 7]^= tdat[k][i >> 7] & ((u128(1) << (i & 127)) - 1);
   }
  return x;
 }
 Mat inverse_matrix() const {
  if (!isregular()) return {};  // no solution
  std::vector<bool> b(rows);
  Mat ret;
  for (std::size_t i= 0; i < rows; b[i++]= 0) b[i]= 1, ret.emplace_back(linear_equations(b));
  for (std::size_t i= 0; i < rows; i++)
   for (std::size_t j= 0; j < i; j++) {
    bool tmp= ret[i][j];
    ret[i][j]= ret[j][i];
    ret[j][i]= tmp;
   }
  return ret;
 }
};