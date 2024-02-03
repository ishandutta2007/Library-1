#pragma once
#include <algorithm>
#include <vector>
#include <cassert>
#include <cstdint>
namespace sps {
namespace sps_internal {
using namespace std;
#define ZETA(s, l) \
 if constexpr (!t) A[s + l]+= A[s]; \
 else if constexpr (t == 1) A[s + l]-= A[s]; \
 else if constexpr (t == 2) A[s]+= A[s + l]; \
 else A[s]-= A[s + l]
template <int t, class T> void rec(T A[], int l) {
 if (l > 127) {
  l>>= 1, rec<t>(A, l), rec<t>(A + l, l);
  for (int s= l; s--;) ZETA(s, l);
 } else
  for (int k= 1; k < l; k<<= 1)
   for (int i= 0; i < l; i+= k + k)
    for (int j= 0; j < k; ++j) ZETA(i + j, k);
}
#undef ZETA
/* subset_zeta:   f -> g s.t. g[S] = sum_{T subseteq S} f[T]  O(n 2^n) */
template <class T> void subset_zeta(vector<T>& f) { rec<0>(f.data(), f.size()); }
/* supset_zeta:   f -> g s.t. g[S] = sum_{S subseteq T} f[T]  O(n 2^n) */
template <class T> void subset_mobius(vector<T>& f) { rec<1>(f.data(), f.size()); }
/* subset_mobius: f -> g s.t. f[S] = sum_{T subseteq S} g[T]  O(n 2^n) */
template <class T> void supset_zeta(vector<T>& f) { rec<2>(f.data(), f.size()); }
/* supset_mobius: f -> g s.t. f[S] = sum_{S subseteq T} g[T]  O(n 2^n) */
template <class T> void supset_mobius(vector<T>& f) { rec<3>(f.data(), f.size()); }
/* h[S] = sum_{U | T == S} f[U]g[T]  O(n 2^n) */
template <class T> vector<T> or_convolve(vector<T> f, vector<T> g) {
 subset_zeta(f), subset_zeta(g);
 for (int s= f.size(); s--;) f[s]*= g[s];
 return subset_mobius(f), f;
}
/* h[S] = sum_{U & T == S} f[U]g[T]  O(n 2^n) */
template <class T> vector<T> and_convolve(vector<T> f, vector<T> g) {
 supset_zeta(f), supset_zeta(g);
 for (int s= f.size(); s--;) f[s]*= g[s];
 return supset_mobius(f), f;
}
template <int t, class T> void rec_r(T A[], int l, int n, int c= 0) {
 if (l >= (n << 4)) {
  l>>= 1, rec_r<t>(A, l, n, c), rec_r<t>(A + l, l, n, c + 1);
  for (int s= l / n; s--;)
   if constexpr (!t)
    for (int d= 0, e= __builtin_popcount(s) + c + 1; d < e; ++d) A[s * n + d + l]+= A[s * n + d];
   else
    for (int d= __builtin_popcount(s) + c + 1; d < n; ++d) A[s * n + d + l]-= A[s * n + d];
 } else
  for (int k= 1, m= l / n; k < m; k<<= 1)
   for (int i= 0; i < m; i+= k + k)
    for (int j= 0; j < k; ++j)
     if constexpr (!t)
      for (int u= i + j, s= u + k, d= 0, e= __builtin_popcount(s) + c; d < e; ++d) A[s * n + d]+= A[u * n + d];
     else
      for (int u= i + j, s= u + k, d= __builtin_popcount(s) + c; d < n; ++d) A[s * n + d]-= A[u * n + d];
}
template <class T> void rnk_zeta(const T f[], T F[], int n) {
 for (int s= 1 << n; s--;) F[s * (n + 1) + __builtin_popcount(s)]= f[s];
 rec_r<0>(F, (n + 1) << n, n + 1);
}
template <class T> void rnk_mobius(T F[], T f[], int n) {
 rec_r<1>(F, (n + 1) << n, n + 1);
 for (int s= 1 << n; s--;) f[s]= F[s * (n + 1) + __builtin_popcount(s)];
}
template <class T> void cnv_(T A[], const T B[], int n) {
 for (int s= 1 << (n - 1); s--;) {
  T t;
  T *a= A + s * n, *b= B + s * n;
  for (int c= __builtin_popcount(s), d= min(2 * c, n - 1), e; d >= c; a[d--]= t)
   for (t= 0, e= d - c; e <= c; ++e) t+= a[e] * b[d - e];
 }
}
template <class T> void cnv_na(const T f[], const T g[], T h[], int N) {
 for (int s= N, t; s--;)
  for (h[t= s]= f[s] * g[0]; t; --t&= s) h[s]+= f[s ^ t] * g[t];
}
// fg, O(n^2 2^n)
template <class T> vector<T> convolve(const vector<T>& f, const vector<T>& g) {
 const int N= f.size(), n= __builtin_ctz(N);
 assert(!(N & (N - 1))), assert(N == (int)g.size());
 vector<T> h(N);
 if (n < 11) return cnv_na(f.data(), g.data(), h.data(), N), h;
 vector<T> F((n + 1) << n), G((n + 1) << n);
 return rnk_zeta(f.data(), F.data(), n), rnk_zeta(g.data(), G.data(), n), cnv_(F.data(), G.data(), n + 1), rnk_mobius(F.data(), h.data(), n), h;
}
template <class T> void div_na(T f[], const T g[], int N) {
 for (int s= 1; s < N; ++s)
  for (int t= s; t; --t&= s) f[s]-= f[s ^ t] * g[t];
}
// 1/f, "f[empty] = 1" is required, O(n^2 2^n)
template <class T> vector<T> inv(const vector<T>& f) {
 const int N= f.size(), n= __builtin_ctz(N);
 assert(!(N & (N - 1))), assert(f[0] == 1);
 vector<T> h(N);
 return h[0]= 1, div_na(h.data(), f.data(), N), h;
 if (n < 11) return h[0]= 1, div_na(h.data(), f.data(), N), h;
 vector<T> F((n + 1) << n), G((n + 1) << n);
 rnk_zeta(f.data(), G.data(), n);
 for (int s= N; s--;) {
  T *a= F.data() + s * (n + 1), *b= G.data() + s * (n + 1);
  a[0]= 1;
  for (int d= 0, c= __builtin_popcount(s); d++ < n;)
   for (int e= max(0, d - c); e < d; ++e) a[d]-= a[e] * b[d - e];
 }
 return rnk_mobius(F.data(), h.data(), n), h;
}
// f/g, "f[empty] = 1" is required, O(n^2 2^n)
template <class T> vector<T> div(vector<T> f, const vector<T>& g) {
 const int N= f.size(), n= __builtin_ctz(N);
 assert(!(N & (N - 1))), assert(N == (int)g.size()), assert(g[0] == 1);
 if (n < 12) return div_na(f.data(), g.data(), N), f;
 vector<T> F((n + 1) << n), G((n + 1) << n);
 rnk_zeta(f.data(), F.data(), n), rnk_zeta(g.data(), G.data(), n);
 for (int s= N; s--;) {
  T *a= F.data() + s * (n + 1), *b= G.data() + s * (n + 1);
  for (int d= 0, c= __builtin_popcount(s); d++ < n;)
   for (int e= max(0, d - c); e < d; ++e) a[d]-= a[e] * b[d - e];
 }
 return rnk_mobius(F.data(), f.data(), n), f;
}
template <class T, class P> void oncnv_(const T f[], T h[], const P& phi, int n) {
 vector<T> F((n + 1) << n), G((n + 1) << n);
 rnk_zeta(f, F.data(), n), fill_n(G.data(), 1 << n, h[0]);
 T* a= G.data() + (1 << n);
 for (int l= 1 << n; l>>= 1;) phi(l, a[l]= h[0] * f[l]), h[l]= a[l];
 for (int d= 2, s; d <= n; ++d) {
  for (rec<0>(a, 1 << n), a+= (s= 1 << n); s--;)
   if (int c= __builtin_popcount(s); c <= d && d <= 2 * c)
    for (int e= d; e--;) a[s]+= G[e << n | s] * F[s * (n + 1) + d - e];
  for (rec<1>(a, 1 << n), s= 1 << n; s--;)
   if (__builtin_popcount(s) == d) phi(s, a[s]), h[s]= a[s];
   else a[s]= 0;
 }
}
// h[S] = phi(S, sum_{T subsetneq S} h[T]f[S/T] )   O(n^2 2^n)
template <class T, class P> vector<T> semi_relaxed_convolve(const vector<T>& f, T init, const P& phi) {
 const int N= f.size(), n= __builtin_ctz(N);
 assert(!(N & (N - 1)));
 vector<T> h(N);
 if (h[0]= init; n < 12) {
  for (int s= 1, t; s < N; phi(s, h[s]), ++s)
   for (t= s; t; --t&= s) h[s]+= h[s ^ t] * f[t];
 } else oncnv_(f.data(), h.data(), phi, n);
 return h;
}
// h[S] = phi(S, 1/2 sum_{empty neq T subseteq S} h[T]h[S/T] )   O(n^2 2^n)
template <class T, class P> vector<T> self_relaxed_convolve(const P& phi, int n) {
 const int e= min(n, 12);
 int i= 0, l= 1;
 vector<T> f(1 << n);
 for (int u= 1; i < e; l<<= 1, ++i)
  for (int s= 0; s < l; phi(u, f[u]), ++s, ++u)
   for (int t= s; t; --t&= s) f[u]+= f[u ^ t] * f[t];
 for (; i < n; l<<= 1, ++i)
  oncnv_(
      f.data(), f.data() + l, [&](int s, T& x) { phi(s | l, x); }, i);
 return f;
}
// exp(f) , "f[empty] = 0" is required,  O(n^2 2^n)
template <class T> vector<T> exp(const vector<T>& f) {
 const int N= f.size(), n= __builtin_ctz(N), e= min(N, 1 << 11);
 assert(!(N & (N - 1))), assert(f[0] == 0);
 vector<T> h(N);
 int i= 0, l= 1;
 for (h[0]= 1; l < e; l<<= 1, ++i) cnv_na(h.data(), f.data() + l, h.data() + l, l);
 if (l < N) {
  vector<T> F(n << (n - 1)), G(n << (n - 1));
  for (; l < N; l<<= 1, ++i) fill_n(F.data(), (i + 1) << i, 0), fill_n(G.data(), (i + 1) << (i - 1), 0), rnk_zeta(h.data(), F.data(), i), rnk_zeta(f.data() + l, G.data(), i), cnv_(F.data(), G.data(), i + 1), rnk_mobius(F.data(), h.data() + l, i);
 }
 return h;
}
// log(f) , "f[empty] = 1" is required,  O(n^2 2^n)
template <class T> vector<T> log(const vector<T>& f) {
 const int N= f.size(), n= __builtin_ctz(N);
 assert(!(N & (N - 1))), assert(f[0] == 1);
 vector<T> h(N);
 int i= n - 1, l= N >> 1;
 if (i > 11) {
  vector<T> F(n << (n - 1)), G(n << (n - 1));
  rnk_zeta(f.data(), G.data(), n - 1);
  for (; i > 11; l>>= 1, --i) {
   fill_n(F.data(), (i + 1) << i, 0), rnk_zeta(f.data() + l, F.data(), i);
   for (int s= l; s--;) {
    T *a= F.data() + s * (i + 1), *b= G.data() + s * n;
    for (int d= 0, c= __builtin_popcount(s); d++ < i;)
     for (int e= max(0, d - c); e < d; ++e) a[d]-= a[e] * b[d - e];
   }
   rnk_mobius(F.data(), h.data() + l, i);
  }
 }
 for (; l; l>>= 1) copy_n(f.data() + l, l, h.data() + l), div_na(h.data() + l, f.data(), l);
 return h;
}
// F(f) =  sum_i F_i f^i/i! , "f[empty] = 0" is required, O(n^2 2^n)
template <class T> vector<T> egf_comp(const vector<T>& F, const vector<T>& f) {
 const int N= f.size(), n= __builtin_ctz(N), e= min(N, 1 << 11);
 assert(!(N & (N - 1))), assert(f[0] == 0);
 vector<T> h(N);
 T* b= h.data() + N;
 for (int i= n - F.size(); i++ < n;) h[N - (1 << i)]= F[n - i];
 int i= 0, l= 1;
 for (; l < e; l<<= 1, ++i)
  for (int j= N >> 1; j >= l; j>>= 1) cnv_na(b - j, f.data() + l, b - j - j + l, l);
 if (l < N) {
  vector<T> A(n << (n - 1)), B(n << (n - 1));
  for (; l < N; l<<= 1, ++i) {
   fill_n(B.data(), (i + 1) << i, 0), rnk_zeta(f.data() + l, B.data(), i);
   for (int j= N >> 1; j >= l; j>>= 1) fill_n(A.data(), (i + 1) << i, 0), rnk_zeta(b - j, A.data(), i), cnv_(A.data(), B.data(), i + 1), rnk_mobius(A.data(), b - j - j + l, i);
  }
 }
 return h;
}
// P(f) = sum_{i=0}^m P_i f^i ,  O(n^2 2^n)
template <class T> vector<T> poly_comp(vector<T> P, vector<T> f) {
 const int N= f.size(), n= __builtin_ctz(N);
 assert(!(N & (N - 1)));
 vector<T> F(n + 1);
 for (int j= 0, e= P.size();; ++j, --e) {
  for (int i= e; i--;) (F[j]*= f[0])+= P[i];
  if (j == n || e <= 1) break;
  for (int i= 1; i < e; ++i) P[i - 1]= P[i] * i;
 }
 return f[0]= 0, egf_comp(F, f);
}
template <class T> egf_(T* b, T* h, int M) {
 T *a, d;
 vector<T> c(n + 1);
 if (int i= __builtin_ctz(M); i > 10) {
  vector<T> F((i + 1) << i), G((i + 1) << i);
  for (int m, s; i > 10; fill_n(F.data(), (i + 1) << i, 0), rnk_zeta(h, F.data(), i), cnv_(F.data(), G.data(), i + 1), rnk_mobius(F.data(), h, i), b-= (l>>= 1), --i)
   for (fill_n(G.data(), (i + 1) << i, 0), rnk_zeta(b, G.data(), i), m= M; m > l; m>>= 1)
    for (a= h + (m - l), fill_n(F.data(), (i + 1) << i, 0), rnk_zeta(a + m - l, F.data(), i), cnv_(F.data(), G.data(), i + 1), rec_r<1>(F.data(), (i + 1) << i, i + 1), s= l; s--;) a[s]+= F[s * (i + 1) + __builtin_popcount(s)];
 }
 for (; l; cnv_na(h, b, h, l), b-= (l>>= 1))
  for (int m= M, s, t; m > l; m>>= 1)
   for (a= h + (m - l), d= a + (m - l), s= l; s--;)
    for (a[t= s]+= d[s] * b[0]; t; --t&= s) a[s]+= d[s ^ t] * b[t];
 for (int i= 1; i <= n; ++i) c[i]= h[(1 << (n - i)) - 1];
 return c
}
// [X^{[n]}] f^k/k! for k=0,1,...,n , O(n^2 2^n)
template <class T> vector<T> egf_T(vector<T> f) {
 const int N= f.size(), n= __builtin_ctz(N);
 assert(!(N & (N - 1)));
 if (n == 0) return {1};
 if (n == 1) return {0, f[1]};
 return egf_(f.data() + (N >> 2), f.data() + (N >> 1), N >> 2);
}
// [X^{[n]}] f^k/k! g for k=0,1,...,n , O(n^2 2^n)
template <class T> vector<T> egf_T(const vector<T>& f, vector<T> g) {
 const int N= f.size(), n= __builtin_ctz(N);
 assert(!(N & (N - 1)));
 if (n == 0) return {g[1]};
 return egf_(f.data() + (N >> 1), g.data(), N >> 1);
}
}
using sps_internal::subset_zeta, sps_internal::subset_mobius, sps_internal::supset_zeta, sps_internal::supset_mobius, sps_internal::or_convolve, sps_internal::and_convolve, sps_internal::convolve, sps_internal::semi_relaxed_convolve, sps_internal::self_relaxed_convolve, sps_internal::inv, sps_internal::div, sps_internal::exp, sps_internal::log, sps_internal::egf_comp, sps_internal::poly_comp, sps_internal::egf_T;
}