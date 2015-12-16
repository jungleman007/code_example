#include "test_data_buffer.h"
#include "DataBuffer.h"

CPPUNIT_TEST_SUITE_REGISTRATION (test_data_buffer);

void test_data_buffer::setUp (void)
{}

void test_data_buffer::tearDown (void) {}

void test_data_buffer::runTest1(void) 
{
	try
	{
		RasterDims rd(15, 20, 20, 30);
		DataBuffer<unsigned char> db(rd, 1, true);
		
		if (db.width()  != 6  ||
				db.height() != 11 ||
				db.nbands() != 1)
			CPPUNIT_FAIL("Parameters do not match");		
	}
	catch (std::exception& e)
	{
		std::ostringstream ostr;
		ostr << "*** Exception thrown in runTest1: " << e.what();
		CPPUNIT_FAIL(ostr.str().c_str());
	}
	
	std::cout << std::endl << "test_data_buffer::runTest1 completed successfully" << std::endl << std::endl;
}

void test_data_buffer::runTest2(void) 
{
	try
	{
		int start = 0, end = 10;
		RasterDims rd(start, end, start, end);
		DataBuffer<unsigned char> db(rd, 1, true);
		
		for (int row=start; row<=end; row++)
		{
			for (int col=start; col<=end; col++)
			{
				if (db[col+row*db.width()] != 0)
				{
					std::ostringstream ostr;
					ostr << "Pixel at coordinate " << row << "," << col << " is not zero." << std::endl;
					CPPUNIT_FAIL(ostr.str().c_str());
				}
			}
		}
	}
	catch (std::exception& e)
	{
		std::ostringstream ostr;
		ostr << "*** Exception thrown in runTest2: " << e.what();
		CPPUNIT_FAIL(ostr.str().c_str());
	}
	
	std::cout << std::endl << "test_data_buffer::runTest2 completed successfully" << std::endl << std::endl;
}

void test_data_buffer::runTest3(void) 
{
	try
	{
		int start = 0, end = 10;
		RasterDims rd(start, end, start, end);
		int nbands = 1;
		DataBuffer<unsigned char> db(rd, nbands, true);
		
		unsigned char* data = db.data();
		unsigned char val = 255;
		memset((void*)data, 255, sizeof(unsigned char) * db.width() * db.height() * nbands);
		
		for (int row=start; row<=end; row++)
		{
			for (int col=start; col<=end; col++)
			{
				if (db[col+row*db.width()] != val)
				{
					std::ostringstream ostr;
					ostr << "Pixel at coordinate " << row << "," << col << " is not correct" << std::endl;
					CPPUNIT_FAIL(ostr.str().c_str());
				}
			}
		}
	}
	catch (std::exception& e)
	{
		std::ostringstream ostr;
		ostr << "*** Exception thrown in runTest3: " << e.what();
		CPPUNIT_FAIL(ostr.str().c_str());
	}
	
	std::cout << std::endl << "test_data_buffer::runTest3 completed successfully" << std::endl << std::endl;
}



