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
    typedef void (Object::*Method)();

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
    typedef void (TestSuite1::*TestMethod)();

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
        Test::assertTrue("1 + 1 == 2 is true",
                1 + 1 == 2);

        Test::assertTrue("1 + 1 == 3 is true (must FAIL)",
                1 + 1 == 3);

        Test::assertFalse("1 + 1 == 2 is false (must FAIL)",
                1 + 1 == 2);

        Test::assertFalse("1 + 1 == 3 is false",
                1 + 1 == 3);

        Test::assertEqual<int>("1 + 1 == 2",
                1 + 1, 2);

        Test::assertEqual<int>("1 + 1 == 3 (must FAIL)",
                1 + 1, 3);

        Test::assertNotEqual<int>("1 + 1 != 2 (must FAIL)",
                1 + 1, 2);

        Test::assertNotEqual<int>("1 + 1 != 3",
                1 + 1, 3);

        Test::assertEqual<std::string>("\"abc\" == \"abc\"",
                "abc", "abc");

        Test::assertWontThrow<TestSuite1, TestMethod>
            ("won't throw exceptions",
                *this, &TestSuite1::testObjectWontThrow);

        Test::assertThrows<TestSuite1, TestMethod, std::logic_error>
            ("throws logic_error",
                *this, &TestSuite1::testObjectThrows);

        Test::assertWontThrow<TestSuite1, TestMethod>
            ("throws logic_error (must FAIL)",
                *this, &TestSuite1::testObjectThrows);

        Test::assertThrows<TestSuite1, TestMethod, std::logic_error>
            ("doesn't throw logic_error (must FAIL)",
                *this, &TestSuite1::testObjectWontThrow);

        // You can also use object-under-test methods directly.
        // This assert fails as the exception thrown is not std::runtime_error.
        Test::assertThrows<Object, Object::Method, std::runtime_error>
            ("throws logic_error instead of runtime_error (must FAIL)",
                _object, &Object::instanceMethodThrowsException);

        // The unhandled exception is caught, but stops the testsuite.
        testObjectThrows();
        Test::assertTrue("not reached because of "
                "the previous unhandled exception", true);
    }

private:
    Object _object;
};

int main()
{
    // Example of running tests outside of a suite.
    // Controller initializes the StdOutView observer by default.

    Test::assertTrue("1 - 1 == 0 is true",
            1 - 1 == 0);

    // Example of running a test suite.
    // Also demonstrate explicitly initializing the observer.

    Test::Controller &c = Test::Controller::instance();
    c.setObserver(Test::observer_transferable_ptr(new Test::ColoredStdOutView));

    c.addTestSuite("testsuite1", Test::Suite::instance<TestSuite1>);

    int numErrors = c.run();

    std::cout << "Press ENTER to exit" << std::endl;
    std::cin.get();

    return numErrors;
}
