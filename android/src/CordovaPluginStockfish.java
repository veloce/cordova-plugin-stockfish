package org.lichess.stockfish;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.json.JSONArray;
import org.json.JSONException;

public final class CordovaPluginStockfish extends CordovaPlugin {
    @Override
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
        if (action.equals("toUpper")) {
            toUpper(args, callbackContext);
        } else {
            return false;
        }
        return true;
    }

    private void toUpper(JSONArray args, CallbackContext callbackContext) throws JSONException {
        callbackContext.success(args.getString(0).toUpperCase());
    }
}
