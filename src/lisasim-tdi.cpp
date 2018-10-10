/* $Id$
 * $Date$
 * $Author$
 * $Revision$
 */

#include "lisasim-tdi.h"

//#include <Python.h>

#include <time.h>
#include <stdio.h>

// in these expressions the order of the delays is physically
// motivated, but the combination still does not cancel laser
// noise (from the final ref. lasers) in ModifiedLISA

double TDI::alpham(double t) {
    return( y(3,-2, 1, 0, 0, 0, t) -
            y(2, 3, 1, 0, 0, 0, t) +
            y(2,-1, 3,-2, 0, 0, t) -
            y(3, 1, 2, 3, 0, 0, t) +
            y(1,-3, 2,-1,-2, 0, t) -
            y(1, 2, 3, 1, 3, 0, t) +
    0.5 * ( z(2, 3, 1,-3,-1,-2, 0, t) -
            z(3,-2, 1, 2, 1, 3, 0, t) +
            z(3, 1, 2,-1,-2, 0, 0, t) -
            z(2,-1, 3, 1, 3, 0, 0, t) +
            z(1, 2, 3, 1, 3, 0, 0, t) -
            z(1,-3, 2,-1,-2, 0, 0, t) +
            z(3, 1, 2, 3, 0, 0, 0, t) -
            z(2,-1, 3,-2, 0, 0, 0, t) +
            z(1, 2, 3,-2, 0, 0, 0, t) -
            z(1,-3, 2, 3, 0, 0, 0, t) +
            z(2, 3, 1, 0, 0, 0, 0, t) -
            z(3,-2, 1, 0, 0, 0, 0, t) ) );
}

double TDI::betam(double t) {
    return( y(1,-3, 2, 0, 0, 0, t) -
            y(3, 1, 2, 0, 0, 0, t) +
            y(3,-2, 1,-3, 0, 0, t) -
            y(1, 2, 3, 1, 0, 0, t) +
            y(2,-1, 3,-2,-3, 0, t) -
            y(2, 3, 1, 2, 1, 0, t) +
    0.5 * ( z(3, 1, 2,-1,-2,-3, 0, t) -
            z(1,-3, 2, 3, 2, 1, 0, t) +
            z(1, 2, 3,-2,-3, 0, 0, t) -
            z(3,-2, 1, 2, 1, 0, 0, t) +
            z(2, 3, 1, 2, 1, 0, 0, t) -
            z(2,-1, 3,-2,-3, 0, 0, t) +
            z(1, 2, 3, 1, 0, 0, 0, t) -
            z(3,-2, 1,-3, 0, 0, 0, t) +
            z(2, 3, 1,-3, 0, 0, 0, t) -
            z(2,-1, 3, 1, 0, 0, 0, t) +
            z(3, 1, 2, 0, 0, 0, 0, t) -
            z(1,-3, 2, 0, 0, 0, 0, t) ) );
}

double TDI::gammam(double t) {
    return( y(2,-1, 3, 0, 0, 0, t) -
            y(1, 2, 3, 0, 0, 0, t) +
            y(1,-3, 2,-1, 0, 0, t) -
            y(2, 3, 1, 2, 0, 0, t) +
            y(3,-2, 1,-3,-1, 0, t) -
            y(3, 1, 2, 3, 2, 0, t) +
    0.5 * ( z(1, 2, 3,-2,-3,-1, 0, t) -
            z(2,-1, 3, 1, 3, 2, 0, t) +
            z(2, 3, 1,-3,-1, 0, 0, t) -
            z(1,-3, 2, 3, 2, 0, 0, t) +
            z(3, 1, 2, 3, 2, 0, 0, t) -
            z(3,-2, 1,-3,-1, 0, 0, t) +
            z(2, 3, 1, 2, 0, 0, 0, t) -
            z(1,-3, 2,-1, 0, 0, 0, t) +
            z(3, 1, 2,-1, 0, 0, 0, t) -
            z(3,-2, 1, 2, 0, 0, 0, t) +
            z(1, 2, 3, 0, 0, 0, 0, t) -
            z(2,-1, 3, 0, 0, 0, 0, t) ) );
}

// how to set the sign of the retardations here?

double TDI::zetam(double t) {
    return( y(1,-3, 2, 2, 0, 0, t) -
            y(1, 2, 3, 3, 0, 0, t) +
            y(2,-1, 3, 3, 0, 0, t) -
            y(2, 3, 1, 1, 0, 0, t) +
            y(3,-2, 1, 1, 0, 0, t) -
            y(3, 1, 2, 2, 0, 0, t) +
    0.5 * (-z(2,-1, 3, 2, 1, 0, 0, t) +
            z(1, 2, 3, 1, 2, 0, 0, t) -
            z(3,-2, 1, 2, 3, 0, 0, t) +
            z(2, 3, 1, 2, 3, 0, 0, t) -
            z(1,-3, 2, 1, 3, 0, 0, t) +
            z(3, 1, 2, 1, 3, 0, 0, t) ) +
    0.5 * (-z(1,-3, 2, 2, 0, 0, 0, t) +
            z(3, 1, 2, 2, 0, 0, 0, t) -
            z(2,-1, 3, 3, 0, 0, 0, t) +
            z(1, 2, 3, 3, 0, 0, 0, t) -
            z(3,-2, 1, 1, 0, 0, 0, t) +
            z(2, 3, 1, 1, 0, 0, 0, t) ) );
}

