# ntp-test

## Необходимые пакеты для сборки под ubuntu:

* `sudo apt install gcc cmake gdb g++ libboost-all-dev libsqlite3-dev`
* Можно еще клиент для работы с  файлом БД: `sudo apt install sqlite3`

## Сборка
В директории с проектом выполнить:
```
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```
