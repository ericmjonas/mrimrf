#include "pywrap.h"


BOOST_PYTHON_MODULE(pywrap)
{
  import_array();  

  pymulconv<double, 1>(); 
  
  pymulconv<double, 2>(); 
  pymulconv<double, 3>(); 
  pymulconv<unsigned char, 2>(); 
  pymulconv<unsigned char, 3>();
  pymulconv<unsigned char, 4>();


  
  def("identity2", identity<multi_array<double, 2> >); 
  def("identity3", identity<multi_array<double, 3> >); 

  def("identity4", identity<multi_array<unsigned char, 2 > >); 

}