double TDI::alpha1(double t) {
    return( y(1,-3, 2,-1,-2, 2, 1, 3, 0, 0, t) -
            y(1, 2, 3, 1, 3,-3,-1,-2, 0, 0, t) +
            y(2,-1, 3,-2, 2, 1, 3, 0, 0, 0, t) -
            y(3, 1, 2, 3,-3,-1,-2, 0, 0, 0, t) +
            y(3,-2, 1, 2, 1, 3, t) -
            y(2, 3, 1,-3,-1,-2, t) + 
            y(1, 2, 3, 1, 3, 0, t) - 
            y(1,-3, 2,-1,-2, 0, t) +
            y(3, 1, 2, 3, 0, 0, t) -
            y(2,-1, 3,-2, 0, 0, t) +
            y(2, 3, 1, 0, 0, 0, t) -
            y(3,-2, 1, 0, 0, 0, t) +
    0.5 * ( z(2, 3, 1,-3,-1,-2, 2, 1, 3, 0, 0, t) -
            z(3,-2, 1, 2, 1, 3,-3,-1,-2, 0, 0, t) +
            z(1, 2, 3, 1, 3,-3,-1,-2, 0, 0, 0, t) -
            z(1,-3, 2,-1,-2, 2, 1, 3, 0, 0, 0, t) +
            z(3, 1, 2,-1,-2, 2, 1, 3, 0, 0, 0, t) -
            z(2,-1, 3, 1, 3,-3,-1,-2, 0, 0, 0, t) +
            z(1, 2, 3,-2, 2, 1, 3, t) -
            z(2,-1, 3,-2, 2, 1, 3, t) +
            z(3, 1, 2, 3,-3,-1,-2, t) -
            z(1,-3, 2, 3,-3,-1,-2, t) +
            z(1,-3, 2,-1,-2, 0, 0, t) -
            z(3, 1, 2,-1,-2, 0, 0, t) +
            z(2,-1, 3, 1, 3, 0, 0, t) -
            z(1, 2, 3, 1, 3, 0, 0, t) +
            z(1,-3, 2, 3, 0, 0, 0, t) -
            z(1, 2, 3,-2, 0, 0, 0, t) +
            z(2,-1, 3,-2, 0, 0, 0, t) -
            z(3, 1, 2, 3, 0, 0, 0, t) +
            z(3,-2, 1, 0, 0, 0, 0, t) -
            z(2, 3, 1, 0, 0, 0, 0, t) ) );
}

double TDI::alpha2(double t) {
    return( y(2,-1, 3,-2,-3, 3, 2, 1, 0, 0, t) -
            y(2, 3, 1, 2, 1,-1,-2,-3, 0, 0, t) +
            y(3,-2, 1,-3, 3, 2, 1, 0, 0, 0, t) -
            y(1, 2, 3, 1,-1,-2,-3, 0, 0, 0, t) +
            y(1,-3, 2, 3, 2, 1, t) -
            y(3, 1, 2,-1,-2,-3, t) +
            y(2, 3, 1, 2, 1, 0, t) -
            y(2,-1, 3,-2,-3, 0, t) +
            y(1, 2, 3, 1, 0, 0, t) -
            y(3,-2, 1,-3, 0, 0, t) +
            y(3, 1, 2, 0, 0, 0, t) -
            y(1,-3, 2, 0, 0, 0, t) +
    0.5 * ( z(3, 1, 2,-1,-2,-3, 3, 2, 1, 0, 0, t) -
            z(1,-3, 2, 3, 2, 1,-1,-2,-3, 0, 0, t) +
            z(2, 3, 1, 2, 1,-1,-2,-3, 0, 0, 0, t) -
            z(2,-1, 3,-2,-3, 3, 2, 1, 0, 0, 0, t) +
            z(1, 2, 3,-2,-3, 3, 2, 1, 0, 0, 0, t) -
            z(3,-2, 1, 2, 1,-1,-2,-3, 0, 0, 0, t) +
            z(2, 3, 1,-3, 3, 2, 1, t) -
            z(3,-2, 1,-3, 3, 2, 1, t) +
            z(1, 2, 3, 1,-1,-2,-3, t) -
            z(2,-1, 3, 1,-1,-2,-3, t) +
            z(2,-1, 3,-2,-3, 0, 0, t) -
            z(1, 2, 3,-2,-3, 0, 0, t) +
            z(3,-2, 1, 2, 1, 0, 0, t) -
            z(2, 3, 1, 2, 1, 0, 0, t) +
            z(2,-1, 3, 1, 0, 0, 0, t) -
            z(2, 3, 1,-3, 0, 0, 0, t) +
            z(3,-2, 1,-3, 0, 0, 0, t) -
            z(1, 2, 3, 1, 0, 0, 0, t) +
            z(1,-3, 2, 0, 0, 0, 0, t) -
            z(3, 1, 2, 0, 0, 0, 0, t) ) );
}

