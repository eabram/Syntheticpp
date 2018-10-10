#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "lisasim-lisa.h"
#include "lisasim-tens.h"
#include "lisasim-wave.h"
#include "lisasim-tdinoise.h"
#include "lisasim-tdisignal.h"

#include "SimpleBinary.h"

#include <gsl/gsl_const_mksa.h>

int main(int argc, const char* argv[])
{

  const double c = GSL_CONST_MKSA_SPEED_OF_LIGHT;
  const double armlength = 2.5e9/c;
  
  EccentricInclined *lisamission = new EccentricInclined(armlength, 0,0,1,1);

  // create TDInoise object
  int stime = 8;
  // double st_laser_noise[6] = {1,1,1,1,1,1};
  // double st_shot_noise[6]  = {1,1,1,1,1,1};
  // double st_pm_noise[6]    = {1,1,1,1,1,1};

  // double sd_laser_noise[6] = {1,1,1,1,1,1};
  // double sd_shot_noise[6]  = {1,1,1,1,1,1};
  // double sd_pm_noise[6]    = {1,1,1,1,1,1};


  // double st_laser_noise[6] = {1,1,1,1,1,1};
  // double st_shot_noise[6]  = {1,1,1,1,1,1};
  // double st_pm_noise[6]    = {1,1,1,1,1,1};

  // double sd_laser_noise[6] = {0,0,0,0,0,0};
  // double sd_shot_noise[6]  = {1e-46, 1e-46, 1e-46, 1e-46, 1e-46, 1e-46};
  // double sd_pm_noise[6]    = {0,0,0,0,0,0};
  
  // TDIpointing *pointingTDI = new TDIpointing(lisamission,
  // 					     st_pm_noise,    sd_pm_noise,
  // 					     st_shot_noise,  sd_shot_noise,
  // 					     st_laser_noise, sd_laser_noise);
  
  SimpleBinary *mysystem = new SimpleBinary( 1.0e-4,
					     0.0,
					     0,
					     1.4,
					     0,
					     0,
					     0);

    // create TDInoise object
  TDIsignal *eccentricTDI = new TDIsignal(lisamission, mysystem);

  for (int i =0; i < 1e8; i+= 1e3)
    {
      Vector p1,p2;
      Vector u,v;
      lisamission->putp(p1, 1, i);
      lisamission->putp(p2, 2, i);
      lisamission->putn(v, 1, i);
      lisamission->putn(u, 2, i);
      double angle = dotproduct(u, v);

      std::cout
 	<< i
	<< "\t" << eccentricTDI->Xm(i)
	// << "\t" << lisamission->armlengthaccurate(1, i)
	// << "\t" << p1[0]
	// << "\t" << p1[1]
	// << "\t" << p1[2]
	// << "\t" << p2[0]
	// << "\t" << p2[1]
	// << "\t" << p2[2]
// 	      << "\t" << pointingTDI->y123(i)
// 	      << "\t" << pointingTDI->y231(i)
// 	      << "\t" << pointingTDI->y312(i)
// 	      << "\t" << pointingTDI->z123(i)
// 	      << "\t" << pointingTDI->z213(i)
//	      << "\t" << pointingTDI->z312(i)
	      << std::endl;
    }
      

   
}
