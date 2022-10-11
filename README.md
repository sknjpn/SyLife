# SyLife
セルフデザイン生態系シミュレータ

## Introduction
セルフデザインな生態系シミュレータです。
- 生き物をユーザー自身がデザインし、生態系の構築とシミュレーションが可能
- Linux, Web Browserに対応
- タッチパネルによる操作に対応

## Requirements
すべてのRequirementsは[OpenSiv3D](https://github.com/Siv3D/OpenSiv3D) v6.5.0に基づきます。

|Platform|Requirements|
|:------------------:|:------------------------------|
|Linux|GCC 9.3.0 (with Boost 1.71.0) / GCC 11.2 (with Boost 1.74.0)<br>Intel / AMD CPU<br>OpenGL 4.1 compatible hardware|
|Web|Web browser with WebAssembly and WebGL2 support|

## How to build

### Linux
- [OpenSiv3Dをインストール](https://siv3d.github.io/ja-jp/)する。
- `mkdir build && cd build && cmake .. && cd .. && cmake --build build`
- 完成した実行ファイルをAppフォルダに移動する。
- resourcesフォルダにengineフォルダを移動し、実行する。

### Web
- [Emscriptenをインストール](https://emscripten.org/docs/getting_started/downloads.html)する。
- [emscriptenのテンプレート](https://github.com/sknjpn/OpenSiv3DForWeb-VSCode)をダウンロードする。
- srcフォルダにこのプロジェクトのsrcフォルダを移動し、compile.rspをアップデートする。
- コンパイルして実行する。

## License
SyLife is made available under the [MIT License](LICENSE).