double TDI::alpha3(double t) {
    return( y(3,-2, 1,-3,-1, 1, 3, 2, 0, 0, t) -
            y(3, 1, 2, 3, 2,-2,-3,-1, 0, 0, t) + 
            y(1,-3, 2,-1, 1, 3, 2, 0, 0, 0, t) -
            y(2, 3, 1, 2,-2,-3,-1, 0, 0, 0, t) +
            y(2,-1, 3, 1, 3, 2, t) -
            y(1, 2, 3,-2,-3,-1, t) +
            y(3, 1, 2, 3, 2, 0, t) -
            y(3,-2, 1,-3,-1, 0, t) +
            y(2, 3, 1, 2, 0, 0, t) -
            y(1,-3, 2,-1, 0, 0, t) +
            y(1, 2, 3, 0, 0, 0, t) -
            y(2,-1, 3, 0, 0, 0, t) +
    0.5 * ( z(1, 2, 3,-2,-3,-1, 1, 3, 2, 0, 0, t) -
            z(2,-1, 3, 1, 3, 2,-2,-3,-1, 0, 0, t) +
            z(3, 1, 2, 3, 2,-2,-3,-1, 0, 0, 0, t) -
            z(3,-2, 1,-3,-1, 1, 3, 2, 0, 0, 0, t) +
            z(2, 3, 1,-3,-1, 1, 3, 2, 0, 0, 0, t) -
            z(1,-3, 2, 3, 2,-2,-3,-1, 0, 0, 0, t) +
            z(3, 1, 2,-1, 1, 3, 2, t) -
            z(1,-3, 2,-1, 1, 3, 2, t) +
            z(2, 3, 1, 2,-2,-3,-1, t) -
            z(3,-2, 1, 2,-2,-3,-1, t) +
            z(3,-2, 1,-3,-1, 0, 0, t) -
            z(2, 3, 1,-3,-1, 0, 0, t) +
            z(1,-3, 2, 3, 2, 0, 0, t) -
            z(3, 1, 2, 3, 2, 0, 0, t) +
            z(3,-2, 1, 2, 0, 0, 0, t) -
            z(3, 1, 2,-1, 0, 0, 0, t) +
            z(1,-3, 2,-1, 0, 0, 0, t) -
            z(2, 3, 1, 2, 0, 0, 0, t) +
            z(2,-1, 3, 0, 0, 0, 0, t) -
            z(1, 2, 3, 0, 0, 0, 0, t) ) );
}

double TDI::zeta1(double t) {
    return( ( y(3,-2, 1, 1, 2, 3, t) -
              y(3, 1, 2,-2, 2, 3, t) +
              y(1,-3, 2,-2, 2, 3, t) ) -
            ( y(1, 2, 3, 3,-3,-2, t) -
              y(2,-1, 3, 3,-3,-2, t) +
              y(2, 3, 1,-1,-3,-2, t) ) -
            ( y(3,-2, 1, 1,-1, 0, t) -
              y(3, 1, 2,-2,-1, 0, t) +
              y(1,-3, 2,-2,-1, 0, t) ) +
            ( y(1, 2, 3, 3, 1, 0, t) -
              y(2,-1, 3, 3, 1, 0, t) +
              y(2, 3, 1,-1, 1, 0, t) ) +
      0.5*( ( z(3, 1, 2,-2, 2, 3, 0, t) -
              z(1,-3, 2,-2, 2, 3, 0, t)   ) -
            ( z(3, 1, 2,-2,-1, 0, 0, t) -
              z(1,-3, 2,-2,-1, 0, 0, t)   ) +
            ( z(3, 1, 2,-1, 3,-3,-2, t) -
              z(1,-3, 2,-1, 3,-3,-2, t)   ) -
            ( z(3, 1, 2,-1, 3, 1, 0, t) -
              z(1,-3, 2,-1, 3, 1, 0, t)   ) +
            ( z(1, 2, 3,-2, 2, 3, 1, t) -
              z(2,-1, 3,-2, 2, 3, 1, t)   ) -
            ( z(1, 2, 3,-2,-1, 1, 0, t) -
              z(2,-1, 3,-2,-1, 1, 0, t)   ) +
            ( z(1, 2, 3,-2,-3, 3, 0, t) -
              z(2,-1, 3,-2,-3, 3, 0, t)   ) -
            ( z(1, 2, 3, 3, 1, 0, 0, t) -
              z(2,-1, 3, 3, 1, 0, 0, t)   ) +
            ( z(2, 3, 1,-2, 2,-3, 3, t) -
              z(3,-2, 1,-2, 2,-3, 3, t)   ) -
            ( z(2, 3, 1,-1, 1, 0, 0, t) -
              z(3,-2, 1,-1, 1, 0, 0, t)   ) ) );
}

