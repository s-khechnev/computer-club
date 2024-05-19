# Тестовое задание на должность инженера по разработке ПО для базовых станций (C++)

## Как собрать

Для сборки требуется поддержка C++20.

```sh
git clone git@github.com:s-khechnev/computer-club-manager.git
cd computer-club-manager && mkdir build
cmake -S . -B build && cmake --build build
```

Зависимости:

1. cmake 3.8
2. Google Test

## Как запустить

```sh
build/src/main input.txt
```

### Как запустить тесты

```sh
build/tests/end-to-end-test
```

### Тестовые примеры

1. [example](tests/end-to-end/data/example)
2. [all](tests/end-to-end/data/all)
