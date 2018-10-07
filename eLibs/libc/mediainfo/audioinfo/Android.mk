LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(PLATFORM_VERSION),2.3.4)
AV_BASE_PATH := base
TEMP_COMPILE_DISABLE := false
endif
ifeq ($(PLATFORM_VERSION),4.0.1)
AV_BASE_PATH := base
TEMP_COMPILE_DISABLE := false
endif
ifeq ($(PLATFORM_VERSION),4.0.3)
AV_BASE_PATH := base
TEMP_COMPILE_DISABLE := false
endif
ifeq ($(PLATFORM_VERSION),4.0.4)
AV_BASE_PATH := base
TEMP_COMPILE_DISABLE := false
endif
ifeq ($(PLATFORM_VERSION),4.1.1)
AV_BASE_PATH := av
TEMP_COMPILE_DISABLE := true
endif
ifeq ($(PLATFORM_VERSION),4.2.1)
AV_BASE_PATH := av
TEMP_COMPILE_DISABLE := true
endif
ifeq ($(PLATFORM_VERSION),4.2.2)
AV_BASE_PATH := av
TEMP_COMPILE_DISABLE := true
endif

LOCAL_SRC_FILES := \
	AudioInfo_MID.c \
	Tag_VBRI.c \
	Tag_Enoding.c \
	AudioInfo_MP3.c \
	AudioInfo_FLAC.c \
	AudioInfo_AAC.c \
	Tag_ID3.c \
	AudioInfo_OGG.c \
	AudioInfo_AMR.c \
	AudioInfo_WAV.c \
	GetAudio_format.c \
	bitstream.c \
	Tag_OGG.c \
	Tag_APE.c \
	AudioInfo_WMA.c \
	Tag_XING.c \
	AudioInfo_ATRAC.c \
	AudioInfo_APE.c \
	AudioInfo_DTS.c \
	AudioInfo_AC3.c \
	AudioInfo_CAF.c \
	AudioInfo_RA.c
	 
	
LOCAL_C_INCLUDES := \
	$(TOP)/frameworks/${AV_BASE_PATH}/media/CedarX-Projects/CedarX/include

	


LOCAL_CFLAGS += -D__OS_LINUX -D__OS_ANDROID

LOCAL_MODULE_TAGS := optional
 
#LOCAL_CFLAGS += -O3
#LOCAL_CFLAGS += -march=armv6j

LOCAL_MODULE:= libGetAudio_format

include $(BUILD_STATIC_LIBRARY)

