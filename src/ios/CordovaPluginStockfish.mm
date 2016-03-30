#import "CordovaPluginStockfish.h"
#import "CordovaPluginStockfishios.h"
#import <Cordova/CDVPlugin.h>

@implementation CordovaPluginStockfish

NSString *outputCallback;
NSNumber *isInit = @FALSE;

- (void)init:(CDVInvokedUrlCommand*)command
{
  [self.commandDelegate runInBackground:^{
      if(![isInit boolValue]) {
        CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
        stockfishios::init((__bridge void*)self);
        isInit = @TRUE;
        [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
      } else {
        NSString *error = @"Stockfish is already initialized";
        CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:error];
        [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
      }
  }];
}

- (void)cmd:(CDVInvokedUrlCommand*)command
{
  if([isInit boolValue]) {
    [self.commandDelegate runInBackground:^{
        NSString* cmd = [command.arguments objectAtIndex:0];
        CDVPluginResult* pluginResult = nil;
        if (cmd != nil) {
          stockfishios::cmd(std::string([cmd UTF8String]));
          pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
        } else {
          pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:@"Missing cmd arg"];
        }
        [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
    }];
  } else {
    NSString *error = @"Please exec init before doing anything";
    CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:error];
    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
  }
}

- (void)output:(CDVInvokedUrlCommand*)command
{
  outputCallback = command.callbackId;
  CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
  [pluginResult setKeepCallback:[NSNumber numberWithBool:YES]];
}

- (void)exit:(CDVInvokedUrlCommand*)command
{
  if([isInit boolValue]) {
    [self.commandDelegate runInBackground:^{
        CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
        stockfishios::cmd("stop");
        stockfishios::exit();
        [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
    }];
  } else {
    NSString *error = @"Stockfish isn't currently running!";
    CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:error];
    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
  }
}

- (void)sendOutput:(NSString *) output
{
  CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:output];
  [pluginResult setKeepCallbackAsBool:YES];
  [self.commandDelegate sendPluginResult:pluginResult callbackId:outputCallback];
}

void StockfishSendOutput (void *stockfish, const char *output)
{
    [(__bridge id) stockfish sendOutput:[NSString stringWithUTF8String:output]];
}

@end
