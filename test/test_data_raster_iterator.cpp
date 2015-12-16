#include <gdal.h>
#include <vector>
#include "test_data_raster_iterator.h"
#include "RasterDims.h"
#include "DataRaster.h"
#include "DataRasterIterator.h"
#include "DataBuffer.h"

CPPUNIT_TEST_SUITE_REGISTRATION (test_data_raster_iterator);

void test_data_raster_iterator::setUp (void)
{}

void test_data_raster_iterator::tearDown (void)
{}

void test_data_raster_iterator::runTest1(void) 
{
  try
  {
    DataRaster dr;
    dr.open(std::string("ms_chip"), GA_ReadOnly);
    
    int memsize = static_cast<int>(0.1 * 1024. * 1024.);
    DataRasterIterator iter(dr, memsize, 0);
    
    //test accessors
    if (iter.ntiles()  != 4 ||
        iter.overlap() != 0)
      CPPUNIT_FAIL("Parameters do not match"); 
    
    for (int tilenum=0; tilenum<iter.ntiles(); tilenum++)
    {
      RasterDims inputdims, outputdims;
      iter.getTileDims(tilenum, inputdims, &outputdims);
      if (inputdims == outputdims)
      {
        std::cout << "Tile number: " << tilenum << std::endl;
        inputdims.print();
      }
      else
      {
        CPPUNIT_FAIL("input and output tile dimensions do not match."); 
      }
      
      
    }
    
  }
  catch (std::exception& e)
  {
    std::ostringstream ostr;
    ostr << "*** Exception thrown in test_data_raster_iterator::runTest1: " << e.what();
    CPPUNIT_FAIL(ostr.str().c_str());
  }
  
  std::cout << std::endl << "test_data_raster_iterator::runTest1 completed successfully" << std::endl << std::endl;
}

