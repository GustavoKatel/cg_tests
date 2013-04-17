CC = g++
CC_C = gcc
CFLAGS += -Wall 

LIBS += -lm -lglut -lGLU -lGL -I.
	
#OBJS = list.o objLoader.o obj_parser.o string_extra.o \
#	object.o #target.o camera.o util.o level.o

SRCS_C = $(shell ls *c)
SRCS_CPP = $(shell ls *cpp | grep -v "main.cpp")
OBJS = ${SRCS_C:.c=.o}
OBJS += ${SRCS_CPP:.cpp=.o}

all: $(OBJS) main.cpp
	$(CC) main.cpp $(CFLAGS) $(OBJS) $(LIBS) -o cg_tests

debug: CFLAGS += -g
debug: all	

%.o: %.c
	$(CC_C) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o
