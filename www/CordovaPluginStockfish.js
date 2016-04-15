module.exports = {

  init: function (success, error) {
    if (success) {
      cordova.exec(success, error, 'CordovaPluginStockfish', 'init', []);
    } else {
      return new Promise(function(resolve, reject) {
        cordova.exec(resolve, reject, 'CordovaPluginStockfish', 'init', []);
      });
    }
  },

  cmd: function (cmd, success, error) {
    if (typeof cmd !== 'string') {
      throw new Error('window.Stockfish.cmd must be called with a single string parameter.');
    }

    if (success) {
      cordova.exec(success, error, 'CordovaPluginStockfish', 'cmd', [cmd]);
    } else {
      return new Promise(function(resolve, reject) {
        cordova.exec(resolve, reject, 'CordovaPluginStockfish', 'cmd', [cmd]);
      });
    }
  },

  output: function (success, error) {
    cordova.exec(success, error, 'CordovaPluginStockfish', 'output', []);
  },

  exit: function (success, error) {
    if (success) {
      cordova.exec(success, error, 'CordovaPluginStockfish', 'exit', []);
    } else {
      return new Promise(function(resolve, reject) {
        cordova.exec(resolve, reject, 'CordovaPluginStockfish', 'exit', []);
      });
    }
  }
};
