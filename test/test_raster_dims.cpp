#include "test_raster_dims.h"
#include "RasterDims.h"

CPPUNIT_TEST_SUITE_REGISTRATION (test_raster_dims);

void test_raster_dims::setUp (void)
{}

void test_raster_dims::tearDown (void)
{}
void test_raster_dims::runTest1(void) 
{
	try
	{
		RasterDims rd;
		rd.setStartSample(15);
		rd.setEndSample(20);
		rd.setStartLine(20);
		rd.setEndLine(30);
		
		if (rd.startSample() != 15 ||
				rd.endSample() 	 != 20 ||
				rd.startLine()   != 20 ||
				rd.endLine()     != 30)
			CPPUNIT_FAIL("test_raster_dims:runTest1 Parameters do not match");		
	}
	catch (std::exception& e)
	{
		std::ostringstream ostr;
		ostr << "*** Exception thrown in runTest: " << e.what();
		CPPUNIT_FAIL(ostr.str().c_str());
	}
	
	std::cout << std::endl << "test_raster_dims::runTest1 completed successfully" << std::endl << std::endl;
}

void test_raster_dims::runTest2(void) 
{
	try
	{
		RasterDims rd(15, 20, 20, 30);
		
		if (rd.startSample() != 15 ||
				rd.endSample() 	 != 20 ||
				rd.startLine()   != 20 ||
				rd.endLine()     != 30)
			CPPUNIT_FAIL("test_raster_dims:runTest2 Parameters do not match");		
	}
	catch (std::exception& e)
	{
		std::ostringstream ostr;
		ostr << "*** Exception thrown in runTest: " << e.what();
		CPPUNIT_FAIL(ostr.str().c_str());
	}
	
	std::cout << std::endl << "test_raster_dims::runTest2 completed successfully" << std::endl << std::endl;
}

void test_raster_dims::runTest3(void) 
{
	try
	{
		RasterDims rd(15, 20, 20, 30);
		RasterDims rd2(rd);
		
		if (rd2.startSample() != 15 ||
				rd2.endSample() 	!= 20 ||
				rd2.startLine()   != 20 ||
				rd2.endLine()     != 30)
			CPPUNIT_FAIL("test_raster_dims:runTest3 Parameters do not match");		
	}
	catch (std::exception& e)
	{
		std::ostringstream ostr;
		ostr << "*** Exception thrown in runTest: " << e.what();
		CPPUNIT_FAIL(ostr.str().c_str());
	}
	
	std::cout << std::endl << "test_raster_dims::runTest3 completed successfully" << std::endl << std::endl;
}

void test_raster_dims::runTest4(void) 
{
	try
	{
		RasterDims rd(15, 20, 20, 30);
		rd.print();
	}
	catch (std::exception& e)
	{
		std::ostringstream ostr;
		ostr << "*** Exception thrown in test_raster_dims:runTest4: " << e.what();
		CPPUNIT_FAIL(ostr.str().c_str());
	}
	
	std::cout << std::endl << "test_raster_dims::runTest4 completed successfully" << std::endl << std::endl;
}


