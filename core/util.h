#ifndef UTIL_H
#define UTIL_H

#include "types.h"

double unirand(rng_t& gen); 
int intrand(rng_t& gen, int low, int high); 

double gaussrand(rng_t& gen, double mean, double std); 


double logsumexp(double x, double y); 

int sampleFromProbabilities(rng_t & gen, 
			    const probvector_t & pv); 
double logSumProbVect(const probvector_t & lprobs); 

const float PI(3.14159265); 

#endif
