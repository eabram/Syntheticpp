/* $Id$
 * $Date$
 * $Author$
 * $Revision$
 */

#include "lisasim-tdinoise.h"
#include "lisasim-except.h"

#include <time.h>
#include <iostream>

#include <gsl/gsl_rng.h>


// this version takes the parameters of the basic noises and lets us allocate objects as needed

TDInoise::TDInoise(LISA *mylisa, double stproof, double sdproof, double stshot, double sdshot, double stlaser, double sdlaser) {
    phlisa = mylisa->physlisa();
    lisa = mylisa;

    // allocate noise objects

    // set the global random-number seed if it is not set already
    WhiteNoiseSource::getglobalseed();

    for(int craft = 1; craft <= 3; craft++) {
      pm[craft] = stdproofnoise(lisa,stproof,sdproof);
      pms[craft] = stdproofnoise(lisa,stproof,sdproof);
    }
        
    for(int craft1 = 1; craft1 <= 3; craft1++) {
      for(int craft2 = 1; craft2 <= 3; craft2++) {
	if(craft1 != craft2)
	  shot[craft1][craft2] = stdopticalnoise(lisa,stshot,sdshot);
      }
    }

    for(int craft = 1; craft <= 3; craft++) {
      c[craft] = stdlasernoise(lisa,stlaser,sdlaser);
      cs[craft] = stdlasernoise(lisa,stlaser,sdlaser);
    }

    allocated = 1;
}

// this version takes arrays of basic-noise parameters, allowing for different noises on different objects,
// and lets us allocate objects as needed

TDInoise::TDInoise(LISA *mylisa, double *stproof, double *sdproof, double *stshot, double *sdshot, double *stlaser, double *sdlaser) {
    phlisa = mylisa->physlisa();
    lisa = mylisa;

    // allocate noise objects
    // the convention is {1,1*,2,2*,3,3*}, and {12,21,23,32,31,13}

    // set the global random-number seed if it is not set already
    WhiteNoiseSource::getglobalseed();

    for(int craft = 1; craft <= 3; craft++) {
      pm[craft]  = stdproofnoise(lisa,stproof[2*(craft-1)],  sdproof[2*(craft-1)]);
      pms[craft] = stdproofnoise(lisa,stproof[2*(craft-1)+1],sdproof[2*(craft-1)+1]);
    }
        
    for(int craft1 = 1; craft1 <= 3; craft1++) {
      for(int craft2 = 1; craft2 <= 3; craft2++) {
	if(craft1 != craft2) {
	  if( (craft1 == 1 && craft2 == 2) || (craft1 == 2 && craft2 == 3) || (craft1 == 3 && craft2 == 1) )
	    shot[craft1][craft2] = stdopticalnoise(lisa,stshot[2*(craft1-1)],  sdshot[2*(craft1-1)]);
	  else
	    shot[craft1][craft2] = stdopticalnoise(lisa,stshot[2*(craft2-1)+1],sdshot[2*(craft2-1)+1]);
	}
      }
    }

    for(int craft = 1; craft <= 3; craft++) {
      c[craft]  = stdlasernoise(lisa,stlaser[2*(craft-1)],  sdlaser[2*(craft-1)]);
      cs[craft] = stdlasernoise(lisa,stlaser[2*(craft-1)+1],sdlaser[2*(craft-1)+1]);
    }
    
    allocated = 1;
}

