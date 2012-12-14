# (C)2004-2008 Metamod:Source Development Team
# Makefile written by David "BAILOPAN" Anderson

###########################################
### EDIT THESE PATHS FOR YOUR OWN SETUP ###
###########################################

HL2SDK_L4D = ../../hl2sdk-l4d
HL2SDK_L4D2 = ../../hl2sdk-l4d2
MMSOURCE18 = ../
SRCDS_BASE = ~/srcds

#####################################
### EDIT BELOW FOR OTHER PROJECTS ###
#####################################

OBJECTS = l4dtoolz_mm.cpp
BINARY = l4dtoolz_mm_i486.so

##############################################
### CONFIGURE ANY OTHER FLAGS/OPTIONS HERE ###
##############################################
override ENGSET = false

OPT_FLAGS = -Os -s -pipe
ifeq "$(ENGINE)" "left4dead"
GCC4_FLAGS = -fvisibility=hidden -fvisibility-inlines-hidden -DL4D1
else
GCC4_FLAGS = -fvisibility=hidden -fvisibility-inlines-hidden -DL4D2
endif
DEBUG_FLAGS = -g -ggdb3 -D_DEBUG
CPP = gcc-4.1


ifeq "$(ENGINE)" "left4dead"
	HL2SDK = $(HL2SDK_L4D)
	HL2PUB = $(HL2SDK)/public
	HL2LIB = $(HL2SDK)/lib/linux
	CFLAGS += -DSOURCE_ENGINE=4
	METAMOD = $(MMSOURCE18)/core
	INCLUDE += -I$(HL2SDK)/public/game/server
	SRCDS = $(SRCDS_BASE)/
	override ENGSET = true
else
	HL2SDK = $(HL2SDK_L4D2)
	HL2PUB = $(HL2SDK)/public
	HL2LIB = $(HL2SDK)/lib/linux
	CFLAGS += -DSOURCE_ENGINE=4
	METAMOD = $(MMSOURCE18)/core
	INCLUDE += -I$(HL2SDK)/public/game/server
	SRCDS = $(SRCDS_BASE)/
	override ENGSET = true
endif

CFLAGS += -DSE_EPISODEONE=1 -SE_DARKMESSIAH=2 -DSE_ORANGEBOX=3 -DSE_LEFT4DEAD=4

ifeq "$(ENGINE)" "left4dead"
LINK += $(HL2LIB)/tier1_i486.a vstdlib_i486.so tier0_i486.so -static-libgcc
else
LINK += $(HL2LIB)/tier1_i486.a libvstdlib.so libtier0.so -static-libgcc
endif
INCLUDE += -I. -I.. -I$(HL2PUB) -I$(HL2PUB)/engine -I$(HL2PUB)/mathlib -I$(HL2PUB)/vstdlib \
	-I$(HL2PUB)/tier0 -I$(HL2PUB)/tier1 -I. -I$(METAMOD) -I$(METAMOD)/sourcehook
	
################################################
### DO NOT EDIT BELOW HERE FOR MOST PROJECTS ###
################################################

ifeq "$(DEBUG)" "true"
	BIN_DIR = Debug.$(ENGINE)
	CFLAGS += $(DEBUG_FLAGS)
else
	BIN_DIR = Release.$(ENGINE)
	CFLAGS += $(OPT_FLAGS)
endif

GCC_VERSION := $(shell $(CPP) -dumpversion >&1 | cut -b1)

CFLAGS += -D_LINUX -Dstricmp=strcasecmp -D_stricmp=strcasecmp -D_strnicmp=strncasecmp \
	-Dstrnicmp=strncasecmp -D_snprintf=snprintf -D_vsnprintf=vsnprintf -D_alloca=alloca \
	-Dstrcmpi=strcasecmp -Wall -Wno-non-virtual-dtor -Werror -fPIC -fno-exceptions \
	-fno-rtti -msse -m32 -fno-strict-aliasing

ifeq "$(GCC_VERSION)" "4"
	CFLAGS += $(GCC4_FLAGS)
endif

OBJ_LINUX := $(OBJECTS:%.cpp=$(BIN_DIR)/%.o)

$(BIN_DIR)/%.o: %.cpp
	$(CPP) $(INCLUDE) $(CFLAGS) -o $@ -c $<

all: check
	mkdir -p $(BIN_DIR)
ifeq "$(ENGINE)" "left4dead"
	rm -f vstdlib_i486.so
	rm -f tier0_i486.so
	ln -sf $(SRCDS)/bin/vstdlib_i486.so vstdlib_i486.so
	ln -sf $(SRCDS)/bin/tier0_i486.so tier0_i486.so

else
	rm -f libvstdlib.so
	rm -f libtier0.so
	ln -sf $(SRCDS)/bin/libvstdlib.so libvstdlib.so
	ln -sf $(SRCDS)/bin/libtier0.so libtier0.so
endif
	$(MAKE) -f Makefile l4dtoolz_mm
	
check:
	if [ "$(ENGSET)" = "false" ]; then \
		echo "You must supply ENGINE=left4dead or ENGINE=left4dead2"; \
		exit 1; \
	fi

l4dtoolz_mm: check $(OBJ_LINUX)
	$(CPP) $(INCLUDE) -m32 $(OBJ_LINUX) $(LINK) -shared -ldl -lm -o$(BIN_DIR)/$(BINARY)

default: all

clean: check
	rm -rf $(BIN_DIR)/*.o
	rm -rf $(BIN_DIR)/$(BINARY)

