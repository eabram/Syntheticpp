#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "lisasim-lisa.h"
#include "lisasim-tens.h"
#include "lisasim-wave.h"
#include "lisasim-tdisignal.h"
#include "SimpleBinary.h"

#include <gsl/gsl_blas.h>
#include <gsl/gsl_const_mksa.h>

// #include <boost/program_options.hpp>
// #include <boost/lexical_cast.hpp>

// namespace po = boost::program_options;
// using namespace std;


int main(int argc, const char* argv[])
{

  const double c = GSL_CONST_MKSA_SPEED_OF_LIGHT;
  const double armlength = 1e9/c;
  
  EccentricInclined *l = new EccentricInclined(armlength, 0,0,1,1);

  int link=1;
  double t = 10.0;
  
  //  std::cout << l->putn(v, link, t) << std::endl;
  SimpleBinary *mysystem = new SimpleBinary( 1.0e-4,
					     0.0,
					     0,
					     1.4,
					     0,
					     0,
					     0);

    // create TDInoise object
  int stime = 8;
  TDIsignal *eccentricTDI = new TDIsignal(l, mysystem);
  
  for (int i =0; i < 1e8 ; i+= 1e3)
    {
      std::cout << i << "\t" << eccentricTDI->Xm(i) << std::endl;
    }
  return 0;
}
