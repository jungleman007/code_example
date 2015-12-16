#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include <iostream>

int main (int argc, char* argv[])
{
    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // create test runner
    CPPUNIT_NS::TestRunner testrunner;

    // get registered tests
    CPPUNIT_NS::Test* pAllTests = CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();

    // handle command line arguments
    if (argc > 1)
    {
      // add arguments to a set for fast lookup
      std::set<std::string> args;
      for (int i = 1; i < argc; ++i)
      {
        args.insert(std::string(argv[i]));
      }

      // get test list
      CPPUNIT_NS::TestSuite* pSuite = dynamic_cast<CPPUNIT_NS::TestSuite*>(pAllTests);
      const std::vector<CPPUNIT_NS::Test*> tests = pSuite->getTests();

      // check for -h or --help
      if ((args.count("-h") > 0) || (args.count("--help") > 0))
      {
        // print usage and exit
        std::cout << "USAGE: " << argv[0] << " [options] [specific test]" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "  -h or --help: Print usage and exit" << std::endl;
        std::cout << "  -l or --list: List tests and exit" << std::endl;
        return 0;
      }

      // check for -l or --list
      if ((args.count("-l") > 0) || (args.count("--list") > 0))
      {
        // list tests and exit
        std::cout << "TEST LIST:" << std::endl;
        std::vector<CPPUNIT_NS::Test*>::const_iterator testIter = tests.begin();
        for (; testIter != tests.end(); ++testIter)
        {
          CPPUNIT_NS::Test* pTest = *testIter;
          std::cout << pTest->getName() << std::endl;
        }
        return 0;
      }

      // add named tests
      std::vector<CPPUNIT_NS::Test*>::const_iterator testIter = tests.begin();
      for (; testIter != tests.end(); ++testIter)
      {
        CPPUNIT_NS::Test* pTest = *testIter;
        if (args.count(pTest->getName()) > 0)
        {
          std::cout << "ADDING TEST: " << pTest->getName() << std::endl;
          testrunner.addTest(pTest);
        }
      }
    }
    else
    {
      // no test names passed, run all tests
      std::cout << "ADDING ALL TESTS" << std::endl;
      testrunner.addTest (pAllTests);
    }

    // run the added tests
    testrunner.run (testresult);

    // output results in compiler-format
    CPPUNIT_NS::CompilerOutputter compileroutputter (&collectedresults, std::cerr);
    compileroutputter.write ();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful () ? 0 : 1;
}
