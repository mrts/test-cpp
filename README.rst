testcpp: a lightweight C++ testing framework
============================================

``testcpp`` is a simple yet reasonably comprehensive and extensible C++ testing
framework:

* simple: about 350 lines of code;

* comprehensive: supports test suites, fixtures (by test suite
  constructors and destructors) and exception assertions additionally to
  the standard ``assertTrue()`` and ``assertEqual()``;

* extensible: you can fully customize test event handling (e.g. reporting
  output to a GUI window) by implementing your own test observer; see
  `TextStreamTestView.h`_ for an example.

``testcpp`` is `licenced under the MIT licence`_.

**Status**: production/stable.

Usage
-----

This input::

  class TestSuite1 : public Test::Suite
  {
  public:
      static void testException()
      { throw std::logic_error("exception message"); }

      static void testNoException()
      { int a = 1; ++a; }

      void test()
      {
          Test::assertTrue("1 + 1 == 2 is true",
                  1 + 1 == 2);
          Test::assertTrue("1 + 1 == 3 is true",
                  1 + 1 == 3); // fails
          Test::assertThrows<std::logic_error>("throws std::logic_error",
                  TestSuite1::testException);
          Test::assertThrows<std::logic_error>("doesn't throw std::logic_error",
                  TestSuite1::testNoException); // fails
          testException(); // unhandled exception is caught, but stops the testsuite

          Test::assertTrue("not reached", true);
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
    test exception 'throws std::logic_error': ... OK
    test exception 'doesn't throw std::logic_error': ... FAIL
    Unhandled exception 'St11logic_error' with message: exception message
    FAIL due exception with 2 non-exception errors
  Total test suites run: 1, # of errors: 2, # of uncaught exceptions: 1

See `main test`_ for more details.

.. _`licenced under the MIT licence`: https://github.com/mrts/testcpp/blob/master/LICENCE.rst
.. _main test: https://github.com/mrts/testcpp/blob/master/test/src/main.cpp
.. _TextStreamTestView.h: https://github.com/mrts/testcpp/blob/master/include/testcpp/detail/TextStreamTestView.h
