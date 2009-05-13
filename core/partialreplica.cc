#include "partialreplica.h"
#include <boost/graph/connected_components.hpp>


void partial_replica_exchange_move(MRIMRF & primary_replica, 
				   MRIMRF & secondary_replica, int attemptcount)
{
  /*
    Take the first replica's partitioning of space as the 
    partitioning that we're considering. 
    
    // right now we do deterministic crossover


    We use the primary replica's RNG as necessary
   */ 
  
  float prob = 0.1; 
  float delta = 0.5; 
  
  primary_replica.generateDataDrivenPartitioning(prob, delta); 

  graph_t g = primary_replica.partitioning_; 
  // now find connected comps
  std::vector<int> component(num_vertices(g));
  int numcomp = connected_components(g, &component[0]); 
  
  for(int attempt = 0; attempt < attemptcount; attempt++) {
    int comp_to_change = intrand(primary_replica.rng_, 0, numcomp); 
    std::cout << "Changing component " << comp_to_change << std::endl; 
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
    
    for(std::list<coords_t>::const_iterator ci = coordinates.begin(); 
	ci != coordinates.end(); ci++ ){
      coords_t c = *ci; 
      
      int pw1 = primary_replica.latentPhaseWraps_[c[0]][c[1]][c[2]]; 
      int pw2 = secondary_replica.latentPhaseWraps_[c[0]][c[1]][c[2]]; 
      primary_replica.latentPhaseWraps_[c[0]][c[1]][c[2]] = pw2; 
      secondary_replica.latentPhaseWraps_[c[0]][c[1]][c[2]] = pw1; 
      
    }
        
  }

}
