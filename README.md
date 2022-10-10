# SyLife
インタラクティブ生態系シミュレータ

## Introduction
インタラクティブかつマルチプラットフォームで動作可能な生態系シミュレータです。
- 生き物をユーザー自身がデザインし、生態系の構築とシミュレーションが可能
- Windows, (macOS), (Linux), (Web Browser)に対応
- タッチパネルによる操作に対応

## Requirements
すべてのRequirementsは[OpenSiv3D](https://github.com/Siv3D/OpenSiv3D) v6.5.0に基づきます。

|Platform|Requirements|
|:------------------:|:------------------------------|
|Windows|Windows 7 SP1 / 8.1 / 10 / 11 (64-bit)<br>Microsoft Visual C++ 2022 17.3<br>Windows 10 SDK<br>Intel / AMD CPU|
|macOS|macOS Mojave / Catalina / Big Sur / Monterey<br>Xcode 11.3 or newer (Big Sur requires Xcode 12.5 or newer)<br>Intel CPU / Apple Silicon (Rosetta mode)<br>OpenGL 4.1 compatible hardware|
|Linux|GCC 9.3.0 (with Boost 1.71.0) / GCC 11.2 (with Boost 1.74.0)<br>Intel / AMD CPU<br>OpenGL 4.1 compatible hardware|
|Web|Web browser with WebAssembly and WebGL2 support|

## How to build

### Windows
- [OpenSiv3Dをインストール](https://siv3d.github.io/ja-jp/)する。
- SyLife.slnを開いてビルドする。

### macOS
- [OpenSiv3Dをインストール](https://siv3d.github.io/ja-jp/)する。

### Linux
- [OpenSiv3Dをインストール](https://siv3d.github.io/ja-jp/)する。

### Web
- [Emscriptenをインストール](https://emscripten.org/docs/getting_started/downloads.html)する。

## License
SyLife is made available under the [MIT License](LICENSE).
