

ALLEGRO_PATH=/home/ksomervi/src/allegro-mingw-gcc6.2.0-x64-static-5.2.2/allegro


INCLUDES=-I$(ALLEGRO_PATH)/include

CXXFLAGS= -std=c++11 -Wall -g $(INCLUDES)

LDFLAGS=-L$(ALLEGRO_PATH)/lib 

LDFLAGS +=-lallegro_monolith-debug-static
LDFLAGS +=-ljpeg 
LDFLAGS +=-ldumb 
LDFLAGS +=-lFLAC 
LDFLAGS +=-lfreetype 
LDFLAGS +=-lvorbisfile 
LDFLAGS +=-lvorbis 
LDFLAGS +=-logg 
LDFLAGS +=-lphysfs 
LDFLAGS +=-lpng16 
LDFLAGS +=-lz
#LDFLAGS +=-lzlibstatic 
LDFLAGS +=-ldsound
LDFLAGS +=-lgdiplus 
LDFLAGS +=-luuid 
LDFLAGS +=-lkernel32 
LDFLAGS +=-lwinmm 
LDFLAGS +=-lpsapi 
LDFLAGS +=-lopengl32 
LDFLAGS +=-lglu32 
LDFLAGS +=-luser32 
LDFLAGS +=-lcomdlg32 
LDFLAGS +=-lgdi32 
LDFLAGS +=-lshell32 
LDFLAGS +=-lole32 
LDFLAGS +=-ladvapi32 
LDFLAGS +=-lws2_32 
LDFLAGS +=-lshlwapi 
#LDFLAGS +=-lmsvcrt 
LDFLAGS +=-static-libstdc++ 
LDFLAGS +=-static-libgcc
# Prevent console from being created
#LDFLAGS +=-mwindows
# Or you can use -Wl,-subsystem,windows
#
# The -Wl,<options> switch passes <options> to the linker. The -subsystem 
# switch tells the linker which system to target when generating the 
# executable.


CXX=x86_64-w64-mingw32-g++


