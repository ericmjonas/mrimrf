#include "sw.h"
#include "util.h"
#include <boost/graph/connected_components.hpp>
graph_t wrap_cube_to_graph(const wrap_cube_t & pc)
{
  /*
    turn our latent phase cube into a graph
    
   */
  
  graph_t g(pc.num_elements());
  typedef graph_traits<graph_t>::vertex_iterator virtex_iter; 
  std::pair<virtex_iter, virtex_iter> vp; 
  vp = vertices(g); 

  // fully connect
  boost::multi_array<virtex_iter, 3> 
    virtex_pos_map(boost::extents[pc.shape()[0]][pc.shape()[1]][pc.shape()[2]]); 
  
  for(int i = 0; i < pc.shape()[0]; i++) {
    for(int j = 0; j < pc.shape()[1]; j++) {
      for(int k = 0; k < pc.shape()[2]; k++) {

	coords_t c; 
	c[0] = i; 
	c[1] = j;
	c[2] = k;
	g[*vp.first].coordinates = c; 
	g[*vp.first].phasewrap = pc[i][j][k];
	virtex_pos_map[i][j][k] = *vp.first; 
	vp.first++; 
	
      }
    }
  }

  // now add the edges!
  for(int i = 0; i < pc.shape()[0]; i++) {
    for(int j = 0; j < pc.shape()[1]; j++) {
      for(int k = 0; k < pc.shape()[2]; k++) {
	

	if (i < pc.shape()[0]-1 )  {
	  add_edge(*virtex_pos_map[i][j][k], *virtex_pos_map[i+1][j][k], g); 
	}
	
	if (j < pc.shape()[1]-1 )  {
	  add_edge(*virtex_pos_map[i][j][k], *virtex_pos_map[i][j+1][k], g); 
	}
	
	if (k < pc.shape()[2]-1 )  {
	  add_edge(*virtex_pos_map[i][j][k], *virtex_pos_map[i][j][k+1], g); 
	}
	
      }
    }
  }

  return g; 
}

void disconnect_nonsimilar_phase_edges(graph_t & g)
{
  /*
    Delete all edges in the graph that do not 
    connect similarly-colored components
 
   */
  
  typedef  graph_traits<graph_t>::edge_iterator ei_t; 
  std::list<graph_t::edge_descriptor> edges_to_remove; 
  ei_t ei, ei_end;
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
    if( g[source(*ei, g)].phasewrap  != g[target(*ei, g)].phasewrap) {
      edges_to_remove.push_back(*ei); 
    }
  }
  
  for(std::list<graph_t::edge_descriptor >::iterator i = edges_to_remove.begin(); 
      i  != edges_to_remove.end(); i++) {
    remove_edge(*i, g);     
  }
  
}

void flip_edges_off(graph_t & g, rng_t & rng, float p)
{
  /* flip each remaining edge off with probability p 
  */ 

  typedef  graph_traits<graph_t>::edge_iterator ei_t; 
  std::list<graph_t::edge_descriptor> edges_to_remove; 
  ei_t ei, ei_end;
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
    if (p < unirand(rng)) {
      edges_to_remove.push_back(*ei); 
    }
  }
  
  for(std::list<graph_t::edge_descriptor >::iterator i = edges_to_remove.begin(); 
      i  != edges_to_remove.end(); i++) {
    remove_edge(*i, g);     
  }

}

coloring_cube_t graph_to_coloring_cube(graph_t & g, coords_t dims)
{
  // returns the coloring cube of a graph showing the connected 
  // components
  std::vector<int> component(num_vertices(g));
  coloring_cube_t colorcube(dims); 
  
  connected_components(g, &component[0]); 
  
  typedef graph_t::vertex_iterator vi_t; 
  std::pair<vi_t, vi_t> vp = vertices(g); 
  int pos(0); 
  for(vi_t v = vp.first; v != vp.second; ++v) {
    int i = g[*v].coordinates[0]; 
    int j = g[*v].coordinates[1]; 
    int k = g[*v].coordinates[2]; 

    colorcube[i][j][k] = component[pos];
    pos++; 

  }
  
  return colorcube; 

}

void swendsen_wang(wrap_cube_t & wrapcube, rng_t & rng, int minval, int maxval)
{

  graph_t g = wrap_cube_to_graph(wrapcube); 
  
  disconnect_nonsimilar_phase_edges(g); 
  
  flip_edges_off(g, rng, 0.3); 
  
  // randomly get a component
  
}
