#include <mrimrf.h>
#include "util.h"

MRIMRF::MRIMRF(int MaxWrapCount, phase_cube_t obsval) :
  MAXWRAPCOUNT_(MaxWrapCount),
  observation_(obsval), 
  latentVals_(boost::extents[obsval.shape()[0]][obsval.shape()[1]][obsval.shape()[2]]),
  temp_(1.0)
{

}

void MRIMRF::sequential_gibbs_scan()
{
  for(int i = 0; i < latentVals_.shape()[0]; ++i) {
    for(int j = 0; j < latentVals_.shape()[1]; ++j) {
      for(int k = 0; k < latentVals_.shape()[2]; ++k) {
	gibbsAtVoxel(i, j, k); 
      }
    }
  }

}

void MRIMRF::random_gibbs_scan()
{

  // huge vector, create on heap
  for (int i = 0; i < latentVals_.num_elements(); i++) {
    int i = intrand(rng_, 0, latentVals_.shape()[0]-1); 
    int j = intrand(rng_, 0, latentVals_.shape()[1]-1); 
    int k = intrand(rng_, 0, latentVals_.shape()[2]-1); 
    gibbsAtVoxel(i, j, k); 
  }

}

void MRIMRF::gibbsAtVoxel(int i, int j, int k)
{
  probvector_t probvect(MAXWRAPCOUNT_ * 2 + 1); 

  float obsval = observation_[i][j][k]; 

  for(int phase_cycles = -MAXWRAPCOUNT_; phase_cycles < (MAXWRAPCOUNT_ + 1);
      phase_cycles++) {
    
    float possibleobsval = obsval + 2*PI*phase_cycles; 
    
    // now for each of the neighboring pixels, compute the score, and
    // add it to the total score

    float score(0.0); 
     
    if(i > 0) { 
      score += gauss_markov_prior(possibleobsval, latentVals_[i-1][j][k]); 
    }
    
    if(i < (observation_.shape()[0]-1)) { 
      score += gauss_markov_prior(possibleobsval, latentVals_[i+1][j][k]); 
    }
    
    if(j > 0) { 
      score += gauss_markov_prior(possibleobsval, latentVals_[i][j-1][k]); 
    }
    
    if(j < (observation_.shape()[1]-1)) { 
      score += gauss_markov_prior(possibleobsval, latentVals_[i][j+1][k]); 
    }
    
    if(k > 0) { 
      score += gauss_markov_prior(possibleobsval, latentVals_[i][j][k-1]); 
    }
    
    if(k < (observation_.shape()[2]-1)) { 
      score += gauss_markov_prior(possibleobsval, latentVals_[i][j][k+1]); 
    }
    
    probvect[phase_cycles + MAXWRAPCOUNT_] = exp(-score / temp_); 
    
  }
  
//   double sum = logSumProbVect(probvect); 
//   for(int iq = 0; iq < probvect.size(); iq++) {
//     probvect[iq] = exp(probvect[iq] - sum); 
//   }
//   int phasesel = sampleFromProbabilities(rng_, probvect); 

//   latentVals_[i][j][k] = obsval + 2*PI*(phasesel - MAXWRAPCOUNT_); 
  
  double sum = 0.0; 
  for(int qi = 0; qi < probvect.size(); qi++) {
    sum += probvect[qi]; 
  }
  
  for(int qi = 0; qi < probvect.size(); qi++) {
    probvect[qi] = probvect[qi] / sum; 
  }
  
  int phasesel = sampleFromProbabilities(rng_, probvect); 

  latentVals_[i][j][k] = obsval + 2*PI*(phasesel - MAXWRAPCOUNT_); 
  
}

phase_cube_t MRIMRF::getLatentVals()
{
  return latentVals_; 

}

void MRIMRF::setLatentVals(const phase_cube_t & v)
{
  for(int i = 0; i < 3; i++) {
    if(latentVals_.shape()[i] != v.shape()[i]) {
      throw std::runtime_error("attempted to set latent vars with invalid shape"); 
    }
  }
  latentVals_ = v; 
  
//   for (int i = 0; i < v.shape()[0]; i++) {
//     std::cout << "I = " << i << std::endl; 
//     for (int j = 0; j < v.shape()[1]; j++) {
//       for (int k = 0; k < v.shape()[2]; k++) {
// 	std::cout << v[i][j][k] << " ";
//       }
//       std::cout << std::endl; 
//     }
//     std::cout << std::endl;
//   }

//   std::cout << "latentVals[0, 3, 4]:"  
// 	    << v[0][3][4] << std::endl;
  
}

void MRIMRF::setTemp(float f)
{
  temp_ = f; 

}

float MRIMRF::getTemp()
{
  return temp_; 
}
