TegeViewer
===========
**TegeViewer** はQt6を使った画像ビュアーです。

## 概要
- ~~LinuxとWindows両方で使える（ソースコードも共通に）~~
- Ver1.20よりWindowsのみ
- 対応画像ファイル .bmp .jpg .png （Qt6で読み込めるフォーマット）
- 指定エリア（ROI）内平均値、標準偏差等を解析
- 指定エリア（ROI）の画像レベルやヒストグラム表示
- 表示データをクリップボードにコピー（TAB区切りテキスト）
- Ver1.10より、UVCカメラ取込対応（ひとまずWindowsのみ）
## 動作環境
- Winodws10 / Windows11
- ~~Ubuntu等Linux~~

LinuxはUbuntu22.04とLinuxMint22では動作確認しましたが、  
それ以外の環境は未確認です。（Ver1.10まで）

## 参照
Ver1.20で下記QtCameraControlを参考に実装しました。

https://github.com/m-riley04/QtCameraControls

## 使い方
[Release](https://github.com/immengineer/TegeViewer/releases)からダウンロードして下さい、
詳細はTegeViewer.pdfを参照して下さい。
