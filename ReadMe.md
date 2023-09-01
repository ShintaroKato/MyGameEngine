# 制作期間
- 2022/04 ～ 2023/08 (16ヶ月、現在も制作中)
# 制作人数
- 個人制作
# 使用した外部ライブラリ
- DirectXTex
- FBXSDK
# PR
- 特に技術的に工夫した点
    - ステージ作成シーンで作成したものを他のシーンに反映させる処理
    - 配置したオブジェクトを自動的に避けて移動する処理
# Git
- https://github.com/ShintaroKato/MyGameEngine/tree/game01
# 動作環境
- エディション	    Windows 11 Pro
- OS ビルド	       22621.1635
- プロセッサ	    Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz   2.60 GHz
- 実装 RAM	       16.0 GB (15.8 GB 使用可能)
- システムの種類	64 ビット オペレーティング システム、x64 ベース プロセッサ
# 概要
- 自分でステージを作成して防衛対象を守りながら決められた数の敵を、
　タワーディフェンス型の3Dアクションです。
# 操作説明
## ステージエディタ画面
- 左側のボタンを左クリック：防衛用のオブジェクトを呼び出せます。
    - 赤：耐久力が高く判定が広めの障壁です。
    - 青：敵が接近したときに１体を狙って自動的に攻撃します。
- 最初から配置されているオブジェクトが防衛対象です。
- 右クリックしながらドラッグ操作：視点を変更できます。
- 配置済みのオブジェクトを左クリック：オブジェクトの移動、回転ができます。
    - [SPACE]キー：選択中のオブジェクトを削除します。
    - オブジェクトを選択中[A]/[D]キー：オブジェクトを回転させます。
        - [SHIFT]キーを押しながら回転させると90度ずつ回せます。
## ゲーム本編
- マウスで視点を操作できます。
- 左クリック：近接攻撃と遠距離攻撃を同時に行います。
- [SPACE]キー：ジャンプします。
- [W][A][S][D]キー：入力した方向へ移動します。
- [esc]キー：ポーズメニューを表示します。
# 出現する敵
- 直進型：防衛対象に向かって直進します。
- 飛行型：空中から接近し、遠距離攻撃をしてきます。
- 経路探索型：見た目は直進型と同じですが、ある程度オブジェクトを避けて防衛対象に接近してきます。