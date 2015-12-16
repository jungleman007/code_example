#include <gdal.h>
#include <vector>
#include "test_ndvi.h"

CPPUNIT_TEST_SUITE_REGISTRATION (test_ndvi);

void test_ndvi::setUp (void)
{}

void test_ndvi::tearDown (void)
{}

void test_ndvi::runTest1(void) 
{
  try
  {
    Ndvi ndvicalc(std::string("ms_chip"), std::string("ndvi_output.tif"));
		ndvicalc.run();
  }
  catch (std::exception& e)
  {
    std::ostringstream ostr;
    ostr << "*** Exception thrown in test_ndvi::runTest1: " << e.what();
    CPPUNIT_FAIL(ostr.str().c_str());
  }
  
  std::cout << std::endl << "test_ndvi::runTest1 completed successfully" << std::endl << std::endl;
}



