---
title: オートマトン上のDP
documentation_of: ../../src/Misc/Automaton.hpp
---

オートマトンで表現できる条件を満たす文字列（や数列）について、その個数や総和などを効率的に計算するためのライブラリ。いわゆる「桁DP」は、このライブラリの典型的な応用例の一つである。

状態遷移をオートマトンとして定義し、そのオートマトンが受理する文字列に対してDPを行う。決定性有限オートマトン (DFA) だけでなく、非決定性有限オートマトン (NFA) も扱うことができる。

## 主な機能
- 決定性/非決定性オートマトンの構築
- 2つのオートマトンの積の構築 (`operator&`)
- オートマトンが受理する長さ `n` の文字列の個数計算 (`num`)
- カスタム演算（総和など）でのDP実行 (`dp_run`)

## 注意事項
- **NFAの取り扱い**: NFAをコンストラクタに渡すと、内部で部分集合構成法により等価なDFAに変換される。これにより状態数が指数関数的に増加（ $O(2^S)$ ）する可能性があるため、元のNFAの状態数が大きい場合には注意が必要。

## メンバ関数

| 名前 | 概要 |
|:---|:---|
| `Automaton(alp, init, tr, ac, ...)` | コンストラクタ。<br> `alp`: アルファベット (`std::vector<symbol_t>`)。<br> `init`: 初期状態 (`state_t`)。<br> `tr`: 遷移関数。`state_t` と `symbol_t` から次の状態 `state_t` (DFA) または `std::set<state_t>` (NFA) を返す。<br> `ac`: 状態が受理状態かを判定する関数 (`state_t -> bool`)。<br> その他、拒否状態 `rej` やイプシロン遷移 `eps` を指定するオーバーロードが存在する。 |
| `operator&` | 2つのオートマトンの積オートマトンを返す。積オートマトンの状態は元の状態のペア `(s1, s2)` で表される。文字列 `w` が積オートマトンに受理されるのは、`w` が元の両方のオートマトンに受理される場合のみ。 |
| `dp_run<T>(n, op, ti, f, init)` | オートマトンが受理する長さ `n` の文字列に対し、指定された演算でDPを実行し、結果を返す。<br> `T`: DPの値の型（モノイドをなす必要がある）。<br> `op`: `T`上の二項演算 (`T x T -> T`)。<br> `ti`: `op` に対する単位元。<br> `f`: DPの遷移を表す関数 (`T x symbol_t x int -> T`)。`i` は文字の位置（0-indexed）。<br> `init`: 長さ0の文字列に対するDPの初期値。 |
| `num<T>(n)` | オートマトンが受理する長さ `n` の文字列の個数を返す。`dp_run` のラッパー。 |

## 使用例：桁DP (ABC154 E - Almost Everywhere Zero)

「$N$ 以下の正の整数で、0でない数字がちょうど $K$ 個あるものの個数はいくつか？」

### 1. オートマトンの設計

この問題を解くために、2つの条件をそれぞれ判定するオートマトンを設計し、その積を取る。

- **条件1: $N$ 以下である**
  - `state_t`: `int` 型。`s < N.size()` なら $N$ の $s$ 文字目まで一致、`s == N.size()` なら $N$ より小さいことが確定、`s == N.size() + 1` なら $N$ より大きいことが確定（拒否状態）。
  - `symbol_t`: `int` 型 (0-9)。
  - 遷移: 今の状態が `s` で数字 `c` を受け取ったとき、
    - `s < N.size()` の場合:
      - `c < N[s]`: 次の状態は `N.size()` (未満確定)
      - `c == N[s]`: 次の状態は `s + 1` (一致継続)
      - `c > N[s]`: 次の状態は `N.size() + 1` (超過確定)
    - `s >= N.size()` の場合: 次の状態は `s` (状態維持)