double TDI::zeta2(double t) {
    return( ( y(1,-3, 2, 2, 3, 1, t) -
              y(1, 2, 3,-3, 3, 1, t) +
              y(2,-1, 3,-3, 3, 1, t) ) -
            ( y(2, 3, 1, 1,-1,-3, t) -
              y(3,-2, 1, 1,-1,-3, t) +
              y(3, 1, 2,-2,-1,-3, t) ) -
            ( y(1,-3, 2, 2,-2, 0, t) -
              y(1, 2, 3,-3,-2, 0, t) +
              y(2,-1, 3,-3,-2, 0, t) ) +
            ( y(2, 3, 1, 1, 2, 0, t) -
              y(3,-2, 1, 1, 2, 0, t) +
              y(3, 1, 2,-2, 2, 0, t) ) +
      0.5*( ( z(1, 2, 3,-3, 3, 1, 0, t) -
              z(2,-1, 3,-3, 3, 1, 0, t)   ) -
            ( z(1, 2, 3,-3,-2, 0, 0, t) -
              z(2,-1, 3,-3,-2, 0, 0, t)   ) +
            ( z(1, 2, 3,-2, 1,-1,-3, t) -
              z(2,-1, 3,-2, 1,-1,-3, t)   ) -
            ( z(1, 2, 3,-2, 1, 2, 0, t) -
              z(2,-1, 3,-2, 1, 2, 0, t)   ) +
            ( z(2, 3, 1,-3, 3, 1, 2, t) -
              z(3,-2, 1,-3, 3, 1, 2, t)   ) -
            ( z(2, 3, 1,-3,-2, 2, 0, t) -
              z(3,-2, 1,-3,-2, 2, 0, t)   ) +
            ( z(2, 3, 1,-3,-1, 1, 0, t) -
              z(3,-2, 1,-3,-1, 1, 0, t)   ) -
            ( z(2, 3, 1, 1, 2, 0, 0, t) -
              z(3,-2, 1, 1, 2, 0, 0, t)   ) +
            ( z(3, 1, 2,-3, 3,-1, 1, t) -
              z(1,-3, 2,-3, 3,-1, 1, t)   ) -
            ( z(3, 1, 2,-2, 2, 0, 0, t) -
              z(1,-3, 2,-2, 2, 0, 0, t)   ) ) );
}

double TDI::zeta3(double t) {
    return( ( y(2,-1, 3, 3, 1, 2, t) -
              y(2, 3, 1,-1, 1, 2, t) +
              y(3,-2, 1,-1, 1, 2, t) ) -
            ( y(3, 1, 2, 2,-2,-1, t) -
              y(1,-3, 2, 2,-2,-1, t) +
              y(1, 2, 3,-3,-2,-1, t) ) -
            ( y(2,-1, 3, 3,-3, 0, t) -
              y(2, 3, 1,-1,-3, 0, t) +
              y(3,-2, 1,-1,-3, 0, t) ) +
            ( y(3, 1, 2, 2, 3, 0, t) -
              y(1,-3, 2, 2, 3, 0, t) +
              y(1, 2, 3,-3, 3, 0, t) ) +
      0.5*( ( z(2, 3, 1,-1, 1, 2, 0, t) -
              z(3,-2, 1,-1, 1, 2, 0, t)   ) -
            ( z(2, 3, 1,-1,-3, 0, 0, t) -
              z(3,-2, 1,-1,-3, 0, 0, t)   ) +
            ( z(2, 3, 1,-3, 2,-2,-1, t) -
              z(3,-2, 1,-3, 2,-2,-1, t)   ) -
            ( z(2, 3, 1,-3, 2, 3, 0, t) -
              z(3,-2, 1,-3, 2, 3, 0, t)   ) +
            ( z(3, 1, 2,-1, 1, 2, 3, t) -
              z(1,-3, 2,-1, 1, 2, 3, t)   ) -
            ( z(3, 1, 2,-1,-3, 3, 0, t) -
              z(1,-3, 2,-1,-3, 3, 0, t)   ) +
            ( z(3, 1, 2,-1,-2, 2, 0, t) -
              z(1,-3, 2,-1,-2, 2, 0, t)   ) -
            ( z(3, 1, 2, 2, 3, 0, 0, t) -
              z(1,-3, 2, 2, 3, 0, 0, t)   ) +
            ( z(1, 2, 3,-1, 1,-2, 2, t) -
              z(2,-1, 3,-1, 1,-2, 2, t)   ) -
            ( z(1, 2, 3,-3, 3, 0, 0, t) -
              z(2,-1, 3,-3, 3, 0, 0, t)   ) ) );
}

// to be updated with physical-delay expressions
// and with multiple arm expressions

double TDI::P(double t) {
    return( y(1, 3, 2, 2, 0, 0, t) -
            y(1, 2, 3, 3, 0, 0, t) - 
            y(3, 1, 2, 2, 0, 0, t) +
            y(2, 1, 3, 3, 0, 0, t) +
            y(3, 1, 2, 1, 3, 0, t) -
            y(2, 1, 3, 1, 2, 0, t) + 
            y(1, 2, 3, 3, 1, 1, t) -
            y(1, 3, 2, 2, 1, 1, t) +
    0.5 * (-z(3, 2, 1, 2, 3, 0, 0, t) +
            z(3, 2, 1, 1, 1, 2, 3, t) + 
            z(2, 3, 1, 2, 3, 0, 0, t) -
            z(2, 3, 1, 1, 1, 2, 3, t) ) +
    0.5 * (-z(1, 3, 2, 2, 0, 0, 0, t) + 
            z(1, 3, 2, 1, 1, 2, 0, t) +
            z(3, 1, 2, 2, 0, 0, 0, t) -
            z(3, 1, 2, 1, 1, 2, 0, t) ) +
    0.5 * (-z(2, 1, 3, 3, 0, 0, 0, t) +
            z(2, 1, 3, 1, 1, 3, 0, t) +
            z(1, 2, 3, 3, 0, 0, 0, t) -
            z(1, 2, 3, 1, 1, 3, 0, t) ) );
}

