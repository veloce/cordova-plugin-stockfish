package org.lichess.stockfish;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.json.JSONArray;
import org.json.JSONException;

public final class CordovaPluginStockfish extends CordovaPlugin {

  static {
    System.loadLibrary("stockfishjni");
  }

  @Override
  public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
    if (action.equals("init")) {
      init(callbackContext);
    } else if (action.equals("cmd")) {
      cmd(args, callbackContext);
    } else if (action.equals("exit")) {
      exit(callbackContext);
    } else {
      return false;
    }
    return true;
  }

  private void init(CallbackContext callbackContext) throws JSONException {
    jniInit();
    callbackContext.success();
  }

  private void cmd(final JSONArray args, final CallbackContext callbackContext) throws JSONException {
    this.cordova.getThreadPool().execute(new Runnable() {
      public void run() {
        try {
          String cmd = args.getString(0);
          jniCmd(cmd);
          callbackContext.success();
        } catch (JSONException e) {
          callbackContext.error("Unable to perform `cmd`: " + e.getMessage());
        }
      }
    });
  }

  private void exit(CallbackContext callbackContext) throws JSONException {
    jniExit();
    callbackContext.success();
  }

  // JNI stuff

  public void f(String s) {
    android.util.Log.d("LICHOBILE", s);
  }

  public native void jniInit();

  public native void jniExit();

  public native void jniCmd(String cmd);
}
