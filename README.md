# cordova-plugin-stockfish

Cordova interface for the Stockfish chess engine.

## Exemple app

https://github.com/srenault/stockfishm/tree/master

## Requirements

* [android ndk](http://developer.android.com/ndk/index.html)

## Upgrade stockfish (lichess)

```
git remote add stockfish https://github.com/ddugovic/Stockfish.git
git subtree pull --prefix=src/share/stockfish --squash stockfish master
```

## Build android

Build the native code in your project using:

```
ndk-build -C app/platforms/android
```

## Build IOS

Through XCode, in the build settings menu:
  * Set `C++ Language Dialect` option to `C++11` value.
  * Set `C++ Standard Library` option to `lib++` value.