double TDI::E(double t) {
    return( y(3, 1, 2, 2, 1, 0, t) -
            y(2, 1, 3, 3, 1, 0, t) -
            y(3, 1, 2, 3, 0, 0, t) +
            y(2, 1, 3, 2, 0, 0, t) +
            y(2, 3, 1, 1, 1, 0, t) -
            y(3, 2, 1, 1, 1, 0, t) -
            y(2, 3, 1, 0, 0, 0, t) +
            y(3, 2, 1, 0, 0, 0, t) -
    0.5 * ( z(2, 1, 3, 2, 0, 0, 0, t) +
            z(3, 2, 1, 0, 0, 0, 0, t) +
            z(1, 3, 2, 3, 0, 0, 0, t) -
            z(2, 1, 3, 1, 1, 2, 0, t) +
            z(1, 2, 3, 1, 1, 2, 0, t) -
            z(1, 3, 2, 1, 1, 3, 0, t) ) +
    0.5 * ( z(1, 2, 3, 2, 0, 0, 0, t) +
            z(2, 3, 1, 0, 0, 0, 0, t) +
            z(3, 1, 2, 3, 0, 0, 0, t) -
            z(3, 1, 2, 1, 1, 3, 0, t) +
            z(3, 2, 1, 1, 1, 0, 0, t) -
            z(2, 3, 1, 1, 1, 0, 0, t) ) );
};

double TDI::U(double t) {
    return( y(3, 2, 1, 1, 1, 3, t) -
            y(3, 2, 1, 3, 0, 0, t) -
            y(3, 1, 2, 1, 2, 3, t) +
            y(2, 1, 3, 1, 0, 0, t) -
            y(2, 1, 3, 2, 3, 0, t) +
            y(1, 3, 2, 1, 1, 0, t) -
            y(1, 3, 2, 0, 0, 0, t) +
            y(3, 1, 2, 0, 0, 0, t) -
    0.5 * ( z(2, 3, 1, 3, 0, 0, 0, t) +
            z(3, 1, 2, 0, 0, 0, 0, t) +
            z(1, 2, 3, 2, 3, 0, 0, t) +
            z(1, 3, 2, 1, 1, 0, 0, t) +
            z(2, 1, 3, 1, 1, 2, 3, t) +
            z(3, 2, 1, 1, 1, 3, 0, t) ) +
    0.5 * ( z(3, 2, 1, 3, 0, 0, 0, t) +
            z(1, 3, 2, 0, 0, 0, 0, t) +
            z(2, 1, 3, 2, 3, 0, 0, t) +
            z(3, 1, 2, 1, 1, 0, 0, t) +
            z(1, 2, 3, 1, 1, 2, 3, t) +
            z(2, 3, 1, 1, 1, 3, 0, t) ) );
};

double TDI::Xm(double t) {
    return( y(1,-3, 2, 3, 2,-2, t) -
            y(1, 2, 3,-2,-3, 3, t) +
            y(2, 3, 1, 2,-2, 0, t) -
            y(3,-2, 1,-3, 3, 0, t) +
            y(1, 2, 3,-2, 0, 0, t) -
            y(1,-3, 2, 3, 0, 0, t) + 
            y(3,-2, 1, 0, 0, 0, t) -
            y(2, 3, 1, 0, 0, 0, t) +
    0.5 * (-z(3,-2, 1, 2,-2,-3, 3, t) +
            z(3,-2, 1,-3, 3, 0, 0, t) +
            z(3,-2, 1, 2,-2, 0, 0, t) -
            z(3,-2, 1, 0, 0, 0, 0, t) ) +
    0.5 * ( z(2, 3, 1,-3, 3, 2,-2, t) -
            z(2, 3, 1,-3, 3, 0, 0, t) -
            z(2, 3, 1, 2,-2, 0, 0, t) +
            z(2, 3, 1, 0, 0, 0, 0, t) ) );
}

double TDI::Xmlock1(double t) {
    return( y(2, 3, 1, 2,-2, 0, t) -
            y(3,-2, 1,-3, 3, 0, t) +
            y(3,-2, 1, 0, 0, 0, t) -
            y(2, 3, 1, 0, 0, 0, t) );
}

double TDI::Xmlock2(double t) {
    return( y(1,-3, 2, 3, 2,-2, t) -
            y(1, 2, 3,-2,-3, 3, t) -
            y(3,-2, 1,-3, 3, 0, t) +
            y(1, 2, 3,-2, 0, 0, t) -
            y(1,-3, 2, 3, 0, 0, t) + 
            y(3,-2, 1, 0, 0, 0, t) );
}

double TDI::Xmlock3(double t) {
    return( y(1,-3, 2, 3, 2,-2, t) -
            y(1, 2, 3,-2,-3, 3, t) +
            y(2, 3, 1, 2,-2, 0, t) +
            y(1, 2, 3,-2, 0, 0, t) -
            y(1,-3, 2, 3, 0, 0, t) -
            y(2, 3, 1, 0, 0, 0, t) );
}

