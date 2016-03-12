#include <jni.h>
#include <string>
#include <stockfishcli.h>
#include <threadbuf.h>
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
static jmethodID onMessage;

bool run = false;

auto readstdout = []() {
  JNIEnv *jenv;

  jvm->GetEnv((void **)&jenv, JNI_VERSION_1_6);
  jvm->AttachCurrentThread(&jenv, (void*) NULL);

  std::streambuf* out = std::cout.rdbuf(); // save output standard

  threadbuf lichbuf;
  std::ostream lichout(&lichbuf);
  std::cout.rdbuf(lichout.rdbuf());
  std::istream lichin(&lichbuf);

  run = true;

  while(run) {
    std::string line;
    std::getline(lichin, line);

    const char* coutput = line.c_str();
    int len = strlen(coutput);
    jbyteArray aoutput = jenv->NewByteArray(len);
    jenv->SetByteArrayRegion (aoutput, 0, len, (jbyte*)coutput);
    jenv->CallVoidMethod(jobj, onMessage, aoutput);
  };


  std::cout.rdbuf(out); // restore output standard
  lichbuf.close();

  jvm->DetachCurrentThread();
};

std::thread reader;

JNIEXPORT void JNICALL Java_org_lichess_stockfish_CordovaPluginStockfish_jniInit(JNIEnv *env, jobject obj) {
  jobj = env->NewGlobalRef(obj);
  env->GetJavaVM(&jvm);
  jclass classStockfish = env->GetObjectClass(obj);
  onMessage = env->GetMethodID(classStockfish, "onMessage", "([B)V");

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
  stockfishcli::commandInit();
  stockfishcli::command(cmd);
  env->ReleaseStringUTFChars(jcmd, cmd);
}
