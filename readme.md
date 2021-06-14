# mtlglfw_01
Metalのラッパーであるmtlppとglfwを組み合わせて処理を実行するサンプルです。  
mtlppにはいくつかglfw向けの機能が足りないのでforkして少し改造したものを使っています。  
https://github.com/desktopgame/mtlpp

また、以下のサンプルを参考にしています。  
https://gist.github.com/gcatlin/987be74e2d58da96093a7598f3fbfb27

## 実行方法
以下でXcodeプロジェクトを作成し、xcodeprojを開いてデバッグ
````
cd xcode
cmake .. -G Xcode
````