double TDI::Ym(double t) {
    return( y(2,-1, 3, 1, 3,-3, t)
           -y(2, 3, 1,-3,-1, 1, t)
           +y(3, 1, 2, 3,-3, 0, t)
           -y(1,-3, 2,-1, 1, 0, t)
           +y(2, 3, 1,-3, 0, 0, t)
           -y(2,-1, 3, 1, 0, 0, t)
           +y(1,-3, 2, 0, 0, 0, t)
           -y(3, 1, 2, 0, 0, 0, t) +
    0.5 * ( z(3, 1, 2,-1, 1, 3,-3, t)
           -z(3, 1, 2,-1, 1, 0, 0, t)
           -z(3, 1, 2, 3,-3, 0, 0, t)
           +z(3, 1, 2, 0, 0, 0, 0, t) ) +
    0.5 * (-z(1,-3, 2, 3,-3,-1, 1, t)
           +z(1,-3, 2,-1, 1, 0, 0, t)
           +z(1,-3, 2, 3,-3, 0, 0, t)
           -z(1,-3, 2, 0, 0, 0, 0, t) ) );
}

double TDI::Zm(double t) {
    return( y(3,-2, 1, 2, 1,-1, t)
           -y(3, 1, 2,-1,-2, 2, t)
           +y(1, 2, 3, 1,-1, 0, t)
           -y(2,-1, 3,-2, 2, 0, t)
           +y(3, 1, 2,-1, 0, 0, t)
           -y(3,-2, 1, 2, 0, 0, t)
           +y(2,-1, 3, 0, 0, 0, t) 
           -y(1, 2, 3, 0, 0, 0, t) +
    0.5 * (-z(2,-1, 3, 1,-1,-2, 2, t)
           +z(2,-1, 3, 1,-1, 0, 0, t)
           +z(2,-1, 3,-2, 2, 0, 0, t)
           -z(2,-1, 3, 0, 0, 0, 0, t) ) +
    0.5 * ( z(1, 2, 3,-2, 2, 1,-1, t)
           -z(1, 2, 3,-2, 2, 0, 0, t)
           -z(1, 2, 3, 1,-1, 0, 0, t)
           +z(1, 2, 3, 0, 0, 0, 0, t) ) );
}

double TDI::X1(double t) {
    return( y(1,-3, 2, 3, 2,-2, 2,-2,-3, 3, t) 
	   -y(1, 2, 3,-2,-3, 3,-3, 3, 2,-2, t) 
	   +y(2, 3, 1, 2,-2, 2,-2,-3, 3, 0, t) 
	   -y(3,-2, 1,-3, 3,-3, 3, 2,-2, 0, t) 
	   +y(1, 2, 3,-2, 2,-2,-3, 3, 0, 0, t) 
	   -y(1,-3, 2, 3,-3, 3, 2,-2, 0, 0, t) 
	   +y(3,-2, 1, 2,-2,-3, 3, 0, 0, 0, t)
	   -y(2, 3, 1,-3, 3, 2,-2, 0, 0, 0, t) 
	   +y(1, 2, 3,-2,-3, 3, t) 
	   -y(1,-3, 2, 3, 2,-2, t) 
	   +y(3,-2, 1,-3, 3, 0, t) 
	   -y(2, 3, 1, 2,-2, 0, t) 
	   +y(1,-3, 2, 3, 0, 0, t) 
	   -y(1, 2, 3,-2, 0, 0, t) 
	   +y(2, 3, 1, 0, 0, 0, t) 
	   -y(3,-2, 1, 0, 0, 0, t) +
    0.5 * ( z(2, 3, 1,-3, 3, 2,-2, 2,-2,-3, 3, t)
	   -z(2, 3, 1,-3, 3,-3, 3, 2,-2, 0, 0, t)
	   -z(2, 3, 1, 2,-2, 2,-2,-3, 3, 0, 0, t)
	   +z(2, 3, 1,-3, 3, 0, 0, t)
	   +z(2, 3, 1, 2,-2, 0, 0, t)
	   -z(2, 3, 1, 0, 0, 0, 0, t) ) +
    0.5 * (-z(3,-2, 1, 2,-2,-3, 3,-3, 3, 2,-2, t)
	   +z(3,-2, 1,-3, 3,-3, 3, 2,-2, 0, 0, t)
	   +z(3,-2, 1, 2,-2, 2,-2,-3, 3, 0, 0, t)
	   -z(3,-2, 1,-3, 3, 0, 0, t)
	   -z(3,-2, 1, 2,-2, 0, 0, t)
	   +z(3,-2, 1, 0, 0, 0, 0, t) ) );
}

