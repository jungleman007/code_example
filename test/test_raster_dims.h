#ifndef _TESTRASTERDIMSH_
#define _TESTRASTERDIMSH_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class test_raster_dims : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE (test_raster_dims);
	CPPUNIT_TEST (runTest1);
	CPPUNIT_TEST (runTest2);
	CPPUNIT_TEST (runTest3);
	CPPUNIT_TEST (runTest4);
  CPPUNIT_TEST_SUITE_END ();

public:

  void setUp (void);
  void tearDown (void);

protected:

  void runTest1(void);
	void runTest2(void);
	void runTest3(void);
	void runTest4(void);

private:

};
#endif
