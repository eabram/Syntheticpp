# source files.
SRC = lisasim-lisa.cpp lisasim-signal.cpp lisasim-tdinoise.cpp \
	lisasim-wave.cpp lisasim-tdi.cpp lisasim-tdisignal.cpp \
	lisasim-retard.cpp  lisasim-tens.cpp SimpleBinary.cpp 

OBJ = $(SRC:.cpp=.o)
OUT = ../lib/libSynthetic.so

# include directories
INCLUDES = -I. -I../include/ -I/usr/local/include

# C++ compiler flags (-g -O2 -Wall)
CCFLAGS = -g -O2 -Wall

# compiler
CCC = g++

# library paths
LIBS = -L/usr/local/lib -lgsl -lgslcblas -lm

# compile flags
LDFLAGS = -shared

.SUFFIXES: .cpp

default: $(OUT)

.cpp.o:
	$(CCC) $(INCLUDES) $(CCFLAGS) $(LIBS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)

Make.depend depend:
	$(CCC) $(INCLUDES) $(CCFLAGS) $(LIBS) -M *.cpp > Make.depend

include Make.depend