// this version takes pointers to noise objects, allowing for user-specified noises on different objects
TDInoise::TDInoise(LISA *mylisa, Noise *proofnoise[6],Noise *shotnoise[6],Noise *lasernoise[6]) {
    phlisa = mylisa->physlisa();
    lisa = mylisa;

    // set noise objects
    // the convention is {1,1*,2,2*,3,3*}, and {12,21,23,32,31,13}

    for(int craft = 1; craft <= 3; craft++) {
        pm[craft] = proofnoise[2*(craft-1)];
        pms[craft] = proofnoise[2*(craft-1)+1];
    }
        
    for(int craft1 = 1; craft1 <= 3; craft1++) {
        for(int craft2 = 1; craft2 <= 3; craft2++) {
            if(craft1 != craft2) {
				if( (craft1 == 1 && craft2 == 2) || (craft1 == 2 && craft2 == 3) || (craft1 == 3 && craft2 == 1) )
		    		shot[craft1][craft2] = shotnoise[2*(craft1-1)];
				else
		    		shot[craft1][craft2] = shotnoise[2*(craft2-1)+1];
	    	}
        }
    }

    for(int craft = 1; craft <= 3; craft++) {
        c[craft] = lasernoise[2*(craft-1)];
        cs[craft] = lasernoise[2*(craft-1)+1];
    }

    allocated = 0;
}

void TDInoise::setphlisa(LISA *mylisa) {
    phlisa = mylisa;
}


// --- zLockNoise ---

class zLockNoise : public Noise {
 private:
    int slave;

    Noise *masterpm, *slavepm, *masterc, *slavec;

 public:
    zLockNoise(int recv,Noise *mpm,Noise *spm,Noise *mc,Noise *sc)
	: slave(recv), masterpm(mpm), slavepm(spm), masterc(mc), slavec(sc) {};

    virtual ~zLockNoise() {
		delete slavec; // ??? Is this deallocation going to cause problems?
    };

    double value(double time);
    double value(double tb,double tc);
};

inline double zLockNoise::value(double t) {
	if (slave > 0) {
		// in this case master is starred, slave is not
		return masterc->value(t) - masterpm->value(t) - slavepm->value(t);
	} else {
		// in this case master is not starred, slave is
		return masterc->value(t) + masterpm->value(t) + slavepm->value(t);
	}
}

inline double zLockNoise::value(double tb,double tc) {
	if (slave > 0) {
		return masterc->value(tb,tc) - masterpm->value(tb,tc) - slavepm->value(tb,tc);
	} else {
		return masterc->value(tb,tc) + masterpm->value(tb,tc) + slavepm->value(tb,tc);
	}
}


// --- yLockNoise ---

class yLockNoise : public Noise {
 private:
    int slave, arm;

    LISA *lisa;

    Noise *slavepm, *shot, *masterc, *slavec;

 public:
    yLockNoise(int recv,int link,LISA *l,Noise *spm,Noise *sh,Noise *mc,Noise *sc)
	: slave(recv), arm(link), lisa(l), slavepm(spm), shot(sh), masterc(mc), slavec(sc) {};

    virtual ~yLockNoise() {
		delete slavec; // ??? Is this deallocation going to cause problems?
    };

    double value(double t);
    double value(double tb,double tc);

};

inline double yLockNoise::value(double t) {
	if (slave > 0) {
		// since the slave is not starred, the link is positive (cyclic)
		return masterc->value(t - lisa->armlength(arm,t)) - 2.0 * slavepm->value(t) + shot->value(t);
	} else {
		// the slave is starred, the link is anticyclic
		return masterc->value(t - lisa->armlength(arm,t)) + 2.0 * slavepm->value(t) + shot->value(t);
	}
}

inline double yLockNoise::value(double tb,double tc) {
	if (slave > 0) {
		return masterc->value(tb,tc - lisa->armlength(arm,tb+tc))
				- 2.0 * slavepm->value(tb,tc) + shot->value(tb,tc);
	} else {
		return masterc->noise(tb,tc - lisa->armlength(arm,tb+tc))
				+ 2.0 * slavepm->value(tb,tc) + shot->value(tb,tc);
	}
}    


// locking procedure: use negative numbers for starred lasers

