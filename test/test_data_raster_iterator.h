#ifndef _TESTDATARASTERITERATORH_
#define _TESTDATARASTERITERATORH_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <vector>
#include "DataBuffer.h"

class test_data_raster_iterator : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE (test_data_raster_iterator);
  CPPUNIT_TEST (runTest1);
  CPPUNIT_TEST_SUITE_END ();

public:

  void setUp (void);
  void tearDown (void);

protected:

  void runTest1(void);

private:

};
#endif
