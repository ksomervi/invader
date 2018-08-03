LDFLAGS =-lallegro -lallegro_image -lallegro_primitives
#LDFLAGS += -lallegro_dialog
LDFLAGS += -lallegro_font
LDFLAGS += -lallegro_ttf

TARGET = invader

SRCS = main.cpp game.cpp base_level.cpp level_1.cpp basic_object.cpp fighter.cpp

OBJS = $(subst .cpp,.o,${SRCS})

CXX = g++

CXXFLAGS=-std=c++11 -Wall -g

#INCLUDE = -I../src

COMPILE = ${CXX} ${CXXFLAGS}

.cpp.o:
	${COMPILE} -c $? 

${TARGET}: ${OBJS}
	${COMPILE} -o $@ ${OBJS} ${LDFLAGS}

neat:
	-rm -f *.o *~

clean: neat
	-rm ${TARGET}

