#pragma once
#include "src/FFT/fps_inv.hpp"
#include "src/FFT/fps_exp.hpp"
#include "src/FFT/convolve.hpp"
template <typename mod_t, std::size_t LM= 1 << 22> std::vector<mod_t> bernoulli(int N) {  // O(N log N)
 std::vector<mod_t> ret(N + 1);
 mod_t fact= ret[0]= 1;
 for (int i= 1; i <= N; i++) ret[i]= ret[i - 1] * get_inv<mod_t, LM>(i + 1);
 ret= inv<mod_t, LM>(ret);
 for (int i= 1; i <= N; fact*= (++i)) ret[i]*= fact;
 return ret;
}
template <typename mod_t, std::size_t LM= 1 << 22> std::vector<mod_t> partition(int N) {  // O(N log N)
 std::vector<mod_t> ret(N + 1, 0);
 for (int k= 1, l= 2; l <= N; l+= 6 * k + 7, k+= 2) ret[l]= -1;
 for (int k= 2, l= 7; l <= N; l+= 6 * k + 7, k+= 2) ret[l]= 1;
 for (int k= 1, l= 1; l <= N; l+= 6 * k + 5, k+= 2) ret[l]= -1;
 for (int k= 2, l= 5; l <= N; l+= 6 * k + 5, k+= 2) ret[l]= 1;
 return ret[0]= 1, inv<mod_t, LM>(ret);
}
template <typename mod_t, std::size_t LM= 1 << 22> std::vector<mod_t> alternating_permutation(int N) {  // O(N log N)
 std::vector<mod_t> f(N + 1), c(N + 1), s(N + 1);
 mod_t fact= f[0]= 1;
 for (int i= 1; i <= N; i++) f[i]= f[i - 1] * get_inv<mod_t, LM>(i);
 for (int i= 0; i <= N; i+= 4) c[i]= f[i];
 for (int i= 1; i <= N; i+= 4) s[i]= f[i];
 for (int i= 2; i <= N; i+= 4) c[i]= -f[i];
 for (int i= 3; i <= N; i+= 4) s[i]= -f[i];
 s[0]+= 1, f= div<mod_t, LM>(s, c);
 for (int i= 1; i <= N; fact*= ++i) f[i]*= fact;
 return f;
}
// enumeration for k = 0,...,K in ∑_{n=0}^{N-1} n^k
template <typename mod_t, std::size_t LM= 1 << 22> std::vector<mod_t> sum_kth_pows(int K, std::uint64_t N) {  // O(K log K)
 std::vector<mod_t> a(K + 1), b(K + 1);
 int i= 1;
 for (a[0]= 1; i <= K; i++) a[i]= a[i - 1] * get_inv<mod_t, LM>(i + 1);
 for (b[0]= N, i= 1; i <= K; i++) b[i]= b[i - 1] * N;
 for (i= 0; i <= K; i++) b[i]*= a[i];
 mod_t fact= 1;
 for (a= div<mod_t, LM>(b, a), i= 1; i <= K; fact*= ++i) a[i]*= fact;
 return a;
}
// enumeration for k = 0,...,K in s(N, N-k)
template <typename mod_t, std::size_t LM= 1 << 22> std::vector<mod_t> stirling_first(int K, std::uint64_t N= 0) {  // O(K log K)
 if (N < K) N= K;
 auto a= sum_kth_pows<mod_t, LM>(K, N);
 for (int i= 1; i <= K; i++) a[i]*= -get_inv<mod_t, LM>(i);
 return a[0]= 0, exp<mod_t, LM>(a);
}
// enumeration for k = 0,...,N in S(N, k)
template <typename mod_t, std::size_t LM= 1 << 22> std::vector<mod_t> stirling_second(int N) {  // O(N log N)
 std::vector<mod_t> a(N + 1, 1), b(N + 1);
 for (int i= 1; i <= N; i++) a[i]= a[i - 1] * get_inv<mod_t, LM>(i);
 for (int i= 0; i <= N; i+= 2) b[i]= a[i];
 for (int i= 1; i <= N; i+= 2) b[i]= -a[i];
 for (int i= 0; i <= N; i++) a[i]*= mod_t(i).pow(N);
 return a= convolve<mod_t, LM>(a, b), a.resize(N + 1), a;
}