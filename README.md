# tinyShell
====

tiny Linux shell for learn a system call

## Description
実装機能

### 単一コマンドの実行
`fork()` & `execvp()`

```shell
abcdefgh :/myshell> whoami
abcdefgh
abcdefgh :/myshell> whoam
-myshell: whoam: command not found
abcdefgh :/myshell>
```

### リダイレクション
`dup2()`
* `<` リダイレクションイン
* `>` リダイレクションアウト
* `>>` リダイレクションアウト（追加）

```shell
abcdefgh :/myshell> cat ls.txt
apple
bash
mikan
abcdefgh :/myshell> wc < ls.txt > result
abcdefgh :/myshell> cat result
```

### パイプライン
`pipe()` & `dup2()`

```shell
abcdefgh :/myshell> cat ls.txt | sort | uniq | cat -b
     1  apple
     2  bash
     3  cash
     4  mikan
     5  test
```

### 標準エラー出力
* `>&` リダイレクションアウト
* `|&` パイプ
* `>>&` リダイレクションアウト（追加）

### 内部コマンド
#### cd カレントディレクトリの移動
`chdir()`
#### pushd/popd スタックを用いたカレントディレクトリの移動