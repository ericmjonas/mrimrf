#ifndef __MRIMRF_H__
#define __MRIMRF_H__

#include <boost/utility.hpp>
#include "types.h"
#include "util.h"


class MRIMRF : boost::noncopyable
{
public:
  MRIMRF(int MaxWrapCount, phase_cube_t obsval);
  
  void sequential_gibbs_scan();
  void random_gibbs_scan();
 
  phase_cube_t getLatentPhases(); 
  wrap_cube_t getLatentPhaseWraps(); 
  void setLatentPhaseWraps(const wrap_cube_t & wc); 

  void setTemp(float t); 
  float getTemp(); 
  void setSeed(int s); 
  
  float getLogScore() { 
    return score_; 
  }
  
  coloring_cube_t getColoring(); 
  void swendsenWangMove(); 


private:
  const int MAXWRAPCOUNT_; 
  const phase_cube_t observation_; 
  wrap_cube_t latentPhaseWraps_; 

  void gibbsAtVoxel(int i, int j, int k); 
  inline float gauss_markov_prior(float x1, float x2) {
    return (x1 -x2) * (x1 - x2); 
  }
  
  float recomputeLogScore(); 

  inline float getCurrentPhaseValue(int i, int j, int k) {
    float phase = observation_[i][j][k] + 2*PI*latentPhaseWraps_[i][j][k]; 
    return phase;
  }
  
  float computeLogScoreAtVoxel(int i, int j, int k); 

  rng_t rng_; 
  float temp_; 
  float score_; 
}; 


#endif // __MRIMRF_H__
