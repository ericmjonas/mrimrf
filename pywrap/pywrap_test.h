#ifndef PYWRAP_TEST_H
#define PYWRAP_TEST_H

#include <boost/python.hpp>
#include <boost/python/numeric.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/class.hpp>
#include <boost/python/str.hpp>
#include <boost/python/slice.hpp>
#include <iostream>

#include "boost/multi_array.hpp"
#include <numpy/arrayobject.h>


#define NO_IMPORT_ARRAY

typedef unsigned char latent_t ; 
typedef boost::multi_array<latent_t, 2> LatentVars_t; 
typedef boost::multi_array<double, 3> ObsEnergy_t; 
typedef boost::function<float (latent_t x, latent_t y)> pairEnergyFunc_t; 
typedef std::vector<double> probvect_t; 

template<typename T>
T identity(T const & test)
{
  return test; 
};


#endif //PYWRAP_TEST_H