- **条件2: 0でない数字がちょうど $K$ 個**
  - `state_t`: `int` 型。これまでに現れた0でない数字の個数。
  - `symbol_t`: `int` 型 (0-9)。
  - 遷移: 今の状態が `s` で数字 `c` を受け取ったとき、
    - `c == 0`: 次の状態は `s`
    - `c != 0`: 次の状態は `s + 1`
  - 受理状態: `s == K`

### 2. 実装コード

```cpp
#include <iostream>
#include <vector>
#include <string>
#include "src/Misc/Automaton.hpp"

void solve() {
    std::string N;
    int K;
    std::cin >> N >> K;
    int n = N.length();

    std::vector<int> alp = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // オートマトン1: N以下か
    auto tr_le = [&](int s, int c) {
        if (s >= n) return s; // 既に大小関係が確定
        int d = N[s] - '0';
        if (c < d) return n;     // 未満確定
        if (c > d) return n + 1; // 超過確定 (拒否状態)
        return s + 1;            // 一致継続
    };
    auto ac_le = [](int s) { return s <= n; }; // 超過状態でなければOK
    Automaton dfa_le(alp, 0, tr_le, ac_le, n + 1);

    // オートマトン2: 0でない数字がK個
    auto tr_k = [&](int s, int c) {
        if (s > K) return s;
        return s + (c != 0);
    };
    auto ac_k = [&](int s) { return s == K; };
    Automaton dfa_k(alp, 0, tr_k, ac_k, K + 1);

    // 2つのオートマトンの積を取る
    auto dfa = dfa_le & dfa_k;

    // 長さnの文字列（先頭の0埋めを許容）で条件を満たすものを数える
    // ただし、整数「0」は数えないように後で調整
    long long ans = dfa.num<long long>(n);

    // 先頭の0を考慮し、長さがn未満の数も数える
    // (dfa_kは先頭の0を区別しないため、これで長さn-1, n-2, ...の数も数えられる)
    // 例: N=100, K=1 のとき、"010"は10として扱われる
    // 最後に、全て0である "00...0" のケースを除外する
    std::cout << ans - 1 << std::endl;
}
```
*注: 上記コードは桁DPの考え方を説明するためのもので、問題の厳密な解法（特に先頭の0の扱いや長さが異なる数の考慮）とは少し異なります。実際の提出コードでは、`dp_run` を使ってより複雑な条件を処理したり、オートマトンの状態に「先頭から0が続いているか」という情報を追加したりします。*

## 参考
- [https://kuretchi.github.io/blog/entries/automaton-dp/](https://kuretchi.github.io/blog/entries/automaton-dp/)
- [https://qiita.com/hotman78/items/74c2d5d246ba3b6b3836](https://qiita.com/ryuhe1/items/185e1a283f13ac638a53)
- [https://blog.knshnb.com/posts/aoj2587/](https://blog.knshnb.com/posts/aoj2587/)

## Verify
- [Educational DP Contest S - Digit Sum](https://atcoder.jp/contests/dp/tasks/dp_s)
- [Typical DP Contest E - 数](https://atcoder.jp/contests/tdpc/tasks/tdpc_number) 
- [AtCoder Beginner Contest 007 D - 禁止された数字](https://atcoder.jp/contests/abc007/tasks/abc007_4) 
- [AtCoder Beginner Contest 029 D - 1](https://atcoder.jp/contests/abc029/tasks/abc029_d) 
- [AtCoder Beginner Contest 050 D - Xor Sum](https://atcoder.jp/contests/abc050/tasks/arc066_b) (ナイーブにNFA) 
- [Donuts プロコンチャレンジ2014 B - 7th](https://atcoder.jp/contests/donuts-live2014/tasks/donuts_live2014_2) 
- [CODE FESTIVAL 2014 予選A D - 壊れた電卓](https://atcoder.jp/contests/code-festival-2014-quala/tasks/code_festival_qualA_d) 
- [いろはちゃんコンテスト Day1 H - ちらし寿司](https://atcoder.jp/contests/iroha2019-day1/tasks/iroha2019_day1_h) 
- [yukicoder No.1953 ８](https://yukicoder.me/problems/no/1953) (sp judge)
