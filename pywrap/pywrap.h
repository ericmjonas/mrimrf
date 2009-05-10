#ifndef PYWRAP_H
#define PYWRAP_H

#include <boost/python.hpp>
#include <boost/python/numeric.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/class.hpp>
#include <boost/python/str.hpp>
#include <boost/python/slice.hpp>
#include <iostream>

#include <boost/multi_array.hpp>
#include <numpy/arrayobject.h>


#define NO_IMPORT_ARRAY
#include "pywrap_test.h"

using namespace boost::python;
using namespace boost; 

template <typename T> 
PyArray_TYPES getEnum ()
{
  return PyArray_CHAR; 
}

template <>
PyArray_TYPES getEnum<char>()
{
   return PyArray_CHAR;
}

template <>
PyArray_TYPES getEnum<unsigned char>()
{
     return PyArray_UBYTE;
}

template <>
PyArray_TYPES getEnum<float>()
{
  return PyArray_FLOAT; 
}

template <>
PyArray_TYPES getEnum<double>()
{
  return PyArray_DOUBLE; 
}

template <>
PyArray_TYPES getEnum<short>()
{
  return PyArray_SHORT; 
}

template <>
PyArray_TYPES getEnum<unsigned short>()
{
  return PyArray_USHORT; 
}


template <>
PyArray_TYPES getEnum<size_t>()
{
     return PyArray_ULONGLONG;
}

template<typename T, int DIM>
struct multiarray_to_pyarray
{
  static PyObject* convert(multi_array<T, DIM> const & x)
  {
    // dynamic allocation
    int D = x.num_dimensions(); 
    std::vector<npy_intp> dims(D); 
    for (int i = 0; i < D; i++) {
      dims[i] = x.shape()[i]; 
    }
    
    PyObject* p = PyArray_SimpleNew(D, &dims[0], getEnum<T>()); 
    
    memcpy(((PyArrayObject*)p)->data, 
	   x.data(), x.num_elements()*sizeof(T)); 
    return p; 
  }
}; 


template<typename T, int DIM>
struct multiarray_from_pyarray
{
  multiarray_from_pyarray()
  {
    boost::python::converter::registry::push_back(
	 &convertible, 
	 &construct, 
	 boost::python::type_id<multi_array<T, DIM> >()); 

  }
  
  static void* convertible(PyObject* obj_ptr)
  {
    // not sure what this does
    PyArrayObject* p = (PyArrayObject*) obj_ptr; 
    if (p->descr->type_num != getEnum<T>()) {
      return 0; 
    }
    // check if the underlying object is convertable! 

    return obj_ptr; 
  }

  static void 
  construct(PyObject* obj_ptr, 
	    boost::python::converter::rvalue_from_python_stage1_data* data)
  {


    PyArrayObject* pao = (PyArrayObject*)obj_ptr; 
    
    void* storage = (
		     (boost::python::converter::rvalue_from_python_storage<multi_array<T, DIM> >*)
		     data)->storage.bytes;

    int Ndims = pao->nd; 
    std::vector<int> dims; 
    int numElements(1); 
    for (int i = 0; i < Ndims; i++)
      {
	dims.push_back(pao->dimensions[i]); 
	numElements *= pao->dimensions[i]; 
      }

    new (storage) multi_array<T, DIM> (dims); 
    void * dp = (void *) (((multi_array<T, DIM> *)storage)->data()); 
    memcpy(dp, pao->data, numElements*sizeof(T)); 

    data->convertible = storage;
    
  }
  
};

template<typename T, int DIM>
void pymulconv()
{
  boost::python::to_python_converter<multi_array<T, DIM>, 
    multiarray_to_pyarray<T, DIM> >();
  multiarray_from_pyarray<T, DIM> (); 
  

}

#endif // PYWRAP_H
