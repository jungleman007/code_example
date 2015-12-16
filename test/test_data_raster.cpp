#include <gdal.h>
#include <vector>
#include "test_data_raster.h"
#include "RasterDims.h"
#include "DataRaster.h"
#include "DataBuffer.h"

CPPUNIT_TEST_SUITE_REGISTRATION (test_data_raster);

void test_data_raster::setUp (void)
{}

void test_data_raster::tearDown (void)
{}

void test_data_raster::runTest1(void) 
{
	try
	{
		DataRaster dr;
	}
	catch (std::exception& e)
	{
		std::ostringstream ostr;
		ostr << "*** Exception thrown in test_data_raster::runTest1: " << e.what();
		CPPUNIT_FAIL(ostr.str().c_str());
	}
	
	std::cout << std::endl << "test_data_raster::runTest1 completed successfully" << std::endl << std::endl;
}

template <typename T> void test_data_raster::computeMean(DataBuffer<T>& buf, std::vector<double>& meanvals)
{
  for (int band=0; band<buf.nbands(); band++)
  {
    double sum = 0.0;
    int sz = buf.dims().width() * buf.dims().height();
    unsigned short* bandptr = buf.data() + band * sz;
    for (int idx=0; idx<sz; idx++)
      sum += bandptr[idx];
    meanvals.push_back(sum / static_cast<double>(sz));
  }
}
void test_data_raster::runTest2(void) 
{
	try
	{
		DataRaster dr;
    dr.open(std::string("ms_chip"), GA_ReadOnly);
    
    //test accessors
    if (dr.nsamples() != 400 ||
        dr.nlines()   != 400 ||
        dr.nbands()   != 4   || 
        dr.dataType() != GDT_UInt16)
      CPPUNIT_FAIL("Parameters do not match");		
      
    double ulx, uly, lrx, lry;
    dr.getBounds(ulx, uly, lrx, lry);
    std::cout << "DataRaster Bounds are: " << ulx << " " << uly << " " << lrx << " " << lry << std::endl;
    
    //test accessor for dims object
    if (dr.dims().width()  != 400 ||
        dr.dims().height() != 400)
      CPPUNIT_FAIL("Parameters do not match");		
      
    //Allocate a buffer to hold the imagery data
    DataBuffer<unsigned short> databuffer(dr.dims(), dr.nbands());
    //read the data into each band
    for (int band=0; band<dr.nbands(); band++)
      dr.getData(databuffer, band+1, dr.dataType(), band);
      
    //compute the mean for each band
    std::vector<double> meanvals;
    computeMean<unsigned short>(databuffer, meanvals);
    
    std::cout << "Mean Values are: ";
    for (int band=0; band<dr.nbands(); band++)
      std::cout << "Band " << band << " :" << meanvals[band] << " ";
    std::cout << std::endl;
    
    double tol = 1e-6;
    if (meanvals[0] - 276.8724125 > tol  ||
        meanvals[1] - 364.2011437 > tol  ||
        meanvals[2] - 323.0707875 > tol  ||
        meanvals[3] - 518.7096562 > tol)
      CPPUNIT_FAIL("test_data_raster::runTest2: mean values do not agree with truth data");
	}
	catch (std::exception& e)
	{
		std::ostringstream ostr;
		ostr << "*** Exception thrown in test_data_raster::runTest2: " << e.what();
		CPPUNIT_FAIL(ostr.str().c_str());
	}
	
	std::cout << std::endl << "test_data_raster::runTest2 completed successfully" << std::endl << std::endl;
}

void test_data_raster::runTest3(void) 
{
	try
	{
		DataRaster drmschip;
    drmschip.open(std::string("ms_chip"), GA_ReadOnly);
    
    //Allocate a buffer to hold the mschip imagery data
    DataBuffer<unsigned short> drmschipdatabuffer(drmschip.dims(), drmschip.nbands());
    for (int band=0; band<drmschip.nbands(); band++)
      drmschip.getData(drmschipdatabuffer, band+1, drmschip.dataType(), band);
    
    DataRaster newraster;
    newraster.create("output.tif", drmschip.dims(), drmschip.nbands(), GDT_UInt16, "GTiff", &drmschip);
    
    //set the output data.
    for (int band=0; band<newraster.nbands(); band++)
      newraster.setData(drmschipdatabuffer, drmschipdatabuffer.dims(), band+1, newraster.dataType(), band);
    newraster.close();  //this flushes data to the file
    
    //reopen the dataset
    newraster.open(std::string("output.tif"), GA_ReadOnly);
    
    //check the dimensions
    if (newraster.nsamples() != drmschip.nsamples() ||
        newraster.nlines()   != drmschip.nlines()   ||
        newraster.nbands()   != drmschip.nbands())
      CPPUNIT_FAIL("test_data_raster::runTest3: Dimensions of new file do not match truth data");
        
    //read the data from the new file
    DataBuffer<unsigned short> newrasterdatabuffer(newraster.dims(), newraster.nbands());
    for (int band=0; band<newraster.nbands(); band++)
      newraster.getData(newrasterdatabuffer, band+1, newraster.dataType(), band);
      
    //compare the datasets
    for (int band=0; band<newraster.nbands(); band++)
    {
      int sz = newrasterdatabuffer.dims().width() * newrasterdatabuffer.dims().height();
      unsigned short* ptr1 = drmschipdatabuffer.data() + band * sz;
      unsigned short* ptr2 = newrasterdatabuffer.data() + band * sz;
      for (int idx=0; idx<sz; idx++)
      {
        if (ptr1[idx] != ptr2[idx])
          CPPUNIT_FAIL("test_data_raster::runTest3: The data files contain differences");
      }
    }
      
	}
	catch (std::exception& e)
	{
		std::ostringstream ostr;
		ostr << "*** Exception thrown in test_data_raster::runTest3: " << e.what();
		CPPUNIT_FAIL(ostr.str().c_str());
	}
	
	std::cout << std::endl << "test_data_raster::runTest3 completed successfully" << std::endl << std::endl;
}

