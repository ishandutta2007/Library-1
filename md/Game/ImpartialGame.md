---
title: 不偏ゲーム (Grundy数)
documentation_of: ../../src/Game/ImpartialGame.hpp
---

## 概要
不偏ゲームのGrundy数(Nim数, Nimber)を、メモ化再帰を用いて計算する。

不偏ゲームとは、
- 2人のプレイヤーが交互に手を指す
- 可能な手は、現在の局面によってのみ決まる(プレイヤーによらない)
- ゲームが必ず有限回の手番で終了し、引き分けがない
- 最後に手を指したプレイヤーが勝ち(通常規約)
という条件を満たすゲームである。

## 使い方

`ImpartialGame<Game, F> ig(f);`

- `Game`: ゲームの状態を表す型。`std::map` のキーとして使用できる必要がある (例: `int`, `std::pair`, `std::vector`)。
- `F`: `Game` 型の引数をとり、遷移可能な次の状態のリスト (`std::vector<Game>`) を返す関数オブジェクト。
- `f`: `F` 型のインスタンス。

### メンバ関数
- `unsigned eval(Game g)`: 状態 `g` のGrundy数を返す。

## 計算量
- 状態数を $S$、状態 $g$ からの遷移数を $T_g$ とする。
- `eval(g)` を初めて呼び出すとき、計算量は $O(\sum_{g' \in \text{reachable from } g} ((\text{cost of } f(g')) + T_{g'} \log S))$ 程度。
- 2回目以降の呼び出しは $O(\log S)$。

## 使用例

### Nim
石が $N$ 個の山からなるNimのGrundy数を計算する。状態は各山の石の数 `std::vector<int>` で表せる。

```cpp
#include <iostream>
#include <vector>
#include <numeric>
#include "src/Game/ImpartialGame.hpp"

int main() {
    using Game = std::vector<int>;
    auto f = [&](const Game& g) {
        std::vector<Game> ret;
        for (size_t i = 0; i < g.size(); ++i) {
            for (int j = 0; j < g[i]; ++j) {
                Game next_g = g;
                next_g[i] = j;
                ret.push_back(next_g);
            }
        }
        return ret;
    };

    ImpartialGame<Game, decltype(f)> ig(f);

    Game initial_state = {1, 2, 3};
    unsigned g_num = ig.eval(initial_state);

    // NimのGrundy数は石の数のXOR和に等しい
    // g_num == (1 ^ 2 ^ 3) == 0
    std::cout << "Grundy number: " << g_num << std::endl;
    if (g_num != 0) {
        std::cout << "First player wins." << std::endl;
    } else {
        std::cout << "Second player wins." << std::endl;
    }

    return 0;
}
```

