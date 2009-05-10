#ifndef __SW_H__
#define __SW_H__
/*
  Code for swendsen-wang-type moves

 */

#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "types.h"


using namespace boost; 

typedef boost::array<size_t, 3> coords_t;
 
struct graph_pixel_t
{
  coords_t coordinates; 
  char phasewrap; 

};


// create a typedef for the Graph type
typedef boost::adjacency_list<boost::vecS, boost::vecS,  
			      boost::undirectedS, 
			      graph_pixel_t
			      > graph_t;

graph_t wrap_cube_to_graph(const wrap_cube_t & pc);

void disconnect_nonsimilar_phase_edges(graph_t & g); 

void flip_edges_off(graph_t & g, rng_t & rng, float p);


#endif //
