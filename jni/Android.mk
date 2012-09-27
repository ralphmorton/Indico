
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LIBPNG_SRC = libpng/png.c libpng/pngerror.c libpng/pngget.c libpng/pngmem.c libpng/pngpread.c libpng/pngread.c libpng/pngrio.c libpng/pngrtran.c libpng/pngrutil.c libpng/pngset.c libpng/pngtrans.c libpng/pngwio.c libpng/pngwrite.c libpng/pngwtran.c libpng/pngwutil.c
ZLIB_SRC = zlib/adler32.c zlib/compress.c zlib/crc32.c zlib/deflate.c zlib/inffast.c zlib/inflate.c zlib/inftrees.c zlib/trees.c zlib/uncompr.c zlib/zutil.c

LOCAL_CPPFLAGS  := -fexceptions -std=c++0x -D__STDC_INT64__
LOCAL_MODULE    := Indico

LOCAL_C_INCLUDES := \
	IndicoSharedPointer.h \
	IndicoLog.h \
	IndicoTimer.h \
	IndicoString.h \
	IndicoException.h \
	IndicoVector.h \
	IndicoHashMap.h \
	IndicoSingleton.h \
	IndicoTypeInfo.h \
	IndicoMath.h \
	IndicoMatrix.h \
	IndicoQuaternion.h \
	IndicoVector2.h \
	IndicoVector3.h \
	IndicoVector4.h \
	IndicoResource.h \
	IndicoResourceStream.h \
	IndicoResourceLocator.h \
	IndicoAndroidResourceLocator.h \
	IndicoResourceLoader.h \
	IndicoResourceManager.h \
	IndicoApp.h \
	IndicoEngine.h \
	Graphics/IndicoGL.h \
	Graphics/IndicoVertexLayout.h \
	Graphics/IndicoMesh.h \
	Graphics/IndicoTexture.h \
	Graphics/IndicoMaterial.h \
	Graphics/IndicoSkin.h \
	Graphics/IndicoModel.h \
	Graphics/IndicoShader.h \
	Graphics/IndicoProgram.h \
	Graphics/IndicoMeshLoader.h \
	Graphics/IndicoTextureLoader.h \
	Graphics/IndicoMaterialLoader.h \
	Graphics/IndicoSkinLoader.h \
	Graphics/IndicoShaderLoader.h \
	Graphics/IndicoRenderWorld.h

LOCAL_SRC_FILES := \
	$(LIBPNG_SRC) \
	$(ZLIB_SRC) \
	IndicoTimer.cpp \
	IndicoString.cpp \
	IndicoException.cpp \
	IndicoTypeInfo.cpp \
	IndicoMath.cpp \
	IndicoMatrix.cpp \
	IndicoQuaternion.cpp \
	IndicoVector2.cpp \
	IndicoVector3.cpp \
	IndicoVector4.cpp \
	IndicoResource.cpp \
	IndicoAndroidResourceLocator.cpp \
	IndicoResourceManager.cpp \
	IndicoEngine.cpp \
	Graphics/IndicoVertexLayout.cpp \
	Graphics/IndicoMesh.cpp \
	Graphics/IndicoTexture.cpp \
	Graphics/IndicoMaterial.cpp \
	Graphics/IndicoSkin.cpp \
	Graphics/IndicoModel.cpp \
	Graphics/IndicoShader.cpp \
	Graphics/IndicoProgram.cpp \
	Graphics/IndicoMeshLoader.cpp \
	Graphics/IndicoTextureLoader.cpp \
	Graphics/IndicoMaterialLoader.cpp \
	Graphics/IndicoSkinLoader.cpp \
	Graphics/IndicoShaderLoader.cpp \
	Graphics/IndicoRenderWorld.cpp \
	Indico.cpp

LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)