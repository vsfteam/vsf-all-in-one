# Gato图形库 (v0.0.1)
Gato是一个轻量级抗锯齿矢量图形库，支持`svg path`语法，矢量字体渲染，可以实现阴影，模糊等特效。

## 优势
纯软件渲染，可移植性强。代码精简，`cloc`结果如下
```
➜  Gato git:(master) ✗ cloc render 
      10 text files.
      10 unique files.                              
       5 files ignored.

github.com/AlDanial/cloc v 1.88  T=0.03 s (179.7 files/s, 81239.7 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C                                5            274              9           1977
-------------------------------------------------------------------------------
SUM:                             5            274              9           1977
-------------------------------------------------------------------------------
```
## 使用说明
- 准备
### `Ubuntu`
```sh
sudo apt-get install libsdl2-dev
```
### `Mac`
```sh
brew install sdl2
brew install pkg-config
```
- 编译
```sh
make                            #默认编译examples/ui.c
make MAIN=examples/example_1.c  #编译examples/example_1.c
```
- 运行
```sh
./Gato
```
## 联系
QQ群： 934213566