#ifndef _TESTDATABUFFERH_
#define _TESTDATABUFFERH_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class test_data_buffer : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE (test_data_buffer);
  CPPUNIT_TEST (runTest1);
  CPPUNIT_TEST (runTest2);
  CPPUNIT_TEST (runTest3);
  CPPUNIT_TEST_SUITE_END ();

public:

  void setUp (void);
  void tearDown (void);

protected:

  void runTest1(void);
  void runTest2(void);
  void runTest3(void);

private:

};
#endif