void TDInoise::lock(int master) {
    int mastera = abs(master);
    int slaveb = (mastera % 3) + 1;
    int slavec = (slaveb % 3) + 1;

    // first lock the laser on the same bench

    if(master > 0) {
		cs[mastera] = new zLockNoise(-mastera,pm[ mastera],pms[mastera],c[ mastera],cs[mastera]);
    } else {
		c[ mastera] = new zLockNoise( mastera,pms[mastera],pm[ mastera],cs[mastera],c[ mastera]);
    }

    // now lock across to the other benches

    cs[slaveb] = new yLockNoise(-slaveb,-slavec,phlisa,pms[slaveb],shot[mastera][slaveb],c[ mastera],cs[slaveb]);
    c[ slavec] = new yLockNoise( slavec, slaveb,phlisa,pm[ slavec],shot[mastera][slavec],cs[mastera],c[ slavec]);

    // finally, lock the lasers on the back of the other benches

    c[ slaveb] = new zLockNoise( slaveb,pms[slaveb],pm[slaveb],cs[slaveb],c[slaveb]);
    cs[slavec] = new zLockNoise(-slavec,pm[slavec],pms[slavec],c[slavec],cs[slavec]);
}

TDInoise::~TDInoise() {
    if(allocated) {
		// allow for one noise object to be contained in multiple pointers
		// without calling delete twice

		// remove proof-mass-noise InterpolateNoise objects

		for(int craft = 1; craft <= 3; craft++) {
	    	if(pm[craft])  {delete pm[craft]; pm[craft]=0;}
	    	if(pms[craft]) {delete pms[craft]; pms[craft]=0;}
		}
 
		// remove optical-path-noise InterpolateNoise objects

		for(int craft1 = 1; craft1 <= 3; craft1++) {
	    	for(int craft2 = 1; craft2 <= 3; craft2++) {
				if(craft1 != craft2)
		    		if(shot[craft1][craft2]) {
		    			delete shot[craft1][craft2]; shot[craft1][craft2]=0;
		    		}
	    	}
		}
	
		// remove laser-noise InterpolateNoise objects

		for(int craft = 1; craft <= 3; craft++) {
	    	if(c[craft])  {delete c[craft]; c[craft]=0;}
	    	if(cs[craft]) {delete cs[craft]; cs[craft]=0;}
		}
    }
}

void TDInoise::reset(unsigned long seed) {
    WhiteNoiseSource::setglobalseed(seed);

    for(int craft = 1; craft <= 3; craft++) {
        pm[craft]->reset();
        pms[craft]->reset();
    }
 
    // reset optical-path-noise InterpolateNoise objects

    for(int craft1 = 1; craft1 <= 3; craft1++) {
        for(int craft2 = 1; craft2 <= 3; craft2++) {
            if(craft1 != craft2)
                shot[craft1][craft2]->reset();
        }
    }

    // reset laser-noise ExpGaussNoise objects

    for(int craft = 1; craft <= 3; craft++) {
        c[craft]->reset();
        cs[craft]->reset();
    }

    // reset also LISA, in case it includes noise of some kind
    
    lisa->reset();
    if(phlisa != lisa) phlisa->reset();
}

// this is a debugging function, which appears in lisasim-swig.i

double retardation(LISA *lisa,int ret1,int ret2,int ret3,int ret4,int ret5,int ret6,int ret7,int ret8,double t) {
    lisa->newretardtime(t);

    lisa->retard(ret8); lisa->retard(ret7); lisa->retard(ret6); lisa->retard(ret5);
    lisa->retard(ret4); lisa->retard(ret3); lisa->retard(ret2); lisa->retard(ret1);

    return lisa->retardedtime();
}

double TDInoise::y(int send, int slink, int recv, int ret1, int ret2, int ret3, double t) {
    return y(send,slink,recv,ret1,ret2,ret3,0,0,0,0,t);
}

