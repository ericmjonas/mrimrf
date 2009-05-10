
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "sw.h"

BOOST_AUTO_TEST_SUITE(sw_test); 

BOOST_AUTO_TEST_CASE(component_gibbs_test)
{
  using namespace boost; 
  wrap_cube_t cube(boost::extents[3][4][2]); 
  cube[2][1][1] = 10; 
  
  graph_t g = wrap_cube_to_graph(cube); 
  typedef property_map<graph_t, vertex_index_t>::type IndexMap;
  IndexMap index = get(vertex_index, g);
  
  BOOST_CHECK_EQUAL(num_vertices(g), 3 * 4 * 2); 
  BOOST_CHECK_EQUAL(num_edges(g), (4*2 + 3*3) * 2 + 12);

//   std::cout << "vertices(g) = ";
//   typedef graph_traits<graph_t>::vertex_iterator vertex_iter;
//   std::pair<vertex_iter, vertex_iter> vp;
//   for (vp = vertices(g); vp.first != vp.second; ++vp.first)
//     std::cout << index[*vp.first] <<  " ";
//   std::cout << std::endl;
  
}


BOOST_AUTO_TEST_CASE(remove_unsimilar_gibbs_test)
{
  using namespace boost; 
  wrap_cube_t cube(boost::extents[3][4][2]); 
  char pos(0); 
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 2; k++) {
	cube[i][j][k]=pos; 
	pos++;
      }
    }
  }
  
  cube[1][1][1] = cube[1][2][1];
  
  graph_t g = wrap_cube_to_graph(cube); 
  BOOST_CHECK_EQUAL(num_vertices(g), 3 * 4 * 2); 
  BOOST_CHECK_EQUAL(num_edges(g), (4*2 + 3*3) * 2 + 12);
  
  disconnect_nonsimilar_phase_edges(g); 
  BOOST_CHECK_EQUAL(num_edges(g), 1);
  
}


BOOST_AUTO_TEST_SUITE_END(); 
