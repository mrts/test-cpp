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
  `TextStreamTestView.h`_ for an example.

``test-cpp`` is `licenced under the Boost licence`_.

**Status**: production/stable.

**Be sure to pull in submodules** with ``git clone --recursive`` (see below
under **Building**).

Building
--------

Build and test ``test-cpp`` as follows (``clang++`` is the default compiler)::

  git clone --recursive git://github.com/mrts/test-cpp.git

  cd test-cpp

  make
  make test

Includes are in ``include`` and the library will be in ``lib``.

Add ``-I$(TESTCPPDIR)/include`` to include path and
``-L$(TESTCPPDIR)/lib -ltestcpp`` to linker flags in your
project's ``Makefile``.

Usage
-----

In C++03 mode, this input::

  class TestSuite1 : public Test::Suite
  {
  public:
      typedef void (TestSuite1::*TestMethod)();

      void testException()
      { throw std::logic_error("exception message"); }

      void testNoException()
      { int a = 1; ++a; }

      void test()
      {
          Test::assertTrue("1 + 1 == 2 is true",
                  1 + 1 == 2);

          Test::assertTrue("1 + 1 == 3 is true",
                  1 + 1 == 3); // fails

          Test::assertEqual<int>("1 + 1 equals 2",
                  1 + 1, 2);

          Test::assertEqual<int>("1 + 1 equals 3",
                  1 + 1, 3); // fails

          Test::assertThrows<TestSuite1, TestMethod, std::logic_error>
                  ("throws logic_error",
                          *this, &TestSuite1::testException);

          Test::assertThrows<TestSuite1, TestMethod, std::logic_error>
                  ("doesn't throw logic_error",
                          *this, &TestSuite1::testNoException); // fails

          Test::assertWontThrow<TestSuite1, TestMethod>
                  ("doesn't throw any exceptions",
                          *this, &TestSuite1::testNoException);

          Test::assertWontThrow<TestSuite1, TestMethod>
                  ("throws logic_error",
                          *this, &TestSuite1::testException); // fails

          testException(); // unhandled exception is caught, but stops the testsuite
          Test::assertTrue("not reached because of the previous exception", true);
      }
  };

  int main()
  {
      Test::Controller &c = Test::Controller::instance();
      c.addTestSuite("testsuite1", Test::Suite::instance<TestSuite1>);

      return c.run();
  }

results in the following output::

  Test suite 'testsuite1' (#1/1):
    test '1 + 1 == 2 is true': ... OK
    test '1 + 1 == 3 is true': ... FAIL
    test '1 + 1 equals 2': ... OK
    test '1 + 1 equals 3': ... FAIL
    test exception 'throws logic_error': ... OK
    test exception 'doesn't throw logic_error': ... FAIL
    test no exception 'doesn't throw any exceptions': ... OK
    test no exception 'throws logic_error': ... FAIL: unexpected exception St11logic_error: exception message
    Unhandled exception 'St11logic_error' with message: exception message
    FAIL due to exception with 4 non-exception errors
  Total test suites run: 1, # of errors: 4, # of uncaught exceptions: 1

Use lambdas in C++11 mode instead with exception-related asserts.

See `main test`_ for more details.

.. _`licenced under the Boost licence`: https://github.com/mrts/test-cpp/blob/master/LICENCE.rst
.. _`main test`: https://github.com/mrts/test-cpp/blob/master/test/src/main.cpp
.. _TextStreamTestView.h: https://github.com/mrts/test-cpp/blob/master/include/testcpp/detail/TextStreamTestView.h
