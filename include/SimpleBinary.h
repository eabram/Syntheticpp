
#ifndef _SIMPLEBINARY_H_
#define _SIMPLEBINARY_H_

#include "lisasim-wave.h"
#include "lisasim-signal.h"

// --- SimpleBinary ---

class SimpleBinary : public Wave {
 private:
	// frequency, initial phase

	double f, phi0;

	// inclination, polarization amplitudes

	double i, a, ap, ac;

 public:
	SimpleBinary(double freq, double initphi, double inc, double amp, double b, double l, double p);

	double hp(double t);
	double hc(double t);
};

#endif