double TDI::X2(double t) {
    return( y(2,-1, 3, 1, 3,-3, 3,-3,-1, 1, t) 
	   -y(2, 3, 1,-3,-1, 1,-1, 1, 3,-3, t) 
	   +y(3, 1, 2, 3,-3, 3,-3,-1, 1, 0, t) 
	   -y(1,-3, 2,-1, 1,-1, 1, 3,-3, 0, t) 
	   +y(2, 3, 1,-3, 3,-3,-1, 1, 0, 0, t) 
	   -y(2,-1, 3, 1,-1, 1, 3,-3, 0, 0, t) 
	   +y(1,-3, 2, 3,-3,-1, 1, 0, 0, 0, t)
	   -y(3, 1, 2,-1, 1, 3,-3, 0, 0, 0, t) 
	   +y(2, 3, 1,-3,-1, 1, t) 
	   -y(2,-1, 3, 1, 3,-3, t) 
	   +y(1,-3, 2,-1, 1, 0, t) 
	   -y(3, 1, 2, 3,-3, 0, t) 
	   +y(2,-1, 3, 1, 0, 0, t) 
	   -y(2, 3, 1,-3, 0, 0, t) 
	   +y(3, 1, 2, 0, 0, 0, t) 
	   -y(1,-3, 2, 0, 0, 0, t) +
    0.5 * ( z(3, 1, 2,-1, 1, 3,-3, 3,-3,-1, 1, t)
	   -z(3, 1, 2,-1, 1,-1, 1, 3,-3, 0, 0, t)
	   -z(3, 1, 2, 3,-3, 3,-3,-1, 1, 0, 0, t)
	   +z(3, 1, 2,-1, 1, 0, 0, t)
	   +z(3, 1, 2, 3,-3, 0, 0, t)
	   -z(3, 1, 2, 0, 0, 0, 0, t) ) +
    0.5 * (-z(1,-3, 2, 3,-3,-1, 1,-1, 1, 3,-3, t)
	   +z(1,-3, 2,-1, 1,-1, 1, 3,-3, 0, 0, t)
	   +z(1,-3, 2, 3,-3, 3,-3,-1, 1, 0, 0, t)
	   -z(1,-3, 2,-1, 1, 0, 0, t)
	   -z(1,-3, 2, 3,-3, 0, 0, t)
	   +z(1,-3, 2, 0, 0, 0, 0, t) ) );
}

double TDI::X3(double t) {
    return( y(3,-2, 1, 2, 1,-1, 1,-1,-2, 2, t) 
	   -y(3, 1, 2,-1,-2, 2,-2, 2, 1,-1, t) 
	   +y(1, 2, 3, 1,-1, 1,-1,-2, 2, 0, t) 
	   -y(2,-1, 3,-2, 2,-2, 2, 1,-1, 0, t) 
	   +y(3, 1, 2,-1, 1,-1,-2, 2, 0, 0, t) 
	   -y(3,-2, 1, 2,-2, 2, 1,-1, 0, 0, t) 
	   +y(2,-1, 3, 1,-1,-2, 2, 0, 0, 0, t)
	   -y(1, 2, 3,-2, 2, 1,-1, 0, 0, 0, t) 
	   +y(3, 1, 2,-1,-2, 2, t) 
	   -y(3,-2, 1, 2, 1,-1, t) 
	   +y(2,-1, 3,-2, 2, 0, t) 
	   -y(1, 2, 3, 1,-1, 0, t) 
	   +y(3,-2, 1, 2, 0, 0, t) 
	   -y(3, 1, 2,-1, 0, 0, t) 
	   +y(1, 2, 3, 0, 0, 0, t) 
	   -y(2,-1, 3, 0, 0, 0, t) +
    0.5 * ( z(1, 2, 3,-2, 2, 1,-1, 1,-1,-2, 2, t)
	   -z(1, 2, 3,-2, 2,-2, 2, 1,-1, 0, 0, t)
	   -z(1, 2, 3, 1,-1, 1,-1,-2, 2, 0, 0, t)
	   +z(1, 2, 3,-2, 2, 0, 0, t)
	   +z(1, 2, 3, 1,-1, 0, 0, t)
	   -z(1, 2, 3, 0, 0, 0, 0, t) ) +
    0.5 * (-z(2,-1, 3, 1,-1,-2, 2,-2, 2, 1,-1, t)
	   +z(2,-1, 3,-2, 2,-2, 2, 1,-1, 0, 0, t)
	   +z(2,-1, 3, 1,-1, 1,-1,-2, 2, 0, 0, t)
	   -z(2,-1, 3,-2, 2, 0, 0, t)
	   -z(2,-1, 3, 1,-1, 0, 0, t)
	   +z(2,-1, 3, 0, 0, 0, 0, t) ) );
}

// fast C++ replacement for getobs and getobsc in lisautils.py

static void showtime(long maxi,long maxlength,time_t begtime) {
    double percdone = (100.0 * maxi) / maxlength;

    time_t elapsed = time(NULL) - begtime;

    int vel = int((1.0 * maxi) / elapsed);

    int timeleft = int(((1.0 * elapsed) / maxi) * (maxlength - maxi));

    int minleft = timeleft / 60;
    int secleft = timeleft - minleft*60;

    char buffer[256];

    if(maxi == maxlength) {
        sprintf(buffer,"...completed in %ld s [%d rows/s].",long(elapsed),vel);
        fprintf(stderr,"\r%-80s\n",buffer);
    } else {
        sprintf(buffer,"...%ld/%ld (%.2f%%) done [%d rows/s], %dm%ds left...",maxi,maxlength,percdone,vel,minleft,secleft);
        fprintf(stderr,"\r%-80s",buffer);
        fflush(stderr);
    }
    
    // if(PyErr_CheckSignals() != 0) {
    // 		ExceptionKeyboardInterrupt e;

    // 		throw e;
    // }
}

