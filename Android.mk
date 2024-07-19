LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := native_renderer
LOCAL_SRC_FILES := main.c shader_utils.c
LOCAL_LDLIBS    := -llog -lGLESv2 -landroid -lEGL

include $(BUILD_SHARED_LIBRARY)
