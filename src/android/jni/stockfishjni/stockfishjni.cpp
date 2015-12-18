#include <jni.h>
#include <string>
#include <stockfishcli.h>
#include <search.cpp>
#include <android/log.h>

#define LOGD(TAG,...) __android_log_print(ANDROID_LOG_DEBUG  , TAG,__VA_ARGS__)

extern "C" {
  JNIEXPORT void JNICALL Java_org_lichess_stockfish_CordovaPluginStockfishJNI_init(JNIEnv *env, jclass clazz);
  JNIEXPORT void JNICALL Java_org_lichess_stockfish_CordovaPluginStockfishJNI_exit(JNIEnv *env, jclass clazz);
  JNIEXPORT void JNICALL Java_org_lichess_stockfish_CordovaPluginStockfishJNI_cmd(JNIEnv *env, jclass clazz, jstring jcmd);
};

bool run = false;

auto readstdout = []() {
  std::streambuf* out = std::cout.rdbuf();
  std::ostringstream lichout;
  std::cout.rdbuf(lichout.rdbuf());

  LOGD("stockfishcli", "##> UP");

  run = true;

  while(run) {
    std::string output = lichout.str();
    lichout.str("");

    std::istringstream lichin(output);
    char line[1024];
    lichin.getline(line, 1024);

    if(strlen(line) > 0) {
      LOGD("stockfishcli", "##> %s", line);
    }
  };

  std::cout.rdbuf(out);
};

std::thread reader (readstdout);

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
  run = false;
}

JNIEXPORT void JNICALL Java_org_lichess_stockfish_CordovaPluginStockfishJNI_cmd(JNIEnv *env, jclass clazz, jstring jcmd) {
  const char *cmd = env->GetStringUTFChars(jcmd, (jboolean *)0);
  LOGD("stockfishcli", "cmd %s", cmd);
  stockfishcli::commandInit();
  stockfishcli::command(cmd);
  env->ReleaseStringUTFChars(jcmd, cmd);
}
