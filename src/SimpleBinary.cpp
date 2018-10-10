
#include "SimpleBinary.h"

#include <iostream>
#include <math.h>


// full constructor for SimpleBinary; takes frequency in Hertz
// b and l are SSB ecliptic latitude and longitude

SimpleBinary::SimpleBinary(double freq, double initphi, double inc, double amp, double b, double l, double p) : Wave(b,l,p) {
    f = freq;

    phi0 = initphi;

    i = inc;
    a = amp;

    ap = a * (1.0 + cos(i)*cos(i));
    ac = a * (2.0 * cos(i));
}

double SimpleBinary::hp(double t) {
    const double twopi = 2.0*M_PI;

    return ap * cos(twopi*f*t + phi0);
}

double SimpleBinary::hc(double t) {
    const double twopi = 2.0*M_PI;

    return ac * sin(twopi*f*t + phi0);
}
