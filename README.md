# cordova-plugin-stockfish

Cordova interface for the Stockfish chess engine.

## Exemple apps

* https://github.com/srenault/stockfishm/tree/master
* https://github.com/veloce/lichobile

## Requirements

* [android ndk](http://developer.android.com/ndk/index.html)

## Usage

`Stockfish` global object exposes only 4 methods. Except for `output`, they all
return promises:

```js
// init engine
Stockfish.init().then(() => console.log('engine ready'))

// send commands
Stockfish.cmd('position startpos').then(() => console.log('command success'))

// register output listener
Stockfish.output(function(message) {
  console.log('stockfish message received:', message)
})

// exit engine
Stockfish.exit().then(() => console.log('engine successfully exited'))
```

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

To enable variants, in custom compiler flags, `Other C++ flags` section, add:

    -DKOTH -DTHREECHECK -DATOMIC -DHORDE -DRACE -DANTI -DCRAZYHOUSE
