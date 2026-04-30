// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1953
// yuki 1953 ８: self-validating sp judge.
// 公式 Python ジャッジを C++ digit DP に直訳。
//   - K = 入力 (1 行目): 「丸の数」のターゲット
//   - user N = ユーザ出力 (整数 1 個)
//   - 各桁の丸の数: 0,4,6,9 → 1, 8 → 2, 他 → 0
//     ただし leading zero の 0 はカウントしない
//   - getK(N) = 1..N (記法上の桁) に現れる丸の総数
//   - N == -1 の時は、[0, 10^18+1] の範囲で getK(v)==K となる v が
//     存在しないことを二分探索で確認
//   - それ以外は getK(N) == K を確認
// 注意: checker 内では Library の Automaton を使わない (verify 対象なので循環回避)
// Args (run-lib.sh testlib convention): input user_output expected
#include <cstdio>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
using i128= __int128;

// numbers in [0, value-of(a)] における桁 id の出現回数
static i128 count_digit(const std::string& a, int id) {
 int n= (int)a.size();
 std::vector<std::vector<i128>> dp(2, std::vector<i128>(20, 0));
 dp[0][0]= 1;
 for(int i= 0; i < n; ++i) {
  std::vector<std::vector<i128>> ndp(2, std::vector<i128>(20, 0));
  int max_tight= a[i] - '0';
  for(int less= 0; less < 2; ++less) {
   int max_d= less ? 9 : max_tight;
   for(int cnt= 0; cnt < 19; ++cnt) {
    if(dp[less][cnt] == 0) continue;
    for(int d= 0; d <= max_d; ++d) {
     int less_= less || (d < max_d);
     int cnt_= cnt + (d == id);
     ndp[less_][cnt_]+= dp[less][cnt];
    }
   }
  }
  dp= std::move(ndp);
 }
 i128 ret= 0;
 for(int less= 0; less < 2; ++less)
  for(int cnt= 0; cnt < 20; ++cnt) ret+= dp[less][cnt] * cnt;
 return ret;
}

// numbers in [0, value-of(a)] における non-leading 0 の出現回数
static i128 count_nonleading_zero(const std::string& a) {
 int n= (int)a.size();
 // dp[less][started][cntZ]
 std::vector<std::vector<std::vector<i128>>> dp(
  2, std::vector<std::vector<i128>>(2, std::vector<i128>(20, 0)));
 dp[0][0][0]= 1;
 for(int i= 0; i < n; ++i) {
  std::vector<std::vector<std::vector<i128>>> ndp(
   2, std::vector<std::vector<i128>>(2, std::vector<i128>(20, 0)));
  int max_tight= a[i] - '0';
  for(int less= 0; less < 2; ++less)
   for(int started= 0; started < 2; ++started) {
    int max_d= less ? 9 : max_tight;
    for(int cntZ= 0; cntZ < 19; ++cntZ) {
     if(dp[less][started][cntZ] == 0) continue;
     for(int d= 0; d <= max_d; ++d) {
      int less_= less || (d < max_d);
      int started_= started || (d != 0);
      int cntZ_= started ? (cntZ + (d == 0)) : 0;
      ndp[less_][started_][cntZ_]+= dp[less][started][cntZ];
     }
    }
   }
  dp= std::move(ndp);
 }
 i128 ret= 0;
 for(int less= 0; less < 2; ++less)
  for(int cntZ= 0; cntZ < 20; ++cntZ) ret+= dp[less][1][cntZ] * cntZ;
 return ret;
}

static i128 getK(long long N) {
 if(N < 0) return 0;
 std::string s= std::to_string(N);
 i128 ret= 0;
 for(int d: {4, 6, 8, 8, 9}) ret+= count_digit(s, d);
 ret+= count_nonleading_zero(s);
 return ret;
}

int main(int argc, char** argv) {
 if(argc < 3) { std::fprintf(stderr, "usage: checker input user_output [expected]\n"); return 2; }
 std::ifstream in(argv[1]);
 std::ifstream out(argv[2]);
 if(!in || !out) { std::fprintf(stderr, "open failed\n"); return 2; }

 long long K;
 if(!(in >> K)) { std::fprintf(stderr, "input parse: K\n"); return 2; }

 long long N;
 if(!(out >> N)) { std::fprintf(stderr, "user output empty\n"); return 1; }

 long long extra;
 if(out >> extra) { std::fprintf(stderr, "extra output present\n"); return 1; }

 if(N < -1) { std::fprintf(stderr, "N=%lld is negative (not -1)\n", N); return 1; }
 if(N == 0) { std::fprintf(stderr, "N=0 is not positive\n"); return 1; }
 const long long UPPER= 1000000000000000000LL;
 if(N > UPPER) { std::fprintf(stderr, "N=%lld too large\n", N); return 1; }

 if(N == -1) {
  long long ng= 0, ok= UPPER + 1;
  while(ok - ng > 1) {
   long long mid= ng + (ok - ng) / 2;
   if((long long)getK(mid) >= K) ok= mid;
   else ng= mid;
  }
  if((long long)getK(ok) == K) {
   std::fprintf(stderr, "user said -1 but N=%lld gives getK=K=%lld\n", ok, K);
   return 1;
  }
  return 0;
 }

 i128 g= getK(N);
 if(g != (i128)K) {
  std::fprintf(stderr, "getK(%lld)=%lld, expected K=%lld\n", N, (long long)g, K);
  return 1;
 }
 return 0;
}
