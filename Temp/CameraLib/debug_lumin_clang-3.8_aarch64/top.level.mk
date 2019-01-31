# Generated Makefile -- DO NOT EDIT!

CameraLib_OUTPUT=/Users/daniel/Documents/MLProjects/CameraLib/Temp/CameraLib/$(SPEC)
HOST=osx
SPEC=debug_lumin_clang-3.8_aarch64


# this turns off the suffix rules built into make
.SUFFIXES:

# this turns off the RCS / SCCS implicit rules of GNU Make
% : RCS/%,v
% : RCS/%
% : %,v
% : s.%
% : SCCS/s.%

# If a rule fails, delete $@.
.DELETE_ON_ERROR:

ifeq ($(VERBOSE),)
ECHO=@
else
ECHO=
endif

ifeq ($(QUIET),)
INFO=@echo
else
INFO=@:
endif

all: prebuild build postbuild


prebuild :: 

postbuild :: 

clean :: CameraLib-clean

$(MLSDK)/tools/mabu/data/options/debug/on.option : 

$(MLSDK)/tools/mabu/data/options/runtime/shared.option : 

$(MLSDK)/tools/mabu/data/options/standard-c++/11.option : 

$(MLSDK)/tools/mabu/data/options/warn/on.option : 

$(MLSDK)/tools/mabu/data/options/package/debuggable/on.option : 

$(MLSDK)/tools/mabu/data/options/stl/libgnustl.option : 

$(MLSDK)/tools/mabu/data/options/magicleap.option : 

$(MLSDK)/tools/mabu/data/configs/debug.config : 

$(MLSDK)/tools/mabu/data/options/optimize/off.option : 

PROGRAM_PREFIX=
PROGRAM_EXT=
SHARED_PREFIX=lib
SHARED_EXT=.so
IMPLIB_PREFIX=lib
IMPLIB_EXT=.so
STATIC_PREFIX=lib
STATIC_EXT=.a
COMPILER_PREFIX=
LINKER_PREFIX=

-make-directories : /Users/daniel/Documents/MLProjects/CameraLib/Temp/CameraLib/debug_lumin_clang-3.8_aarch64 /Users/daniel/Documents/MLProjects/CameraLib/Temp/CameraLib/debug_lumin_clang-3.8_aarch64/bin /Users/daniel/Documents/MLProjects/CameraLib/Temp/CameraLib/debug_lumin_clang-3.8_aarch64/obj.CameraLib/src

/Users/daniel/Documents/MLProjects/CameraLib/Temp/CameraLib/debug_lumin_clang-3.8_aarch64 : 
	$(ECHO) @mkdir -p /Users/daniel/Documents/MLProjects/CameraLib/Temp/CameraLib/debug_lumin_clang-3.8_aarch64

/Users/daniel/Documents/MLProjects/CameraLib/Temp/CameraLib/debug_lumin_clang-3.8_aarch64/bin : 
	$(ECHO) @mkdir -p /Users/daniel/Documents/MLProjects/CameraLib/Temp/CameraLib/debug_lumin_clang-3.8_aarch64/bin

/Users/daniel/Documents/MLProjects/CameraLib/Temp/CameraLib/debug_lumin_clang-3.8_aarch64/obj.CameraLib/src : 
	$(ECHO) @mkdir -p /Users/daniel/Documents/MLProjects/CameraLib/Temp/CameraLib/debug_lumin_clang-3.8_aarch64/obj.CameraLib/src

include $(CameraLib_OUTPUT)/CameraLib.mk
build :  | CameraLib-all

