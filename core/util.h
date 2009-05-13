#ifndef UTIL_H
#define UTIL_H

#include "types.h"

double unirand(rng_t& gen); 
int intrand(rng_t& gen, int low, int high); 

bool binomial(float p, rng_t & rng); 

double gaussrand(rng_t& gen, double mean, double std); 


double logsumexp(double x, double y); 

int sampleFromProbabilities(rng_t & gen, 
			    const probvector_t & pv); 
double logSumProbVect(const probvector_t & lprobs); 

const float PI(3.14159265); 

inline float max(float f1, float f2)
{
  if (f1 < f2)
    return f2; 
  return f1; 

}

inline float min(float f1, float f2)
{
  if (f1 < f2)
    return f1; 
  return f2; 

}

#endif
