# Makefile for invader game
# Copyright Kevin Somervill
# August 3, 2018

HOST=$(shell uname -o)

ifeq "$(HOST)" "Msys"
	include mingw64.mk
else

INCLUDE = -I../../allegro-mingw-gcc6.2.0-x64-static-5.2.2/allegor/include
LDFLAGS =-lallegro -lallegro_image -lallegro_primitives
#LDFLAGS += -lallegro_dialog
LDFLAGS += -lallegro_font
LDFLAGS += -lallegro_ttf

CXX = g++

CXXFLAGS=-std=c++11 -Wall -g
endif

SRCS = main.cpp game.cpp base_level.cpp level_1.cpp basic_object.cpp fighter.cpp
SRCS += logger.cpp point_2d.cpp
OBJS = $(subst .cpp,.o,${SRCS})

COMPILE = ${CXX} ${CXXFLAGS}

TARGET = invader

.cpp.o:
	${COMPILE} -c $?

${TARGET}: ${OBJS}
	${COMPILE} -o $@ ${OBJS} ${LDFLAGS}

sandbox: sandbox.o basic_object.o fighter.o point_2d.o
	${COMPILE} -o $@ $^ ${LDFLAGS}

neat:
	-rm -f *.o *~

clean: neat
	-rm -f ${TARGET} sandbox

