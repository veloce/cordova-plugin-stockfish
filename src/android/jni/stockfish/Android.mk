LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

stockfish_src_files := \
    benchmark.cpp                                                \
    bitbase.cpp                                                  \
    bitboard.cpp                                                 \
    endgame.cpp                                                  \
    evaluate.cpp                                                 \
    material.cpp                                                 \
    misc.cpp                                                     \
    movegen.cpp                                                  \
    movepick.cpp                                                 \
    pawns.cpp                                                    \
    position.cpp                                                 \
    psqt.cpp                                                     \
    search.cpp                                                   \
    thread.cpp                                                   \
    timeman.cpp                                                  \
    tt.cpp                                                       \
    uci.cpp                                                      \
    ucioption.cpp                                                \
    syzygy/tbprobe.cpp                                           \

LOCAL_MODULE := stockfish
LOCAL_CPP_EXTENSION := .cpp
LOCAL_SRC_FILES := $(stockfish_src_files)

include $(BUILD_STATIC_LIBRARY)
