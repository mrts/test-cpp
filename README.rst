test-cpp: a lightweight C++ testing framework
============================================

``test-cpp`` is a simple yet reasonably comprehensive and extensible C++ testing
framework:

* simple: about 350 lines of code;

* comprehensive: supports test suites, fixtures (by test suite
  constructors and destructors) and exception assertions additionally to
  the standard ``assertTrue()`` and ``assertEqual()``;

* extensible: you can fully customize test event handling (e.g. reporting
  output to a GUI window) by implementing your own test observer; see
  `TextStreamTestView.h`_ for an example;
  
* cross-platform: standard C++03 and `CMake`_ for building.

``test-cpp`` is `licenced under the Boost licence`_.

**Status**: production/stable.

**Be sure to pull in submodules** with ``git clone --recursive`` (see below
under **Building**).

Building
--------

``test-cpp`` build system uses `CMake`_.

Build and test ``test-cpp`` as follows::

  git clone --recursive git://github.com/mrts/test-cpp.git

  cd test-cpp

  ./scripts/prepare-build.sh
  ./scripts/build.sh

Includes are in ``include`` and the library will be in ``lib``.

Add ``-I$(TESTCPPDIR)/include`` to include path and
``-L$(TESTCPPDIR)/lib -ltestcpp`` to linker flags in your
project's ``Makefile``.

Visual Studio integration
.........................

`CMake`_ will generate Visual Studio project files into ``lib`` that can be
added into Visual Studio solutions.

The best way to run the test suite in a Visual Studio solution is to add the
corresponding post-build event to the test-suite project (*Project →
Properties → Build Events → Post-Build Event → Command Line:
"$(TargetDir)$(TargetFileName)"*). The benefit of this approach is that you get
immediate red visual feedback on failure and can open failing tests by
double-clicking on the failure location in the error list window as follows:

.. image:: https://raw.githubusercontent.com/mrts/test-cpp/master/doc/error-list.png

Be sure to set the ``/FC`` (use full path for ``__FILE__``) compiler option for
the test suite project in this case (*Project → Properties → C/C++ →
Advanced → Use Full Paths: Yes (/FC)*). It is also helpful to always show the
error list window if there are any unit test errors (*Tools → Options →
Projects and Solutions → Always show Error List if build finishes with
errors*) and show a message box from main if there are no errors::

  int result = c.run();
  if (result == 0)
      MessageBox(NULL, L"All tests passed", L"Test OK",
                 MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL);
  return result;

The solution file from the `win32-asyncconnect project`_ can be used as the
example for integrating ``test-cpp`` to solutions. It also contains a `test
runner`_.

Alternatively, for another nice colorful change-compile-test experience, use
``Test::ColoredStdOutView`` in the test runner and set the test runner as
startup project so that the colored console application will be run when you
press ``CTRL-F5`` (``CTRL-F5`` keeps the console open).

Adding as a git submodule
-------------------------

Add ``test-cpp`` to your ``git`` repository as a submodule as follows::

  git submodule add git://github.com/mrts/test-cpp.git test/lib/testcpp
  git submodule update --init --recursive

Usage
-----

This input::

  #include <testcpp/testcpp.h>

  class TestSuite1 : public Test::Suite
  {
  public:
      TESTCPP_TYPEDEF_TESTMETHOD(TestSuite1)

      void testException()
      { throw std::logic_error("exception message"); }

      void testNoException()
      { int a = 1; ++a; }

      void test()
      {
          assertTrue("1 + 1 == 2 is true", 1 + 1 == 2);
          assertTrue(1 + 1 == 2);

          assertTrue("1 + 1 == 3 is true (fails)", 1 + 1 == 3);

          assertEqual("1 + 1 equals 2", 1 + 1, 2);
          assertEqual(1 + 1, 2);

          assertEqual("1 + 1 equals 3 (fails)", 1 + 1, 3);

          assertThrows("throws logic_error",
                  TestSuite1, TestMethod, std::logic_error,
                  *this, &TestSuite1::testException);
          assertThrows(TestSuite1, TestMethod, std::logic_error,
                  *this, &TestSuite1::testException);

          assertThrows("doesn't throw logic_error (fails)",
                  TestSuite1, TestMethod, std::logic_error,
                  *this, &TestSuite1::testNoException);

          assertWontThrow("doesn't throw any exceptions",
                  TestSuite1, TestMethod,
                  *this, &TestSuite1::testNoException);
          assertWontThrow(TestSuite1, TestMethod,
                  *this, &TestSuite1::testNoException);

          assertWontThrow("throws logic_error",
                  TestSuite1, TestMethod,
                  *this, &TestSuite1::testException);

          testException(); // unhandled exception is caught,
                           // but stops the testsuite
          assertTrue("not reached because of the previous exception", true);
      }
  };

  int main()
  {
      Test::Controller &c = Test::Controller::instance();
      c.addTestSuite("testsuite1", Test::Suite::instance<TestSuite1>);

      return c.run();
  }

results in the following output::

  Start running 1 test suites
  Test suite 'testsuite1' (#1/1):
    test '1 + 1 == 2 is true': ... OK
    test '1 + 1 == 2': ... OK
    test '1 + 1 == 3 is true (fails)': ... FAIL
      c:\path\to\test-cpp\test\src\main.cpp(78): error: assertTrue failed in TestSuite1::test
    test '1 + 1 equals 2': ... OK
    test '1 + 1 == 2': ... OK
    test '1 + 1 equals 3 (fails)': ... FAIL
      c:\path\to\test-cpp\test\src\main.cpp(83): error: assertEqual failed in TestSuite1::test
    test 'throws logic_error': ... OK
      (message: 'exception message')
    test '&TestSuite1::testException throws std::logic_error': ... OK
      (message: 'exception message')
    test 'doesn't throw logic_error (fails)': ... FAIL
      c:\path\to\test-cpp\test\src\main.cpp(93): error: assertThrows failed in TestSuite1::test
    test 'doesn't throw any exceptions': ... OK
    test '&TestSuite1::testNoException won't throw': ... OK
    test 'throws logic_error': ... FAIL: unexpected exception 'class std::logic_error'
      (message: 'exception message')
      c:\path\to\test-cpp\test\src\main.cpp(103): error: assertWontThrow failed in TestSuite1::test
    ---
    Unhandled exception 'class std::logic_error'
      (message: 'exception message')
    Test suite FAIL due to exception with 4 non-exception errors
  Did run 1 of 1 total test suites, # of errors: 4, # of uncaught exceptions: 1
  Test run result: FAIL

See `main test`_ or `ioc-cpp tests`_ for more details.

Colored output
..............

Use the following for colored output::

  #include <testcpp/StdOutView.h>
  Test::Controller &c = Test::Controller::instance();
  c.setObserver(new Test::ColoredStdOutView);

.. _CMake: http://www.cmake.org/
.. _`ioc-cpp tests`: https://github.com/mrts/ioc-cpp/blob/master/test/src/main.cpp
.. _`licenced under the Boost licence`: https://github.com/mrts/test-cpp/blob/master/LICENCE.rst
.. _`main test`: https://github.com/mrts/test-cpp/blob/master/test/src/main.cpp
.. _`test runner`: https://github.com/mrts/win32-asyncconnect/blob/master/test/Runner/src/TestRunner.cpp
.. _TextStreamTestView.h: https://github.com/mrts/test-cpp/blob/master/include/testcpp/detail/TextStreamTestView.h
.. _`win32-asyncconnect project`: https://github.com/mrts/win32-asyncconnect
