package org.lichess.stockfish;


public class CordovaPluginStockfishJNI {

  static {
    System.loadLibrary("stockfishjni");
  }

  public static native void init();

  public static native void exit();

  public static native void cmd(String cmd);
}