double TDInoise::y(int send, int slink, int recv, int ret1, int ret2, int ret3, int ret4, int ret5, int ret6, int ret7, double t) {
    int link = abs(slink);

    // this recursive retardation procedure assumes smart TDI...

    lisa->newretardtime(t);

    lisa->retard(ret7); lisa->retard(ret6); lisa->retard(ret5);
    lisa->retard(ret4); lisa->retard(ret3); lisa->retard(ret2); lisa->retard(ret1);

    double retardedtime = lisa->retardedtime();

    try {
        if( (link == 3 && recv == 1) || (link == 2 && recv == 3) || (link == 1 && recv == 2)) {
            // cyclic combination
            // if introducing error in the determination of the armlengths, it should not enter
            // the following (physical) retardation of the laser noise, so we use the phlisa object
    
            lisa->retard(phlisa,link);
            double retardlaser = lisa->retardedtime();
        
            return( (*cs[send])[retardlaser] - 2.0 * (*pm[recv])[retardedtime]  - (*c[recv])[retardedtime]  + 
                    (*shot[send][recv])[retardedtime] );
        } else {
            // anticyclic combination
            // ditto here
    
            lisa->retard(phlisa,-link);
            double retardlaser = lisa->retardedtime();
    
            return( (*c[send])[retardlaser]  + 2.0 * (*pms[recv])[retardedtime] - (*cs[recv])[retardedtime] +
                    (*shot[send][recv])[retardedtime] );
        }
    } catch (ExceptionOutOfBounds &e) {
		std::cerr << "TDInoise::y(" << send << "," << slink << "," << recv
		          << "," << ret1 << "," << ret2 << "," << ret3 << "," << ret4
		          << "," << ret5 << "," << ret6 << "," << ret7
		          << ") : could not get noise (OutOfBounds) at time "
		          << t << " [" << __FILE__ << ":" << __LINE__ << "]." << std::endl;
		
		throw e;
	}
}

double TDInoise::z(int send, int slink, int recv, int ret1, int ret2, int ret3, int ret4, double t) {
    return z(send,slink,recv,ret1,ret2,ret3,ret4,0,0,0,0,t);
}

double TDInoise::z(int send, int slink, int recv, int ret1, int ret2, int ret3, int ret4, int ret5, int ret6, int ret7, int ret8, double t) {
    int link = abs(slink);

    // this recursive retardation procedure assumes smart TDI...
    // (and the correct order in the retardation expressions)

    lisa->newretardtime(t);

    lisa->retard(ret8); lisa->retard(ret7); lisa->retard(ret6); lisa->retard(ret5);
    lisa->retard(ret4); lisa->retard(ret3); lisa->retard(ret2); lisa->retard(ret1);

    double retardedtime = lisa->retardedtime();

    try {
        if( (link == 3 && recv == 1) || (link == 2 && recv == 3) || (link == 1 && recv == 2)) {
            // cyclic combination
    
            return( (*cs[recv])[retardedtime] - 2.0 * (*pms[recv])[retardedtime] - (*c[recv])[retardedtime] );
        } else {
            // anticyclic combination
    
            return( (*c[recv])[retardedtime]  + 2.0 * (*pm[recv])[retardedtime]  - (*cs[recv])[retardedtime] );
        }
    } catch (ExceptionOutOfBounds &e) {
		std::cerr << "TDInoise::z(" << send << "," << slink << "," << recv
		          << "," << ret1 << "," << ret2 << "," << ret3 << "," << ret4
		          << "," << ret5 << "," << ret6 << "," << ret7 << "," << ret8
		          << ") : could not get noise (OutOfBounds) at time "
		          << t << " [" << __FILE__ << ":" << __LINE__ << "]." << std::endl;
		
		throw e;
    }
}

// standard noises for TDI, with utility function

double lighttime(LISA *lisa) {
    // to estimate size of noisebuffer, take maximum armlength at time zero,
    // and add 10% for uplink-downlink uncertainty and flexing

    double arm1 = lisa->armlength(1,0.0);
    double arm2 = lisa->armlength(2,0.0);
    double arm3 = lisa->armlength(3,0.0);

    double maxarm = arm1 > arm2 ? arm1 : arm2;
    maxarm = maxarm > arm3 ? maxarm : arm3;

    return(1.10 * maxarm);
}


// --- Standard Noise factories ---

