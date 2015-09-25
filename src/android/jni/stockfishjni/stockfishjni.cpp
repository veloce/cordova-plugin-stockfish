#include <jni.h>
#include <string>
#include <stockfishcli.h>
#include <search.cpp>

extern "C" {
  JNIEXPORT void JNICALL Java_org_lichess_stockfish_CordovaPluginStockfishJNI_init(JNIEnv *env, jclass clazz);
  JNIEXPORT void JNICALL Java_org_lichess_stockfish_CordovaPluginStockfishJNI_exit(JNIEnv *env, jclass clazz);
  JNIEXPORT jstring JNICALL Java_org_lichess_stockfish_CordovaPluginStockfishJNI_cmd(JNIEnv *env, jclass clazz, jstring cmd);
};

JNIEXPORT void JNICALL Java_org_lichess_stockfish_CordovaPluginStockfishJNI_init(JNIEnv *env, jclass clazz) {
  UCI::init(Options);
  PSQT::init();
  Bitboards::init();
  Position::init();
  Bitbases::init();
  Search::init();
  Eval::init();
  Pawns::init();
  Threads.init();
  Tablebases::init(Options["SyzygyPath"]);
  TT.resize(Options["Hash"]);
}

JNIEXPORT void JNICALL Java_org_lichess_stockfish_CordovaPluginStockfishJNI_exit(JNIEnv *env, jclass clazz) {
  //TODO
}

JNIEXPORT jstring JNICALL Java_org_lichess_stockfish_CordovaPluginStockfishJNI_cmd(JNIEnv *env, jclass clazz, jstring jcmd) {
  const char *cmd = env->GetStringUTFChars(jcmd, (jboolean *)0);
  std::streambuf* originOut = std::cout.rdbuf();
  std::ostringstream myout;
  std::cout.rdbuf(myout.rdbuf());
  stockfishcli::commandInit();
  stockfishcli::command(cmd);
  Threads.main()->join();
  std::cout.rdbuf(originOut);
  env->ReleaseStringUTFChars(jcmd, cmd);
  return env->NewStringUTF(myout.str().c_str());
}