void fastgetobsc(double *buffer,long length,long samples,double stime,Signal **thesignals,int signals,double inittime) {
    long maxlength = length < samples ? length : samples;

    // divide up the cycle into batches of 16384
    const int batchlen = 16384;
    
    int batches = (maxlength % batchlen) == 0 ? (maxlength / batchlen) : (maxlength / batchlen + 1);

    time_t begtime = time(NULL);

    fprintf(stderr,"Processing (running enhanced TDI C++ cycle)...");
    fflush(stderr);

    for(int b=0;b<batches;b++) {
        long mini = b * batchlen;
        long maxi = (mini + batchlen) < maxlength ? (mini + batchlen) : maxlength;

        for(long i=mini;i<maxi;i++) {
            double t = inittime + stime * i;

            for(int j=0;j<signals;j++) {
                buffer[i*signals + j] = thesignals[j]->value(t);
            }
        }

        showtime(maxi,maxlength,begtime);
    }
}

void fastgetobs(double *buffer,long length,long samples,double stime,Signal **thesignals,int signals,double inittime) {
    long maxlength = length < samples ? length : samples;

    for(int i=0;i<maxlength;i++) {
        double t = inittime + stime * i;
    
        for(int j=0;j<signals;j++) {
            buffer[i*signals + j] = thesignals[j]->value(t);
        }
    }
}

SampledTDI::SampledTDI(LISA *l,Noise *yijk[6],Noise *zijk[6]) {
    // the convention is {12,21,23,32,31,13}

	lisa = l;

    for(int craft1 = 1; craft1 <= 3; craft1++) {
        for(int craft2 = 1; craft2 <= 3; craft2++) {
            if(craft1 != craft2) {
				if( (craft1 == 1 && craft2 == 2) || (craft1 == 2 && craft2 == 3) || (craft1 == 3 && craft2 == 1) ) {
		    		yobj[craft1][craft2] = yijk[2*(craft1-1)];
		    		zobj[craft1][craft2] = zijk[2*(craft1-1)];
		        } else {
		    	    yobj[craft1][craft2] = yijk[2*(craft2-1)+1];
		    	    zobj[craft1][craft2] = zijk[2*(craft2-1)+1];
		        }		        
	    	}
        }
    }
}

void SampledTDI::reset(unsigned long seed) {
    for(int craft1 = 1; craft1 <= 3; craft1++) {
        for(int craft2 = 1; craft2 <= 3; craft2++) {
            if(craft1 != craft2) {
                yobj[craft1][craft2]->reset();
                zobj[craft1][craft2]->reset();
            }
        }
    }
}

double SampledTDI::y(int send, int slink, int recv, int ret1, int ret2, int ret3, double t) {
    return y(send,slink,recv,ret1,ret2,ret3,0,0,0,0,t);
}

double SampledTDI::z(int send, int slink, int recv, int ret1, int ret2, int ret3, int ret4, double t) {
    return z(send,slink,recv,ret1,ret2,ret3,ret4,0,0,0,0,t);
}

// can improve precision by calling value with two times
// (assuming the underlying implementation supports it)

double SampledTDI::y(int send, int slink, int recv, int ret1, int ret2, int ret3, int ret4, int ret5, int ret6, int ret7, double t) {
    lisa->newretardtime(t);

    lisa->retard(ret7); lisa->retard(ret6); lisa->retard(ret5);
    lisa->retard(ret4); lisa->retard(ret3); lisa->retard(ret2); lisa->retard(ret1);

    return yobj[send][recv]->value(lisa->retardedtime());
}

double SampledTDI::z(int send, int slink, int recv, int ret1, int ret2, int ret3, int ret4, int ret5, int ret6, int ret7, int ret8, double t) {
    lisa->newretardtime(t);

    lisa->retard(ret8); lisa->retard(ret7); lisa->retard(ret6); lisa->retard(ret5);
    lisa->retard(ret4); lisa->retard(ret3); lisa->retard(ret2); lisa->retard(ret1);

    return zobj[send][recv]->value(lisa->retardedtime());
}

double SampledTDIaccurate::y(int send, int slink, int recv, int ret1, int ret2, int ret3, int ret4, int ret5, int ret6, int ret7, double t) {
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

    // lisa->retard(ret7); lisa->retard(ret6); lisa->retard(ret5);
    // lisa->retard(ret4); lisa->retard(ret3); lisa->retard(ret2); lisa->retard(ret1);

    // return dopplerfactor * yobj[send][recv]->value(t,-lisa->retardation());
    return yobj[send][recv]->value(t,-lisa->retardation());
}

double SampledTDIaccurate::z(int send, int slink, int recv, int ret1, int ret2, int ret3, int ret4, int ret5, int ret6, int ret7, int ret8, double t) {
    lisa->newretardtime(t);

    lisa->retard(ret8); lisa->retard(ret7); lisa->retard(ret6); lisa->retard(ret5);
    lisa->retard(ret4); lisa->retard(ret3); lisa->retard(ret2); lisa->retard(ret1);

    return zobj[send][recv]->value(t,-lisa->retardation());
}
