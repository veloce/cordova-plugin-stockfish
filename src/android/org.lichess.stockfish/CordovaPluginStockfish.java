package org.lichess.stockfish;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.PluginResult;
import org.json.JSONArray;
import org.json.JSONException;

public final class CordovaPluginStockfish extends CordovaPlugin {

  private CallbackContext outputCallbackContext;

  private boolean isInit = false;

  static {
    System.loadLibrary("stockfishjni");
  }

  @Override
  public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
    if (action.equals("init")) {
      init(callbackContext);
    } else if (action.equals("cmd")) {
      cmd(args, callbackContext);
    } else if (action.equals("output")) {
      output(callbackContext);
    } else if (action.equals("exit")) {
      exit(callbackContext);
    } else {
      return false;
    }
    return true;
  }

  private void init(CallbackContext callbackContext) {
    if(!isInit) {
      jniInit();
      isInit = true;
    }
    callbackContext.success();
  }

  private void cmd(final JSONArray args, final CallbackContext callbackContext) throws JSONException {
    if(isInit) {
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
    } else {
      callbackContext.error("Please exec init before doing anything");
    }
  }

  private void output(CallbackContext callbackContext) {
    this.outputCallbackContext = callbackContext;
    PluginResult pluginResult = new PluginResult(PluginResult.Status.NO_RESULT);
    pluginResult.setKeepCallback(true);
    callbackContext.sendPluginResult(pluginResult);
  }

  private void exit(CallbackContext callbackContext) throws JSONException {
    if(isInit) {
      jniCmd("stop");
      jniExit();
      callbackContext.success();
      isInit = false;
    } else {
      callbackContext.error("Stockfish isn't currently running!");
    }
  }

  private void sendOutput(String output) {
    if (outputCallbackContext != null) {
      PluginResult result = new PluginResult(PluginResult.Status.OK, output);
      result.setKeepCallback(true);
      outputCallbackContext.sendPluginResult(result);
    }
    webView.postMessage("stockfish", output);
  }

  // JNI stuff

  public void onMessage(byte[] b) {
    String output = new String(b);
    sendOutput(output);
  }

  public native void jniInit();

  public native void jniExit();

  public native void jniCmd(String cmd);
}
