LOCAL_PATH := $(call my-dir)

ifeq ($(FIREBASE_CPP_SDK_DIR),)
$(error FIREBASE_CPP_SDK_DIR must specify the Firebase package location.)
endif

# With Firebase libraries for the selected build configuration (ABI + STL)
STL:=$(firstword $(subst _, ,$(APP_STL)))
FIREBASE_LIBRARY_PATH:=\
$(FIREBASE_CPP_SDK_DIR)/libs/android/$(TARGET_ARCH_ABI)/$(STL)

include $(CLEAR_VARS)
LOCAL_MODULE:=firebase_app
LOCAL_SRC_FILES:=$(FIREBASE_LIBRARY_PATH)/libapp.a
LOCAL_EXPORT_C_INCLUDES:=$(FIREBASE_CPP_SDK_DIR)/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:=firebase_admob
LOCAL_SRC_FILES:=$(FIREBASE_LIBRARY_PATH)/libadmob.a
LOCAL_EXPORT_C_INCLUDES:=$(FIREBASE_CPP_SDK_DIR)/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL


LOCAL_CPPFLAGS += -fexceptions -std=c++14  -Wall -DGLEW_NO_GLU -D_ANDROID_ -DGL_GLEXT_PROTOTYPES -g  


LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
                    $(LOCAL_PATH)/../../../../../common/include

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
    Source.cpp CardObject.cpp CardRenderer.cpp Game.cpp PostProcessor.cpp \
	Shader.cpp TextRenderer.cpp Texture.cpp ResourceManager.cpp GLFunctionLoader.cpp \
	CardBits.cpp \

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_mixer freetype2-static
#LOCAL_STATIC_LIBRARIES :=  
LOCAL_STATIC_LIBRARIES:=\
	firebase_admob \
	firebase_app

LOCAL_LDLIBS:= -lGLESv1_CM -lGLESv2 -lGLESv3 -llog -landroid -latomic
LOCAL_ARM_MODE:=arm
LOCAL_LDFLAGS:=-Wl,-z,defs -Wl,--no-undefined
# -lc++

# -ldl


include $(BUILD_SHARED_LIBRARY)