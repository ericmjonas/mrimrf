#include <mrimrf.h>
#include <boost/graph/connected_components.hpp>

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
  generateDataDrivenPartitioning(0.5, 0.1); 
  
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

  return score / 2;  // we double count


}

void MRIMRF::swendsenWangMove()
{

  swendsen_wang(latentPhaseWraps_, rng_, -MAXWRAPCOUNT_, MAXWRAPCOUNT_); 
  
}

coloring_cube_t MRIMRF::getColoring()
{
  graph_t g = wrap_cube_to_graph(latentPhaseWraps_); 
  
  disconnect_nonsimilar_phase_edges(g); 

  coords_t c; 
  c[0] = latentPhaseWraps_.shape()[0];
  c[1] = latentPhaseWraps_.shape()[1];
  c[2] = latentPhaseWraps_.shape()[2];

  coloring_cube_t c1 = graph_to_coloring_cube(g, c); 

  return c1; 
}

bool MRIMRF::ddmcmc_flip_gibbs(float prob, float delta)
{
  /*
    This is an attempt at doing a data-driven version of something
    swendsen-wang-like. Basically we just probabilisticly flip
    
    1. randomly partition the space into a collection of 
    segments, informed by the data

    2. gibbs one of those segments
    
    3. done! 

    I'm not really convinced we can gibbs here (vs M-H) but we'll see!

    
   */
  
  generateDataDrivenPartitioning(prob, delta); 

  graph_t g = partitioning_; 
  // now find connected comps
  std::vector<int> component(num_vertices(g));
  int numcomp = connected_components(g, &component[0]); 

  std::cout << "The graph has " << num_edges(g) << " edges"
	    << " and " <<  numcomp << " components" 
	    << " prob = " << prob << std::endl;
  
  
  // now get those vertices? 
  for (int n = 0; n < 1; n++) {
    int comp_to_change = intrand(rng_, 0, numcomp); 
    
    std::list<coords_t> coordinates; 
    
    typedef graph_t::vertex_iterator vi_t; 
    std::pair<vi_t, vi_t> vp = vertices(g); 
    int pos = 0; 
    for(vi_t v = vp.first; v != vp.second; ++v) {
      if(component[pos] == comp_to_change) { 
	coordinates.push_back(g[*v].coordinates); 
      }
      pos++; 
    }
    
    // now, gibbs on this set of components
    probvector_t probvect; 
    probvect.reserve(MAXWRAPCOUNT_*2+1); 
    for (int wrap_i = -MAXWRAPCOUNT_; wrap_i <= MAXWRAPCOUNT_; ++wrap_i) {
      
      // set all of the phase wraps in this component
      for(std::list<coords_t>::const_iterator ci = coordinates.begin(); 
	ci != coordinates.end(); ci++ ){
	coords_t c = *ci; 
	
	latentPhaseWraps_[c[0]][c[1]][c[2]] = wrap_i;
      }
      double s= recomputeLogScore(); 
      probvect.push_back(s); 
      
      
    }
    
    double sum =  logSumProbVect(probvect);
    for(int qi = 0; qi < probvect.size(); qi++) {
      probvect[qi] = exp(probvect[qi] - sum); 
      //std::cout << probvect[qi] << " "; 
    }
    //std::cout << std::endl; 

    int phasesel = sampleFromProbabilities(rng_, probvect); 
    
    for(std::list<coords_t>::const_iterator ci = coordinates.begin(); 
	ci != coordinates.end(); ci++ ){
      coords_t c = *ci; 
      
      latentPhaseWraps_[c[0]][c[1]][c[2]] = phasesel - MAXWRAPCOUNT_; 
    }
    
    score_ = recomputeLogScore(); 
  }
    
}
void MRIMRF::generateDataDrivenPartitioning(float prob, float delta)
{
  
  partitioning_ = data_based_graph(observation_, rng_, prob, delta); 
  
}

coloring_cube_t MRIMRF::getCurrentPartitioning()
{
  
  coords_t c; 
  c[0] = observation_.shape()[0]; 
  c[1] = observation_.shape()[1]; 
  c[2] = observation_.shape()[2]; 

  return graph_to_coloring_cube(partitioning_, c); 

}
