# Generated Makefile -- DO NOT EDIT!

CC=/Users/daniel/MagicLeap/mlsdk/v0.19.0/tools/toolchains/bin/aarch64-linux-android-clang
COMPILER_PREFIX=
CXX=/Users/daniel/MagicLeap/mlsdk/v0.19.0/tools/toolchains/bin/aarch64-linux-android-clang++
CameraLib_BASE=/Users/daniel/Documents/MLProjects/CameraLib
CameraLib_CPPFLAGS=-I$(CameraLib_BASE)/inc/
CameraLib_CXXFLAGS=--sysroot=$(MLSDK)/lumin -march=armv8-a -mcpu=cortex-a57+crypto -fPIC -fpic -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -Wa,--noexecstack -Wformat -Werror=format-security -Wno-invalid-command-line-argument -Wno-unused-command-line-argument -g -fno-limit-debug-info -O0 -std=c++11 -nostdinc++ -I$(MLSDK)/lumin/stl/gnu-libstdc++/include -I$(MLSDK)/lumin/stl/gnu-libstdc++/include/aarch64-linux-android -I$(MLSDK)/lumin/stl/gnu-libstdc++/include/backward
CameraLib_LDFLAGS=--sysroot=$(MLSDK)/lumin -Wl,--warn-shared-textrel -Wl,--fatal-warnings -Wl,--build-id -no-canonical-prefixes -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -Wl,--enable-new-dtags '-Wl,-rpath=$$ORIGIN' -g -L$(MLSDK)/lumin/stl/gnu-libstdc++/lib
CameraLib_LIBS=-Wl,--no-as-needed -Wl,--start-group -Bdynamic -lc -lm -lgnustl_shared -Wl,--end-group
CameraLib_OUTPUT=/Users/daniel/Documents/MLProjects/CameraLib/Temp/CameraLib/$(SPEC)
HOST=osx
LINKER_PREFIX=
OBJCOPY=/Users/daniel/MagicLeap/mlsdk/v0.19.0/tools/toolchains/bin/aarch64-linux-android-objcopy
RM=rm
SPEC=debug_lumin_clang-3.8_aarch64
STRIP=/Users/daniel/MagicLeap/mlsdk/v0.19.0/tools/toolchains/bin/aarch64-linux-android-strip

$(CameraLib_OUTPUT)/libCameraLib.so : $(CameraLib_OUTPUT)/obj.CameraLib/src/CameraLib.cpp.o $(CameraLib_BASE)/CameraLib.mabu $(MLSDK)/tools/mabu/data/configs/debug.config $(MLSDK)/tools/mabu/data/options/debug/on.option $(MLSDK)/tools/mabu/data/options/magicleap.option $(MLSDK)/tools/mabu/data/options/optimize/off.option $(MLSDK)/tools/mabu/data/options/package/debuggable/on.option $(MLSDK)/tools/mabu/data/options/runtime/shared.option $(MLSDK)/tools/mabu/data/options/standard-c++/11.option $(MLSDK)/tools/mabu/data/options/stl/libgnustl.option $(MLSDK)/tools/mabu/data/options/warn/on.option
	$(INFO) \[CameraLib\]\ Linking\ shared\ object\ 'libCameraLib.so'...
	$(ECHO) cd $(CameraLib_OUTPUT) && $(LINKER_PREFIX) $(CC) -o libCameraLib.so -shared obj.CameraLib/src/CameraLib.cpp.o $(CameraLib_LIBS) $(CameraLib_LDFLAGS)
	$(ECHO) $(OBJCOPY) --only-keep-debug $(CameraLib_OUTPUT)/libCameraLib.so $(CameraLib_OUTPUT)/libCameraLib.sym
	$(ECHO) $(OBJCOPY) --add-gnu-debuglink $(CameraLib_OUTPUT)/libCameraLib.sym $(CameraLib_OUTPUT)/libCameraLib.so $(CameraLib_OUTPUT)/libCameraLib.so
	$(ECHO) $(STRIP) --strip-unneeded $(CameraLib_OUTPUT)/libCameraLib.so

$(CameraLib_OUTPUT)/obj.CameraLib/src/CameraLib.cpp.o : $(CameraLib_BASE)/src/CameraLib.cpp $(CameraLib_BASE)/CameraLib.mabu $(MLSDK)/tools/mabu/data/configs/debug.config $(MLSDK)/tools/mabu/data/options/debug/on.option $(MLSDK)/tools/mabu/data/options/magicleap.option $(MLSDK)/tools/mabu/data/options/optimize/off.option $(MLSDK)/tools/mabu/data/options/package/debuggable/on.option $(MLSDK)/tools/mabu/data/options/runtime/shared.option $(MLSDK)/tools/mabu/data/options/standard-c++/11.option $(MLSDK)/tools/mabu/data/options/stl/libgnustl.option $(MLSDK)/tools/mabu/data/options/warn/on.option
	$(INFO) \[CameraLib\]\ Compiling\ 'CameraLib.cpp'...
	$(ECHO) $(COMPILER_PREFIX) $(CXX) -c $(CameraLib_BASE)/src/CameraLib.cpp -o $(CameraLib_OUTPUT)/obj.CameraLib/src/CameraLib.cpp.o -MD -MP -MF $(CameraLib_OUTPUT)/obj.CameraLib/src/CameraLib.cpp.d $(CameraLib_CPPFLAGS) $(CameraLib_CXXFLAGS)
	$(ECHO) echo $(CameraLib_BASE)/src/CameraLib.cpp : >> $(CameraLib_OUTPUT)/obj.CameraLib/src/CameraLib.cpp.d

-include $(CameraLib_OUTPUT)/obj.CameraLib/src/CameraLib.cpp.d

CameraLib-prebuild :: 

CameraLib-postbuild :: 

CameraLib-clean :: 
	$(INFO) Cleaning\ CameraLib...
	$(ECHO) $(RM) -rf $(CameraLib_OUTPUT)/libCameraLib.so $(CameraLib_OUTPUT)/CameraLib.sym $(CameraLib_OUTPUT)/obj.CameraLib/src/CameraLib.cpp.o $(CameraLib_OUTPUT)/obj.CameraLib/src/CameraLib.cpp.d $(CameraLib_OUTPUT)/bin/libgnustl_shared.so

CameraLib-all :: -make-directories CameraLib-prebuild $(CameraLib_OUTPUT)/libCameraLib.so CameraLib-postbuild

PROJECT=CameraLib

