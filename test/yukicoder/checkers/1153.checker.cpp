// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1153
// yuki 1153 ねこちゃんゲーム: self-validating sp judge.
// 公式 Python ジャッジを C++ に直訳。
//
// expected ファイル (yukicoder の test_out) は「期待出力」ではなく
// ジャッジ補助データ:
//   line 1: s        Grundy 目標値 (0 なら Kiri 勝ち = user は -1 -1 出すべき)
//   line 2: N        頂点数
//   line 3: M        猫の数
//   line 4: A[M]     各猫の初期位置 (0-indexed)
//   line 5: P[N]     各頂点の親 (root は -1)
//   line 6: G[N]     各頂点の Grundy (rooted)
//   line 7: G_p[N]   親方向ぶんを除いた Grundy
//   line 8: G_bu[N]  bottom-up Grundy
//
// 注意: checker 内では Library の Tree/Rerooting を使わない (verify 対象なので循環回避)
// Args (run-lib.sh testlib convention): input user_output expected
//   - input は Python ジャッジでは未使用なので open すらしない
#include <cstdio>
#include <fstream>
#include <vector>

int main(int argc, char** argv) {
 if(argc < 4) { std::fprintf(stderr, "usage: checker input user_output expected\n"); return 2; }
 std::ifstream out(argv[2]);
 std::ifstream exp(argv[3]);
 if(!out || !exp) { std::fprintf(stderr, "open failed\n"); return 2; }

 long long i, v;
 if(!(out >> i >> v)) { std::fprintf(stderr, "user output must be two integers\n"); return 1; }
 long long extra;
 if(out >> extra) { std::fprintf(stderr, "extra output present\n"); return 1; }

 long long s;
 if(!(exp >> s)) { std::fprintf(stderr, "expected parse: s\n"); return 2; }

 if(s == 0) {
  if(i == -1 && v == -1) return 0;
  std::fprintf(stderr, "expected -1 -1 (Kiri wins) but got %lld %lld\n", i, v);
  return 1;
 }
 if(i == -1 && v == -1) {
  std::fprintf(stderr, "user said -1 -1 but Ebi wins (s=%lld)\n", s);
  return 1;
 }

 int N, M;
 if(!(exp >> N >> M)) { std::fprintf(stderr, "expected parse: N M\n"); return 2; }

 std::vector<long long> A(M), P(N), G(N), G_p(N), G_bu(N);
 for(auto& x: A)
  if(!(exp >> x)) { std::fprintf(stderr, "expected parse: A\n"); return 2; }
 for(auto& x: P)
  if(!(exp >> x)) { std::fprintf(stderr, "expected parse: P\n"); return 2; }
 for(auto& x: G)
  if(!(exp >> x)) { std::fprintf(stderr, "expected parse: G\n"); return 2; }
 for(auto& x: G_p)
  if(!(exp >> x)) { std::fprintf(stderr, "expected parse: G_p\n"); return 2; }
 for(auto& x: G_bu)
  if(!(exp >> x)) { std::fprintf(stderr, "expected parse: G_bu\n"); return 2; }

 if(!(1 <= i && i <= M)) { std::fprintf(stderr, "i=%lld not in [1,%d]\n", i, M); return 1; }
 if(!(1 <= v && v <= N)) { std::fprintf(stderr, "v=%lld not in [1,%d]\n", v, N); return 1; }

 --i, --v;
 long long u= A[i];
 if(u < 0 || u >= N) { std::fprintf(stderr, "A[%lld]=%lld out of [0,%d)\n", i, u, N); return 2; }

 if(v == P[u]) {
  if((G_p[u] ^ G[u]) == s) return 0;
  std::fprintf(stderr, "move-to-parent: G_p[%lld]^G[%lld]=%lld, expected s=%lld\n",
               u, u, G_p[u] ^ G[u], s);
  return 1;
 }
 if(P[v] != u) {
  std::fprintf(stderr, "v=%lld is neither parent nor child of u=%lld (P[v]=%lld)\n", v, u, P[v]);
  return 1;
 }
 if((G[u] ^ G_bu[v]) == s) return 0;
 std::fprintf(stderr, "move-to-child: G[%lld]^G_bu[%lld]=%lld, expected s=%lld\n",
              u, v, G[u] ^ G_bu[v], s);
 return 1;
}
