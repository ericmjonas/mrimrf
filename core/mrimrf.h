#ifndef __MRIMRF_H__
#define __MRIMRF_H__

#include <boost/utility.hpp>
#include "types.h"


class MRIMRF : boost::noncopyable
{
public:
  MRIMRF(int MaxWrapCount, phase_cube_t obsval);
  
  void sequential_gibbs_scan();
  void random_gibbs_scan();
 
  phase_cube_t getLatentVals(); 
  void setLatentVals(const phase_cube_t & v);
  void setTemp(float t); 
  float getTemp(); 
  void setSeed(int s); 

private:
  const int MAXWRAPCOUNT_; 
  const phase_cube_t observation_; 
  phase_cube_t latentVals_; 
  observation_tensor_t obsTensor_; 

  void gibbsAtVoxel(int i, int j, int k); 
  inline float gauss_markov_prior(float x1, float x2) {
    return (x1 -x2) * (x1 - x2); 
  }

  rng_t rng_; 
  float temp_; 

}; 


#endif // __MRIMRF_H__
