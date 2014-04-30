#include <testcpp/testcpp.h>
#include <testcpp/StdOutView.h>
#include <utilcpp/disable_copy.h>

#include <stdexcept>
#include <string>
#include <iostream>

void testExceptionGlobal()
{
    throw std::logic_error("exception message");
}

class Object
{
    UTILCPP_DISABLE_COPY(Object)

public:
    // must define method pointer signature when using C++03
    TESTCPP_TYPEDEF_TESTMETHOD(Object)

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
    // must define method pointer signature when using C++03
    TESTCPP_TYPEDEF_TESTMETHOD(TestSuite1)

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

        assertWontThrow("won't throw exceptions",
                TestSuite1, TestMethod,
                *this, &TestSuite1::testObjectWontThrow);
        assertWontThrow(TestSuite1, TestMethod,
                *this, &TestSuite1::testObjectWontThrow);

        assertThrows("throws logic_error",
                TestSuite1, TestMethod, std::logic_error,
                *this, &TestSuite1::testObjectThrows);
        assertThrows(TestSuite1, TestMethod, std::logic_error,
                *this, &TestSuite1::testObjectThrows);

        assertWontThrow("throws logic_error (must FAIL)",
                TestSuite1, TestMethod,
                *this, &TestSuite1::testObjectThrows);
        assertWontThrow(TestSuite1, TestMethod,
                *this, &TestSuite1::testObjectThrows);

        assertThrows("doesn't throw logic_error (must FAIL)",
                TestSuite1, TestMethod, std::logic_error,
                *this, &TestSuite1::testObjectWontThrow);
        assertThrows(TestSuite1, TestMethod, std::logic_error,
                *this, &TestSuite1::testObjectWontThrow);

        // You can also use object-under-test methods directly.
        // This assert fails as the exception thrown is not std::runtime_error.
        assertThrows("throws logic_error instead of runtime_error (must FAIL)",
                Object, Object::TestMethod, std::runtime_error,
                _object, &Object::instanceMethodThrowsException);
        assertThrows(Object, Object::TestMethod, std::runtime_error,
                _object, &Object::instanceMethodThrowsException);

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
