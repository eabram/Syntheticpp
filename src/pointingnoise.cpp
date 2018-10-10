double BreathingAngleError(double angle) {
  // convert angle jitter to OPD jitter
  // trivial for now, but it is complex and should be updated
  // 
  const double angle2OPD = 1e-12;
  return angle * angle2OPD;
}

double BreathingAngle(LISA l, double t) {
  Vector v, u;
  l->putn(v, 1, t);
  l->putn(u, 2, t);

  // definition of dotproduct as found in lisasim-tens.h 
  double angle = dotproduct(u, v);
  
  return BreathingAngleError(angle);
}


Noise *breathinganglenoise(LISA *lisa, double t) {
  // Noise induced by (mis)pointing while correcting
  // for the breathing angle
  
  double angle = BreathingAngle(lisa, t);
  return BreathingAngleError(angle);
}
