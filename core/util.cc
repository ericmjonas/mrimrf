#include "util.h"

double unirand(rng_t & generator)
{

  boost::uniform_real<> uni_dist(0,1);

  boost::variate_generator<rng_t&,
    boost::uniform_real<> > unigen(generator, uni_dist); 
  
  return unigen(); 
}

int intrand(rng_t & gen, int low, int high)
{
  boost::uniform_int<> int_dist(low, high); 
  
  boost::variate_generator<rng_t&, boost::uniform_int<> > die_gen(gen, int_dist); 

  return die_gen(); 
}

double logsumexp(double x, double y)
{
  double min, max; 
  if (x < y) { 
    min = x; 
    max = y; 
  } else {
    min = y; 
    max = x; 
  }
  
  return max + log(1 +exp(min - max)); 

}

int sampleFromProbabilities(rng_t& gen, const probvector_t & pv)
{
  // given a probability vector pv, sample from it and return the resulting index; 

  int index = 0; 
  double r = unirand(gen); 
  double probsum = 0;

  probvector_t::const_iterator p; 
  for (p = pv.begin(); p != pv.end(); p++) {
    probsum += *p; 
    if (r <= probsum) {
      return index; 
    }
    index++; 
  }
  
  return index-1; 

}


double logSumProbVect(const probvector_t & lprobs){
  /// compute log (\sum exp(lprobs))

  probvector_t::const_iterator i = lprobs.begin(); 
  double sum = *i;
  ++i; 
  while (i != lprobs.end()) {
    sum = logsumexp(sum, *i); 
    ++i;
  }

  return sum; 

}

bool binomial(float p, rng_t& rng)
{
  float x= unirand(rng); 
  if (x < p) {
    return true; 

  }
  
  return false; 


}
