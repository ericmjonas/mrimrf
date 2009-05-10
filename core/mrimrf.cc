#include <mrimrf.h>
#include "util.h"
#include "types.h"
#include "sw.h"

MRIMRF::MRIMRF(int MaxWrapCount, phase_cube_t obsval) :
  MAXWRAPCOUNT_(MaxWrapCount),
  observation_(obsval), 
  latentPhaseWraps_(boost::extents[obsval.shape()[0]][obsval.shape()[1]][obsval.shape()[2]]),
  temp_(1.0), 
  score_(0.0)
{
  score_ = recomputeLogScore(); 

}

void MRIMRF::sequential_gibbs_scan()
{
  for(int i = 0; i < latentPhaseWraps_.shape()[0]; ++i) {
    for(int j = 0; j < latentPhaseWraps_.shape()[1]; ++j) {
      for(int k = 0; k < latentPhaseWraps_.shape()[2]; ++k) {
	gibbsAtVoxel(i, j, k); 
      }
    }
  }

}

void MRIMRF::random_gibbs_scan()
{

  // huge vector, create on heap
  for (int i = 0; i < latentPhaseWraps_.num_elements(); i++) {
    int i = intrand(rng_, 0, latentPhaseWraps_.shape()[0]-1); 
    int j = intrand(rng_, 0, latentPhaseWraps_.shape()[1]-1); 
    int k = intrand(rng_, 0, latentPhaseWraps_.shape()[2]-1); 
    gibbsAtVoxel(i, j, k); 
  }

}


void MRIMRF::gibbsAtVoxel(int i, int j, int k)
{
  probvector_t probvect(MAXWRAPCOUNT_ * 2 + 1); 

  float score_contrib_from_original_value = computeLogScoreAtVoxel(i, j, k); 

  float obsval = observation_[i][j][k]; 
  
  for(int phase_cycles = -MAXWRAPCOUNT_; phase_cycles < (MAXWRAPCOUNT_ + 1);
      phase_cycles++) {

    latentPhaseWraps_[i][j][k] = phase_cycles; 
    
    float score = computeLogScoreAtVoxel(i, j, k); 

    probvect[phase_cycles + MAXWRAPCOUNT_] = exp(score); 
    
  }
  
//   double sum = logSumProbVect(probvect); 
//   for(int iq = 0; iq < probvect.size(); iq++) {
//     probvect[iq] = exp(probvect[iq] - sum); 
//   }
//   int phasesel = sampleFromProbabilities(rng_, probvect); 

//   latentPhaseWraps_[i][j][k] = obsval + 2*PI*(phasesel - MAXWRAPCOUNT_); 
  
  double sum = 0.0; 
  for(int qi = 0; qi < probvect.size(); qi++) {
    sum += probvect[qi]; 
  }
  
  for(int qi = 0; qi < probvect.size(); qi++) {
    probvect[qi] = probvect[qi] / sum; 
  }
  
  int phasesel = sampleFromProbabilities(rng_, probvect); 

  latentPhaseWraps_[i][j][k] = phasesel - MAXWRAPCOUNT_; 
  float score_contrib_from_new_value = computeLogScoreAtVoxel(i, j, k); 

  float scoredelta = score_contrib_from_new_value - 
    score_contrib_from_original_value; 
  score_ += scoredelta; 

}

phase_cube_t MRIMRF::getLatentPhases()
{
  phase_cube_t pc(boost::extents[latentPhaseWraps_.shape()[0]][latentPhaseWraps_.shape()[1]][latentPhaseWraps_.shape()[2]]);
  
  // compute latent phases
  for(int i = 0; i < latentPhaseWraps_.shape()[0]; ++i) {
    for(int j = 0; j < latentPhaseWraps_.shape()[1]; ++j) {
      for(int k = 0; k < latentPhaseWraps_.shape()[2]; ++k) {
	pc[i][j][k] = getCurrentPhaseValue(i, j, k); 
      }
    }
  }
  
  return pc; 
  
}

wrap_cube_t MRIMRF::getLatentPhaseWraps()
{
  return latentPhaseWraps_; 

}

void MRIMRF::setLatentPhaseWraps(const wrap_cube_t & v)
{
  for(int i = 0; i < 3; i++) {
    if(latentPhaseWraps_.shape()[i] != v.shape()[i]) {
      throw std::runtime_error("attempted to set latent vars with invalid shape"); 
    }
  }
  latentPhaseWraps_ = v; 
  
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
  score_ = recomputeLogScore();

}

float MRIMRF::getTemp()
{
  return temp_; 
}

void MRIMRF::setSeed(int x)
{
  rng_.seed(x); 
}


float MRIMRF::computeLogScoreAtVoxel(int i, int j, int k)
{
  float current_phase_value = getCurrentPhaseValue(i, j, k); 

  // now for each of the neighboring pixels, compute the score, and
  // add it to the total score
  
  float score(0.0); 
  
  if(i > 0) { 
    score += gauss_markov_prior(current_phase_value, getCurrentPhaseValue(i-1,j,k)); 
  }
  
  if(i < (observation_.shape()[0]-1)) { 
    score += gauss_markov_prior(current_phase_value, getCurrentPhaseValue(i+1,j,k)); 
  }
  
  if(j > 0) { 
    score += gauss_markov_prior(current_phase_value, getCurrentPhaseValue(i,j-1,k)); 
  }
  
  if(j < (observation_.shape()[1]-1)) { 
    score += gauss_markov_prior(current_phase_value, getCurrentPhaseValue(i,j+1,k)); 
  }
  
  if(k > 0) { 
    score += gauss_markov_prior(current_phase_value, getCurrentPhaseValue(i,j,k-1)); 
  }
  
  if(k < (observation_.shape()[2]-1)) { 
    score += gauss_markov_prior(current_phase_value, getCurrentPhaseValue(i,j,k+1)); 
  }
  
  return -score/temp_; 
}

float MRIMRF::recomputeLogScore()
{

  float score(0.0); 
  // FIXME : Do we double-count here? each factor is shared by two edges!

  // Initially compute score
  for(int i = 0; i < latentPhaseWraps_.shape()[0]; ++i) {
    for(int j = 0; j < latentPhaseWraps_.shape()[1]; ++j) {
      for(int k = 0; k < latentPhaseWraps_.shape()[2]; ++k) {
	score += computeLogScoreAtVoxel(i, j, k); 
      }
    }
  }

  return score; 


}

void MRIMRF::swendsenWangMove()
{

  graph_t g = wrap_cube_to_graph(latentPhaseWraps_); 
  
  disconnect_nonsimilar_phase_edges(g); 

  flip_edges_off(g, rng_, 0.3); 

  // randomly pick a 

}

coloring_cube_t MRIMRF::getColoring()
{
  graph_t g = wrap_cube_to_graph(latentPhaseWraps_); 
  
  disconnect_nonsimilar_phase_edges(g); 

  flip_edges_off(g, rng_, 0.3); 

  coords_t c; 
  c[0] = latentPhaseWraps_.shape()[0];
  c[1] = latentPhaseWraps_.shape()[1];
  c[2] = latentPhaseWraps_.shape()[2];

  coloring_cube_t c1 = graph_to_coloring_cube(g, c); 

  return c1; 
}
