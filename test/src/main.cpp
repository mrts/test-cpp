#include <testcpp/testcpp.h>
#include <testcpp/StdOutView.h>
#include <utilcpp/disable_copy.h>

#include <stdexcept>
#include <string>
#include <iostream>

class Object
{
    UTILCPP_DISABLE_COPY(Object)

public:
    Object():
        _ctr(0)
    { }

    void instanceMethodThrowsException()
    { throw std::logic_error("exception message"); }

    void instanceMethodNoException()
    { ++_ctr; }

private:
    int _ctr;
};

class TestSuite1 : public Test::Suite
{
    UTILCPP_DISABLE_COPY(TestSuite1)

public:
    // typedefs for assertThrows and assertWontThrow
    TESTCPP_TYPEDEFS(TestSuite1)

    // setUp() happens in the constructor
    TestSuite1() :
        _object()
    { }

    // tearDown() happens in the destructor
    ~TestSuite1()
    { }

    void testObjectWontThrow()
    {
        _object.instanceMethodNoException();
    }

    void testObjectThrows()
    {
        _object.instanceMethodThrowsException();
    }

    void test()
    {
        assertTrue("1 + 1 == 2 is true",
                1 + 1 == 2);
        assertTrue(1 + 1 == 2);

        assertTrue("1 + 1 == 3 is true (must FAIL)",
                1 + 1 == 3);
        assertTrue(1 + 1 == 3);

        assertFalse("1 + 1 == 2 is false (must FAIL)",
                1 + 1 == 2);
        assertFalse(1 + 1 == 2);

        assertFalse("1 + 1 == 3 is false",
                1 + 1 == 3);
        assertFalse(1 + 1 == 3);

        assertEqual("1 + 1 == 2",
                1 + 1, 2);
        assertEqual(1 + 1, 2);

        assertEqual("1 + 1 == 3 (must FAIL)",
                1 + 1, 3);
        assertEqual(1 + 1, 3);

        assertNotEqual("1 + 1 != 2 (must FAIL)",
                1 + 1, 2);
        assertNotEqual(1 + 1, 2);

        assertNotEqual("1 + 1 != 3",
                1 + 1, 3);
        assertNotEqual(1 + 1, 3);

        assertEqual("\"abc\" == \"abc\"",
                "abc", "abc");
        assertEqual("abc", "abc");

        assertWontThrow("won't throw exceptions", testObjectWontThrow);
        assertWontThrow(testObjectWontThrow);

        assertThrows("throws logic_error", testObjectThrows, std::logic_error);
        assertThrows(testObjectThrows, std::logic_error);

        assertWontThrow("throws logic_error (must FAIL)", testObjectThrows);
        assertWontThrow(testObjectThrows);

        assertThrows("doesn't throw logic_error (must FAIL)",
                testObjectWontThrow, std::logic_error);
        assertThrows(testObjectWontThrow, std::logic_error);

        // Expect failure as the exception thrown is not std::runtime_error.
        assertThrows("throws logic_error instead of runtime_error (must FAIL)",
                testObjectThrows, std::runtime_error);

        // The unhandled exception is caught, but stops the testsuite.
        testObjectThrows();

        assertTrue("not reached because of the previous unhandled exception",
                true);
    }

private:
    Object _object;
};

int main()
{
    // Example of running tests outside of a suite.
    // Controller initializes the StdOutView observer by default.

    assertTrue("1 - 1 == 0 is true", 1 - 1 == 0);

    // Example of running a test suite.
    // Also demonstrate explicitly initializing the observer.

    Test::Controller &c = Test::Controller::instance();
    c.setObserver(new Test::ColoredStdOutView);

    c.addTestSuite("testsuite1", Test::Suite::instance<TestSuite1>);

    int numErrors = c.run();

#ifdef ASK_FOR_PRESS_ENTER_BEFORE_EXIT
    std::cout << "Press ENTER to exit" << std::endl;
    std::cin.get();
#endif

    return numErrors;
}
