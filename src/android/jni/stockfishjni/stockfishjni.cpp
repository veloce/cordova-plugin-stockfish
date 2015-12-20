#include <jni.h>
#include <string>
#include <stockfishcli.h>
#include <search.cpp>
#include <android/log.h>

#define LOGD(TAG,...) __android_log_print(ANDROID_LOG_DEBUG  , TAG,__VA_ARGS__)

extern "C" {
  JNIEXPORT void JNICALL Java_org_lichess_stockfish_CordovaPluginStockfish_jniInit(JNIEnv *env, jobject obj);
  JNIEXPORT void JNICALL Java_org_lichess_stockfish_CordovaPluginStockfish_jniExit(JNIEnv *env, jobject obj);
  JNIEXPORT void JNICALL Java_org_lichess_stockfish_CordovaPluginStockfish_jniCmd(JNIEnv *env, jobject obj, jstring jcmd);
};

static JavaVM *jvm;
static jobject jobj;
static jmethodID f;

bool run = false;

auto readstdout = []() {
  JNIEnv *jenv;

  jvm->GetEnv((void **)&jenv, JNI_VERSION_1_6);
  jvm->AttachCurrentThread(&jenv, (void*) NULL);

  std::streambuf* out = std::cout.rdbuf();
  std::ostringstream lichout;
  std::cout.rdbuf(lichout.rdbuf());

  run = true;

  while(run) {
    std::string output = lichout.str();
    lichout.str("");

    if(output.length() > 0) {
      jenv->CallVoidMethod(jobj, f, jenv->NewStringUTF(output.c_str()));
    }
  };

  std::cout.rdbuf(out);

  jvm->DetachCurrentThread();
};

std::thread reader;

JNIEXPORT void JNICALL Java_org_lichess_stockfish_CordovaPluginStockfish_jniInit(JNIEnv *env, jobject obj) {
  jobj = env->NewGlobalRef(obj);
  env->GetJavaVM(&jvm);
  jclass classStockfish = env->GetObjectClass(obj);
  f = env->GetMethodID(classStockfish, "f", "(Ljava/lang/String;)V");

  reader = std::thread(readstdout);

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

JNIEXPORT void JNICALL Java_org_lichess_stockfish_CordovaPluginStockfish_jniExit(JNIEnv *env, jobject obj) {
  run = false;
}

JNIEXPORT void JNICALL Java_org_lichess_stockfish_CordovaPluginStockfish_jniCmd(JNIEnv *env, jobject obj, jstring jcmd) {
  const char *cmd = env->GetStringUTFChars(jcmd, (jboolean *)0);
  LOGD("stockfishcli", "cmd %s", cmd);
  stockfishcli::commandInit();
  stockfishcli::command(cmd);
  env->ReleaseStringUTFChars(jcmd, cmd);
}
