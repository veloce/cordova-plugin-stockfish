APP_PLATFORM := android-16
APP_STL := c++_shared
APP_ABI := armeabi-v7a x86
APP_OPTIM := release
APP_CPPFLAGS += -std=c++11 -fexceptions -DHAVE_PTHREAD=1 -DKOTH -DTHREECHECK
APP_USE_CPP0X := true
NDK_TOOLCHAIN_VERSION := 4.9