Noise *stdproofnoise(LISA *lisa,double stproof,double sdproof,int interp) {
    // create InterpolateNoise objects for proof-mass noises
    // we need quadruple retardations for the V's appearing in the z's
    // (octuple for 2nd-gen TDI)

    // add two sampling times to allow linear interpolation for large
    // sampling times
    
    double pbtproof = 8.0 * lighttime(lisa) + 2.0*stproof;

    return new PowerLawNoise(stproof,pbtproof,sdproof,-2.0,interp);
}


Noise *stdopticalnoise(LISA *lisa,double stshot,double sdshot,int interp) {
    // create InterpolateNoise objects for optical-path noises
    // we need only triple retardations for the shot's appearing in the y's
    // (septuple for 2nd-gen TDI)

    // add two sampling times to allow linear interpolation for large
    // sampling times
    
    double pbtshot = 7.0 * lighttime(lisa) + 2.0*stshot;
    
    return new PowerLawNoise(stshot,pbtshot,sdshot,2.0,interp);
}


Noise *stdlasernoise(LISA *lisa,double stlaser,double sdlaser,int interp) {
    // create laser noise objects

    double pbtlaser = 8.0 * lighttime(lisa) + 2.0*stlaser;

    return new PowerLawNoise(stlaser,pbtlaser,sdlaser,0.0,interp);
}


// --- Standard TDInoise factory ---

TDInoise *stdnoise(LISA *mylisa) {
    return new TDInoise(mylisa,1.0,2.5e-48,1.0,1.8e-37,1.0,1.1e-26);
}


double TDIaccurate::y(int send, int slink, int recv, int ret1, int ret2, int ret3, int ret4, int ret5, int ret6, int ret7, double t) {
    int link = abs(slink);

    // this recursive retardation procedure assumes smart TDI...

    lisa->newretardtime(t);

    lisa->retard(ret7); lisa->retard(ret6); lisa->retard(ret5);
    lisa->retard(ret4); lisa->retard(ret3); lisa->retard(ret2); lisa->retard(ret1);

    double retardation = -lisa->retardation();

    if( (link == 3 && recv == 1) || (link == 2 && recv == 3) || (link == 1 && recv == 2)) {
        // cyclic combination
        // if introducing error in the determination of the armlengths, it should not enter
        // the following (physical) retardation of the laser noise, so we use the phlisa object

		lisa->retard(phlisa,link);
		double retardlaser = -lisa->retardation();

        return( cs[send]->noise(t,retardlaser)
				- 2.0 * pm[recv]->noise(t,retardation) 
				- c[recv]->noise(t,retardation)
				+ shot[send][recv]->noise(t,retardation) );
    } else {
        // anticyclic combination
        // ditto here

		lisa->retard(phlisa,-link);
		double retardlaser = -lisa->retardation();

        return( c[send]->noise(t,retardlaser)
				+ 2.0 * pms[recv]->noise(t,retardation)
				- cs[recv]->noise(t,retardation)
				+ shot[send][recv]->noise(t,retardation) );
    }
}

double TDIaccurate::z(int send, int slink, int recv, int ret1, int ret2, int ret3, int ret4, int ret5, int ret6, int ret7, int ret8, double t) {
    int link = abs(slink);

    // this recursive retardation procedure assumes smart TDI...
    // (and the correct order in the retardation expressions)

    lisa->newretardtime(t);

    lisa->retard(ret8); lisa->retard(ret7); lisa->retard(ret6); lisa->retard(ret5);
    lisa->retard(ret4); lisa->retard(ret3); lisa->retard(ret2); lisa->retard(ret1);

    double retardation = -lisa->retardation();

    if( (link == 3 && recv == 1) || (link == 2 && recv == 3) || (link == 1 && recv == 2)) {
        // cyclic combination

        return( cs[recv]->noise(t,retardation)
		- 2.0 * pms[recv]->noise(t,retardation)
		- c[recv]->noise(t,retardation) );
    } else {
        // anticyclic combination

        return( c[recv]->noise(t,retardation)
		+ 2.0 * pm[recv]->noise(t,retardation)
		- cs[recv]->noise(t,retardation) );
    }
}

// --------------------------------------------------------------------------------------------
// Added EJB -->

