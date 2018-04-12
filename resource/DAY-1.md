## コードの修正 

- `RESB <NUMBER>-$` 文法がつかわれているがnaskのみに使える機能.
nasmでかけるように修正した. `RESB <NUMBER>-($-$$)` が正しいらしい

- Ubuntuでの開発を前提としたスクリプトを追加している
    - `bin/setup` 必要なパッケージをインストールしてくれる
    - `bin/run-image <IMAGE_NAME>` `<IMAGE_NAME>` でエミュレータを実行してくる
    - `bin/buld-img` hello.nasmからhello.imgを作成する


