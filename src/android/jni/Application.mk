APP_PLATFORM := android-14
APP_STL := c++_shared
APP_ABI := armeabi-v7a x86
APP_OPTIM := release
APP_CPPFLAGS += -std=c++11 -fexceptions -DHAVE_PTHREAD=1 -DKOTH -DTHREECHECK -DATOMIC -DRACE -DHORDE
APP_USE_CPP0X := true
NDK_TOOLCHAIN_VERSION := 4.9