double TDIpointing::y(int send, int slink, int recv, int ret1, int ret2,
		      int ret3, int ret4, int ret5,
		      int ret6, int ret7, double t)
{
  int link = abs(slink);
  
  // this recursive retardation procedure assumes smart TDI...
  
  lisa->newretardtime(t);
  
  lisa->retard(ret7); lisa->retard(ret6); lisa->retard(ret5);
  lisa->retard(ret4); lisa->retard(ret3); lisa->retard(ret2); lisa->retard(ret1);
  
  double retardation = -lisa->retardation();
  
  double pointing_noise = 0;
  Vector u,v;
  // Get the angle between the arms, but at what time?
  lisa->putn(v, link, t); // sure it is time and not sample numnber?
  lisa->putn(u, recv, t);
  double angle = dotproduct(u, v);
  int interp = 1.0;
  pointing_noise = breathinganglenoise(angle, t, interp);

  if ( (link == 3 && recv == 1) || (link == 2 && recv == 3) || (link == 1 && recv == 2))
    {
      // cyclic combination
      // if introducing error in the determination of the armlengths, it should not enter
      // the following (physical) retardation of the laser noise, so we use the phlisa object
      
      lisa->retard(phlisa,link);
      double retardlaser = -lisa->retardation();

      return( cs[send]->noise(t,retardlaser)
	      - 2.0 * pm[recv]->noise(t,retardation) 
	      - c[recv]->noise(t,retardation)
	      + shot[send][recv]->noise(t,retardation)
	      + pointing_noise);
    }
  else
    {
      // anticyclic combination
      // ditto here
      
      lisa->retard(phlisa,-link);
      double retardlaser = -lisa->retardation();


      return( c[send]->noise(t,retardlaser)
	      + 2.0 * pms[recv]->noise(t,retardation)
	      - cs[recv]->noise(t,retardation)
	      + shot[send][recv]->noise(t,retardation)
	      + pointing_noise);
    }
}

double TDIpointing::z(int send, int slink, int recv, int ret1, int ret2,
		      int ret3, int ret4, int ret5, int ret6,
		      int ret7, int ret8, double t)
{
  int link = abs(slink);
  
  // this recursive retardation procedure assumes smart TDI...
  // (and the correct order in the retardation expressions)
  
  lisa->newretardtime(t);
  
  lisa->retard(ret8); lisa->retard(ret7); lisa->retard(ret6); lisa->retard(ret5);
  lisa->retard(ret4); lisa->retard(ret3); lisa->retard(ret2); lisa->retard(ret1);
  
  double retardation = -lisa->retardation();

  double pointing_noise = 0;
  Vector u,v;
  // Get the angle between the arms, but at what time?
  lisa->putn(v, link, t); // sure it is time and not sample numnber?
  lisa->putn(u, recv, t);
  double angle = dotproduct(u, v);
  int interp = 1.0;
  pointing_noise = breathinganglenoise(angle, t, interp);

  
  if( (link == 3 && recv == 1) || (link == 2 && recv == 3) || (link == 1 && recv == 2))
    {
      // cyclic combination
      
      return( cs[recv]->noise(t,retardation)
	      - 2.0 * pms[recv]->noise(t,retardation)
	      - c[recv]->noise(t,retardation)  + pointing_noise);
    }
  else
    {
      // anticyclic combination
      return( c[recv]->noise(t,retardation)
	      + 2.0 * pm[recv]->noise(t,retardation)
	      - cs[recv]->noise(t,retardation)  + pointing_noise);
    }
}

// <-- Added EJB


// --- TDIdoppler ---
/* TDIdoppler::TDIdoppler(LISA *mylisa, Noise *proofnoise[6],Noise *shotnoise[6],Noise *lasernoise[6],double laserfreqs[6])
    : TDInoise(mylisa,proofnoise,shotnoise,lasernoise) {
    lf[1] = laserfreqs[0];
    lfs[1] = laserfreqs[1];
    
    lf[2] = laserfreqs[2];
    lfs[2] = laserfreqs[3];
    
    lf[3] = laserfreqs[4];
    lfs[3] = laserfreqs[5];
}; */

