module.exports = {
  init: function () {
    return new Promise(function(resolve, reject) {
      cordova.exec(resolve, reject, 'CordovaPluginStockfish', 'init', []);
    });
  },

  cmd: function (cmd) {
    if (typeof cmd !== 'string') {
      throw new Error('window.Stockfish.cmd must be called with a single string parameter.');
    }

    return new Promise(function(resolve, reject) {
      cordova.exec(resolve, reject, 'CordovaPluginStockfish', 'cmd', [cmd]);
    });
  },

  output: function (success, error) {
    cordova.exec(success, error, 'CordovaPluginStockfish', 'output', []);
  },

  exit: function () {
    return new Promise(function(resolve, reject) {
      cordova.exec(resolve, reject, 'CordovaPluginStockfish', 'exit', []);
    });
  }
};
