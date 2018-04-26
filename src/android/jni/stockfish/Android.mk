LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

stockfish_src_files := \
    src/benchmark.cpp                             \
    src/bitbase.cpp                               \
    src/bitboard.cpp                              \
    src/endgame.cpp                               \
    src/evaluate.cpp                              \
    src/material.cpp                              \
    src/misc.cpp                                  \
    src/movegen.cpp                               \
    src/movepick.cpp                              \
    src/pawns.cpp                                 \
    src/position.cpp                              \
    src/psqt.cpp                                  \
    src/search.cpp                                \
    src/thread.cpp                                \
    src/timeman.cpp                               \
    src/tt.cpp                                    \
    src/uci.cpp                                   \
    src/ucioption.cpp                             \
    src/syzygy/tbprobe.cpp                        \

LOCAL_MODULE := stockfish
LOCAL_CPP_EXTENSION := .cpp
LOCAL_SRC_FILES := $(stockfish_src_files)

include $(BUILD_STATIC_LIBRARY)
