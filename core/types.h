#ifndef __TYPES_H__
#define __TYPES_H__

#include "boost/multi_array.hpp"
#include <cassert>
#include <boost/random.hpp>


typedef boost::multi_array<char, 3> wrap_cube_t;
typedef boost::multi_array<size_t, 3> coloring_cube_t;
typedef boost::multi_array<float, 3> phase_cube_t;
typedef boost::multi_array<float, 4> observation_tensor_t; 


typedef std::vector<double> probvector_t; 

typedef boost::rand48 rng_t;

#endif // __TYPES_H__
