# include directories
INCLUDES = -I. -I./include/ -I/usr/local/include -I/home/ester/virenv/py2/lib/python2.7/site-packages/synthlisa/ -I/home/ester/virenv/py2/bin/gsl/include/gsl/ -I/home/ester/git/synthlisa/lisasim/GSL/

# C++ compiler flags (-g -O2 -Wall)
CCFLAGS = -g -O2 -Wall -I/home/ester/virenv/py2/bin/gsl/include/gsl/

# library paths -L/usr/local/lib 
LIBS = -L/home/ester/virenv/py2/lib/ -lgsl -lgslcblas -lm 


all: lib_code Lisa TDI


Lisa: lisa.o
	g++ -lm -o LISA lisa.o $(INCLUDES) -L./lib -lSynthetic $(LIBS)

lisa.o: lisa.cpp
	g++ -O -c lisa.cpp $(INCLUDES) -L./lib -lSynthetic $(LIBS)


TDI: tdi.o
	g++ -lm -o TDI tdi.o $(INCLUDES) -L./lib -lSynthetic $(LIBS)

tdi.o: tdi.cpp
	g++ -O -c tdi.cpp $(INCLUDES) -L./lib -lSynthetic $(LIBS)



Make.depend depend:
	$(CXX) $(INCLUDES) $(CCFLAGS) -M *.cpp > Make.depend

include Make.depend

.PHONY: lib_code

lib_code:
	cd src && make

.PHONY: clean

clean:
	rm -f *.o *.depend *~
