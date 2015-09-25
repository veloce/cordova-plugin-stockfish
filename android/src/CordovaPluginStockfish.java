package org.lichess.stockfish;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.json.JSONArray;
import org.json.JSONException;

public final class CordovaPluginStockfish extends CordovaPlugin {
  @Override
  public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
    if (action.equals("init")) {
      init(callbackContext);
    } else if (action.equals("cmd") {
      cmd(args, callbackContext);
    } else if (action.equals("exit") {
      exit(callbackContext);
    } else {
      return false;
    }
    return true;
  }

  private void init(CallbackContext callbackContext) throws JSONException {
    callbackContext.success();
  }

  private void cmd(JSONArray args, CallbackContext callbackContext) throws JSONException {
    callbackContext.success(args.getString(0));
  }

  private void exit(CallbackContext callbackContext) throws JSONException {
    callbackContext.success();
  }
}
