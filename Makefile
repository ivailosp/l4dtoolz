# (C)2004-2010 Metamod:Source Development Team
# Makefile written by David "BAILOPAN" Anderson

GIT_VERSION = $(shell sh -c 'git describe --abbrev=8 --dirty --always')

###########################################
### EDIT THESE PATHS FOR YOUR OWN SETUP ###
###########################################

HL2SDK_L4D = ../../hl2sdk-l4d
HL2SDK_L4D2 = ../../hl2sdk-l4d2
MMSOURCE19 = ..

#####################################
### EDIT BELOW FOR OTHER PROJECTS ###
#####################################

PROJECT = l4dtoolz_mm_i486
OBJECTS = l4dtoolz_mm.cpp signature.cpp

##############################################
### CONFIGURE ANY OTHER FLAGS/OPTIONS HERE ###
##############################################

OPT_FLAGS = -O3 -funroll-loops -pipe
GCC4_FLAGS = -fvisibility=hidden -fvisibility-inlines-hidden -std=gnu++0x
DEBUG_FLAGS = -g -ggdb3 -D_DEBUG
CPP = gcc
CPP_OSX = clang

##########################
### SDK CONFIGURATIONS ###
##########################

override ENGSET = false

# Check for valid list of engines
ifneq (,$(filter left4dead left4dead2,$(ENGINE)))
	override ENGSET = true
endif

ifeq "$(ENGINE)" "left4dead"
	HL2SDK = $(HL2SDK_L4D)
	CFLAGS += -DSOURCE_ENGINE=8 -DL4D1
endif
ifeq "$(ENGINE)" "left4dead2"
	HL2SDK = $(HL2SDK_L4D2)
	CFLAGS += -DSOURCE_ENGINE=9 -DL4D2
endif

ifneq "$(GIT_VERSION)" ""
	CFLAGS += -D__GIT_VERSION=\"$(GIT_VERSION)\"
endif

HL2PUB = $(HL2SDK)/public


INCLUDE += -I$(HL2SDK)/public/game/server
METAMOD = $(MMSOURCE19)/core

OS := $(shell uname -s)

ifeq "$(OS)" "Darwin"
	LIB_EXT = dylib
	HL2LIB = $(HL2SDK)/lib/mac
else
	LIB_EXT = so
	HL2LIB = $(HL2SDK)/lib/linux
endif


LIB_PREFIX = lib
ifneq (,$(filter left4dead2,$(ENGINE)))
	ifneq "$(OS)" "Darwin"
		LIB_SUFFIX = _srv.$(LIB_EXT)
	else
		LIB_SUFFIX = .$(LIB_EXT)
	endif
else
	LIB_SUFFIX = .$(LIB_EXT)
endif


CFLAGS += -DSE_EPISODEONE=1 -DSE_DARKMESSIAH=2 -DSE_ORANGEBOX=3 -DSE_BLOODYGOODTIME=4 -DSE_EYE=5 \
	-DSE_CSS=6 -DSE_ORANGEBOXVALVE=7 -DSE_LEFT4DEAD=8 -DSE_LEFT4DEAD2=9 -DSE_ALIENSWARM=10 \
	-DSE_PORTAL2=11 -DSE_CSGO=12

LINK += $(HL2LIB)/tier1_i486.a $(LIB_PREFIX)vstdlib$(LIB_SUFFIX) $(LIB_PREFIX)tier0$(LIB_SUFFIX)

INCLUDE += -I. -I.. -I$(HL2PUB) -I$(HL2PUB)/engine -I$(HL2PUB)/mathlib -I$(HL2PUB)/vstdlib \
	-I$(HL2PUB)/tier0 -I$(HL2PUB)/tier1 -I. -I$(METAMOD) -I$(METAMOD)/sourcehook

################################################
### DO NOT EDIT BELOW HERE FOR MOST PROJECTS ###
################################################

BINARY = $(PROJECT).$(LIB_EXT)

ifeq "$(DEBUG)" "true"
	BIN_DIR = Debug.$(ENGINE)
	CFLAGS += $(DEBUG_FLAGS)
else
	BIN_DIR = Release.$(ENGINE)
	CFLAGS += $(OPT_FLAGS)
endif


ifeq "$(OS)" "Darwin"
	CPP = $(CPP_OSX)
	LIB_EXT = dylib
	CFLAGS += -DOSX -D_OSX
	LINK += -dynamiclib -lstdc++ -mmacosx-version-min=10.5
else
	LIB_EXT = so
	CFLAGS += -D_LINUX
	LINK += -shared
endif

IS_CLANG := $(shell $(CPP) --version | head -1 | grep clang > /dev/null && echo "1" || echo "0")

ifeq "$(IS_CLANG)" "1"
	CPP_MAJOR := $(shell $(CPP) --version | grep clang | sed "s/.*version \([0-9]\)*\.[0-9]*.*/\1/")
	CPP_MINOR := $(shell $(CPP) --version | grep clang | sed "s/.*version [0-9]*\.\([0-9]\)*.*/\1/")
else
	CPP_MAJOR := $(shell $(CPP) -dumpversion >&1 | cut -b1)
	CPP_MINOR := $(shell $(CPP) -dumpversion >&1 | cut -b3)
endif

CFLAGS += -DPOSIX -Dstricmp=strcasecmp -D_stricmp=strcasecmp -D_strnicmp=strncasecmp \
	-Dstrnicmp=strncasecmp -D_snprintf=snprintf -D_vsnprintf=vsnprintf -D_alloca=alloca \
	-Dstrcmpi=strcasecmp -DCOMPILER_GCC -Wall -Wno-non-virtual-dtor -Wno-overloaded-virtual \
	-Werror -fPIC -fno-exceptions -fno-rtti -msse -m32 -fno-strict-aliasing

# Clang || GCC >= 4
ifeq "$(shell expr $(IS_CLANG) \| $(CPP_MAJOR) \>= 4)" "1"
	CFLAGS += $(GCC4_FLAGS)
endif

# Clang >= 3 || GCC >= 4.7
ifeq "$(shell expr $(IS_CLANG) \& $(CPP_MAJOR) \>= 3 \| $(CPP_MAJOR) \>= 4 \& $(CPP_MINOR) \>= 7)" "1"
	CFLAGS += -Wno-delete-non-virtual-dtor
endif

# OS is Linux and not using clang
ifeq "$(shell expr $(OS) \= Linux \& $(IS_CLANG) \= 0)" "1"
	LINK += -static-libgcc
endif

OBJ_BIN := $(OBJECTS:%.cpp=$(BIN_DIR)/%.o)

$(BIN_DIR)/%.o: %.cpp
	$(CPP) $(INCLUDE) $(CFLAGS) -o $@ -c $<

all: check
	mkdir -p $(BIN_DIR)
	ln -sf $(HL2LIB)/$(LIB_PREFIX)vstdlib$(LIB_SUFFIX)
	ln -sf $(HL2LIB)/$(LIB_PREFIX)tier0$(LIB_SUFFIX)
	$(MAKE) -f Makefile l4dtoolz_mm
	
check:
	if [ "$(ENGSET)" = "false" ]; then \
		echo "You must supply one of the following values for ENGINE:"; \
		echo "left4dead2 or left4dead"; \
		exit 1; \
	fi

l4dtoolz_mm: check $(OBJ_BIN)
	$(CPP) $(INCLUDE) -m32 $(OBJ_BIN) $(LINK) -ldl -lm -o $(BIN_DIR)/$(BINARY)

default: all

clean: check
	rm -rf $(BIN_DIR)/*.o
	rm -rf $(BIN_DIR)/$(BINARY)

