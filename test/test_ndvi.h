#ifndef _TESTNDVIH_
#define _TESTNDVIH_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <vector>
#include "Ndvi.h"

class test_ndvi : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE (test_ndvi);
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
