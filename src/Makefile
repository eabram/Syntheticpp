# source files.
SRC = lisasim-lisa.cpp lisasim-signal.cpp lisasim-tdinoise.cpp \
	lisasim-wave.cpp lisasim-tdi.cpp lisasim-tdisignal.cpp \
	lisasim-retard.cpp  lisasim-tens.cpp SimpleBinary.cpp 

OBJ = $(SRC:.cpp=.o)
OUT = ../lib/libSynthetic.so

# include directories
INCLUDES = -I. -I../include/ -I/usr/local/include -I/home/ester/virenv/py2/lib/python2.7/site-packages/synthlisa/ -I/home/ester/virenv/py2/bin/gsl/include/gsl/ -I/home/ester/git/synthlisa/lisasim/GSL/

# C++ compiler flags (-g -O2 -Wall)
CCFLAGS = -g -O2 -Wall

# compiler
CCC = g++

# library paths
LIBS = -L/home/ester/virenv/py2/lib -lgsl -lgslcblas -lm 

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
