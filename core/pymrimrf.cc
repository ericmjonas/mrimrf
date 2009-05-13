#include <iostream>
#include <pywrap.h>
#include <boost/python.hpp>

#include "types.h"
#include "mrimrf.h"
#include "partialreplica.h"

using namespace boost::python; 
void hello(std::string msg)
{
  std::cout << "Hello world: " <<  msg << std::endl; 

}

void testarray(const phase_cube_t & pc)
{
  std::cout << " test " << std::endl;
  std::cout << pc.shape()[0] 
	    << " " << pc.shape()[1] 
	    << " " << pc.shape()[2] << std::endl;
}

BOOST_PYTHON_MODULE(pymrimrf)
{
  import_array();  

  pymulconv<float, 3>(); 
  pymulconv<float, 4>();
 
  pymulconv<short, 3>(); 
  pymulconv<size_t, 3>(); 

  def("hello", &hello); 
  def("testarray", &testarray); 
  def("partial_replica_exchange_move", partial_replica_exchange_move);

  class_<MRIMRF, boost::noncopyable>("MRIMRF", init<int, phase_cube_t>())
    .def("sequential_gibbs_scan", &MRIMRF::sequential_gibbs_scan)
    .add_property("latentPhase", &MRIMRF::getLatentPhases)
    .add_property("latentPhaseWraps", &MRIMRF::getLatentPhaseWraps, 
		  &MRIMRF::setLatentPhaseWraps)  
    .add_property("temp", &MRIMRF::getTemp, &MRIMRF::setTemp)
    .def("random_gibbs_scan", &MRIMRF::random_gibbs_scan)
    .def("setSeed", &MRIMRF::setSeed)
    .add_property("score", &MRIMRF::getLogScore)
    .def("getColoring", &MRIMRF::getColoring)
    .def("swendsenWangMove", &MRIMRF::swendsenWangMove)
    .def("ddmcmc_flip_gibbs", &MRIMRF::ddmcmc_flip_gibbs)
    .add_property("currentPartitioning", &MRIMRF::getCurrentPartitioning)
    ;

//   class_<GibbsImageMRF>("GibbsImageMRF", init<const ObsEnergy_t & >())
//     .def("linearSweep", &GibbsImageMRF::linearSweep)
//     .def("randPermuteSweep", &GibbsImageMRF::randPermuteSweep)
//     .def("checkPoint", &GibbsImageMRF::checkpoint)
//     .def("setTemp", &GibbsImageMRF::setTemp)
//     .def("setFunction", &GibbsImageMRF::setFunction)
//     .def("getLogScore", &GibbsImageMRF::getLogScore)
//     .def("getLatentState", &GibbsImageMRF::getLatentState)
//     .def("getLatentStatePoint", &GibbsImageMRF::getLatentStatePoint)
//     .def("setLatentStatePoint", &GibbsImageMRF::setLatentStatePoint)
//     .def("computeLogScoreAtPoint", &GibbsImageMRF::computeLogScoreAtPoint)
//     .def("saveObsEnergy", &GibbsImageMRF::saveObsEnergy); 


}

