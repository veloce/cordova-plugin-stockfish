LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := stockfishcli
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../stockfish/
LOCAL_SRC_FILES := stockfishcli.cpp
LOCAL_CPP_EXTENSION := .cpp
LOCAL_SHARED_LIBRARIES := stockfish

include $(BUILD_STATIC_LIBRARY)
