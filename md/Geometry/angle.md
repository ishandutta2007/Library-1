---
title: 角度・回転関連
documentation_of: ../../src/Geometry/angle.hpp
---

## 関数

|名前|概要|
|---|---|
|`radian_to_degree(r)`|ラジアンの値を度数に変換したものを返す.|
|`degree_to_radian(d)`|度数の値からラジアンに変換したものを返す.|
|`normalize_radian(r)`|ラジアンの値を $\lparen-\pi,\pi\rbrack$ の区間に収まるように正規化したものを返す.|
|`angle(p)`|ベクトル $\boldsymbol{p}$ の偏角を返す.|
|`angle(p,q)`|ベクトル $\boldsymbol{p}$ からベクトル $\boldsymbol{q}$ への偏角を返す. |
|`rotate(theta)`| 原点を中心に角度 $\theta$ だけ回転する変換を返す. <br>返り値の型は `Affine<K>`.|
|`rotate(p, theta)`| 点 $\boldsymbol{p}$ を中心に角度 $\theta$ だけ回転する変換を返す. <br>返り値の型は `Affine<K>`.|
|`rotate90(p)`| 点 $\boldsymbol{p}$ を中心に $\pi/2$ だけ回転する変換を返す. <br>返り値の型は `Affine<K>`.|

## `AngleComp<K>` クラス

`Point<K>` を偏角の大小で比較する関数オブジェクト.\
偏角を $\lparen-\pi,\pi\rbrack$ に正規化した上での大小比較する.\
ただし $\boldsymbol{0}=(0,0)$ の偏角は $0$ とする.\
`K` が整数でも動く \
主に偏角ソートで使う.