double TDIdoppler::y(int send, int slink, int recv, int ret1, int ret2, int ret3, int ret4, int ret5, int ret6, int ret7, double t) {
    int link = abs(slink);

    // this recursive retardation procedure assumes smart TDI...

    lisa->newretardtime(t);
    double dopplerfactor = 1.0;

    if(ret7 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret7,t));
        lisa->retard(ret7);
    }

    if(ret6 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret6,lisa->retardedtime()));
        lisa->retard(ret6);
    }

    if(ret5 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret5,lisa->retardedtime()));
        lisa->retard(ret5);
    }

    if(ret4 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret4,lisa->retardedtime()));
        lisa->retard(ret4);
    }

    if(ret3 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret3,lisa->retardedtime()));
        lisa->retard(ret3);
    }

    if(ret2 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret2,lisa->retardedtime()));
        lisa->retard(ret2);
    }

    if(ret1 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret1,lisa->retardedtime()));
        lisa->retard(ret1);
    }

    double retardation = -lisa->retardation();

    if( (link == 3 && recv == 1) || (link == 2 && recv == 3) || (link == 1 && recv == 2)) {
        lisa->retard(phlisa,link);
        double retardlaser = -lisa->retardation();

        double doppler = 1.0 - phlisa->dotarmlength(link,t + retardation);

        return dopplerfactor * ( doppler * cs[send]->noise(t,retardlaser)
                                 - doppler * 2.0 * pm[recv]->noise(t,retardation) 
                                 - c[recv]->noise(t,retardation)
                                 + shot[send][recv]->noise(t,retardation) );
    } else {
        lisa->retard(phlisa,-link);
        double retardlaser = -lisa->retardation();

        double doppler = 1.0 - phlisa->dotarmlength(-link,t + retardation);
        
        return dopplerfactor * ( doppler * c[send]->noise(t,retardlaser)
                                 + doppler * 2.0 * pms[recv]->noise(t,retardation)
                                 - cs[recv]->noise(t,retardation)
                                 + shot[send][recv]->noise(t,retardation) );
    }
}

double TDIdoppler::z(int send, int slink, int recv, int ret1, int ret2, int ret3, int ret4, int ret5, int ret6, int ret7, int ret8, double t) {
    int link = abs(slink);

    // this recursive retardation procedure assumes smart TDI...
    // (and the correct order in the retardation expressions)

    lisa->newretardtime(t);
    double dopplerfactor = 1.0;

    if(ret8 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret8,t));
        lisa->retard(ret8);
    }

    if(ret7 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret7,lisa->retardedtime()));
        lisa->retard(ret7);
    }

    if(ret6 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret6,lisa->retardedtime()));
        lisa->retard(ret6);
    }

    if(ret5 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret5,lisa->retardedtime()));
        lisa->retard(ret5);
    }

    if(ret4 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret4,lisa->retardedtime()));
        lisa->retard(ret4);
    }

    if(ret3 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret3,lisa->retardedtime()));
        lisa->retard(ret3);
    }

    if(ret2 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret2,lisa->retardedtime()));
        lisa->retard(ret2);
    }

    if(ret1 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret1,lisa->retardedtime()));
        lisa->retard(ret1);
    }
    
    double retardation = -lisa->retardation();

    if( (link == 3 && recv == 1) || (link == 2 && recv == 3) || (link == 1 && recv == 2)) {
        // cyclic combination

        return dopplerfactor * ( cs[recv]->noise(t,retardation)
                                 - 2.0 * pms[recv]->noise(t,retardation)
                                 - c[recv]->noise(t,retardation) );
    } else {
        // anticyclic combination

        return dopplerfactor * ( c[recv]->noise(t,retardation)
                                 + 2.0 * pm[recv]->noise(t,retardation)
                                 - cs[recv]->noise(t,retardation) );
    }
}

