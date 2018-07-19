package org.lichess.stockfish;

import static java.util.concurrent.TimeUnit.*;

import android.app.ActivityManager;
import android.content.Context;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.PluginResult;
import org.json.JSONArray;
import org.json.JSONException;

public final class CordovaPluginStockfish extends CordovaPlugin {

  private CallbackContext outputCallbackContext;

  private final ScheduledExecutorService scheduler =
    Executors.newScheduledThreadPool(1);

  private boolean isInit = false;
  private ScheduledFuture<?> stopOnPauseHandle;

  static {
    System.loadLibrary("stockfishjni");
  }

  @Override
  public void onDestroy() {
    super.onDestroy();
    if(isInit) {
      doExit();
    }
  }

  @Override
  public void onPause(boolean multitasking) {
    super.onPause(multitasking);
    if(isInit) {
      stopOnPauseHandle = scheduler.schedule(new Runnable() {
        public void run() {
          if (isInit) {
            jniCmd("stop");
          }
        }
      }, 60 * 10, SECONDS);
    }
  }

  @Override
  public void onResume(boolean multitasking) {
    super.onResume(multitasking);
    if(isInit) {
      if (stopOnPauseHandle != null) {
        stopOnPauseHandle.cancel(false);
      }
    }
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
      uiExit(callbackContext);
    } else {
      return false;
    }
    return true;
  }

  private void init(CallbackContext callbackContext) {
    if(!isInit) {
      // Get total device RAM for hashtable sizing
      Context context = this.cordova.getActivity().getApplicationContext();
      ActivityManager actManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
      ActivityManager.MemoryInfo memInfo = new ActivityManager.MemoryInfo();
      actManager.getMemoryInfo(memInfo);
      long totalMemory = memInfo.totalMem;
      jniInit(totalMemory);
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

  private void uiExit(CallbackContext callbackContext) throws JSONException {
    if(isInit) {
      doExit();
      callbackContext.success();
    } else {
      callbackContext.error("Stockfish isn't currently running!");
    }
  }

  private void doExit() {
    if(isInit) {
      jniExit();
      isInit = false;
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

  public native void jniInit(long memorySize);

  public native void jniExit();

  public native void jniCmd(String cmd);
}
