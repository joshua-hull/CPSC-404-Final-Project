#sample makefile for CPSC 4040/6040
#author: Joshua A. Levine (levinej@clemson.edu)

#Edited 09/28 for Lab 03 by Joshua Hull

#specify your compiler
CC = g++

#first set up the platform dependent variables
#you could also add another ifeq entry for your own platform
ifeq ("$(shell uname)", "Darwin")
  #Note that these paths are based on installing OpenImageIO with
  #homebrew, hence there are no needs for the include paths
  OIIO_INC = -I /usr/local/include
  OIIO_LIB = -L /usr/local/lib -lOpenImageIO
  OPENGL_LIB = -framework Foundation -framework GLUT -framework OpenGL
else
  ifeq ("$(shell uname)", "Linux")
    #On SoC machines, we need to specifically include the OIIO include
    #path in addition to the lib paths
    OIIO_INC = -I/group/dpa/include
    OIIO_LIB = -L/group/dpa/lib -lOpenImageIO
    OPENGL_LIB = -lglut -lGL -lGLU
  endif
endif

#build the LDFLAG snd CFLAGS based on variables above
LDFLAGS = $(OPENGL_LIB) $(OIIO_LIB)
CFLAGS = -g $(OIIO_INC)

#this will be the name of your executable
PROJECT1 = process

#list a .o file for each .cpp file that you will compile
#this makefile will compile each cpp separately before linking
OBJECTS1 = process.o pixel.o

#this target does the linking step
all: ${PROJECT1}

${PROJECT1}: ${OBJECTS1}
	${CC} -o ${PROJECT1} ${OBJECTS1} ${VECTOROBJS} ${LDFLAGS}

#this target generically compiles each .cpp to a .o file
#it does not check for .h files dependencies, but you could add that
%.o: %.cpp
	${CC} -c ${CFLAGS} *.cpp

#this will clean up all temporary files created by make all
clean:
	rm -f core.* *.o *~ ${PROJECT1}