// --- TDIcarrier ---

TDIcarrier::TDIcarrier(LISA *mylisa,double *laserfreqs)
    : TDInoise(mylisa,1.0,0.0,1.0,0.0,1.0,0.0) {
    lf[1]  = laserfreqs[0];
    lfs[1] = laserfreqs[1];

    lf[2]  = laserfreqs[2];
    lfs[2] = laserfreqs[3];
    
    lf[3]  = laserfreqs[4];
    lfs[3] = laserfreqs[5];    
};

double TDIcarrier::y(int send, int slink, int recv, int ret1, int ret2, int ret3, int ret4, int ret5, int ret6, int ret7, double t) {
    int link = abs(slink);

    // this recursive retardation procedure assumes smart TDI...

    lisa->newretardtime(t);
    double dopplerfactor = 1.0;

    if(ret7 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret7,t));
        lisa->retard(ret7);
    }

    if(ret6 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret6,lisa->retardedtime()));
        lisa->retard(ret6);
    }

    if(ret5 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret5,lisa->retardedtime()));
        lisa->retard(ret5);
    }

    if(ret4 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret4,lisa->retardedtime()));
        lisa->retard(ret4);
    }

    if(ret3 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret3,lisa->retardedtime()));
        lisa->retard(ret3);
    }

    if(ret2 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret2,lisa->retardedtime()));
        lisa->retard(ret2);
    }

    if(ret1 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret1,lisa->retardedtime()));
        lisa->retard(ret1);
    }

    if( (link == 3 && recv == 1) || (link == 2 && recv == 3) || (link == 1 && recv == 2)) {
        double mdoppler = phlisa->dotarmlength(link,lisa->retardedtime());

        return dopplerfactor * ( (lfs[send] - lf[recv]) - mdoppler * lfs[send] );
    } else {
        double mdoppler = phlisa->dotarmlength(-link,lisa->retardedtime());
        
        return dopplerfactor * ( (lf[send] - lfs[recv]) - mdoppler * lf[send] );
    }
}

double TDIcarrier::z(int send, int slink, int recv, int ret1, int ret2, int ret3, int ret4, int ret5, int ret6, int ret7, int ret8, double t) {
    int link = abs(slink);

    // this recursive retardation procedure assumes smart TDI...
    // (and the correct order in the retardation expressions)

    lisa->newretardtime(t);
    double dopplerfactor = 1.0;

    if(ret8 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret8,t));
        lisa->retard(ret8);
    }

    if(ret7 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret7,lisa->retardedtime()));
        lisa->retard(ret7);
    }

    if(ret6 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret6,lisa->retardedtime()));
        lisa->retard(ret6);
    }

    if(ret5 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret5,lisa->retardedtime()));
        lisa->retard(ret5);
    }

    if(ret4 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret4,lisa->retardedtime()));
        lisa->retard(ret4);
    }

    if(ret3 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret3,lisa->retardedtime()));
        lisa->retard(ret3);
    }

    if(ret2 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret2,lisa->retardedtime()));
        lisa->retard(ret2);
    }

    if(ret1 != 0) {
        dopplerfactor *= (1 - lisa->dotarmlength(ret1,lisa->retardedtime()));
        lisa->retard(ret1);
    }
    
    if( (link == 3 && recv == 1) || (link == 2 && recv == 3) || (link == 1 && recv == 2)) {
        // cyclic combination

        return dopplerfactor * ( lfs[recv] - lf[recv] );
    } else {
        // anticyclic combination

        return dopplerfactor * ( lf[recv] - lfs[recv] );
    }
}

double breathinganglenoise(double angle, double t, int interp)
{
  
  double sdpointing = pow(fabs(angle),10)*1e-40;
  // EJB this takes too long! fix it!    
  Noise* breathingnoise = new PowerLawNoise(1.0, 128.0, sdpointing, 0.0, interp);

  double aid  = breathingnoise -> noise(t);
  delete(breathingnoise);
  
  return aid;

}
