module.exports.init = function (success, error) {
  cordova.exec(success, error, 'CordovaPluginStockfish', 'init', []);
};

module.exports.cmd = function (cmd, success, error) {
  if (typeof cmd !== 'string') {
    error('cmd must be called with a single string parameter.');
  } else {
    cordova.exec(success, error, 'CordovaPluginStockfish', 'cmd', [cmd]);
  }
};

module.exports.exit = function (success, error) {
  cordova.exec(success, error, 'CordovaPluginStockfish', 'exit', []);
};
