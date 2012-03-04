#include <testcpp/testcpp.h>
#include <testcpp/StdOutView.h>
#include <utilcpp/disable_copy.h>

#include <stdexcept>
#include <string>

void testExceptionGlobal()
{
    throw std::logic_error("exception message");
}

class Object
{
    UTILCPP_DISABLE_COPY(Object)

public:
#if !(defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L))
    // must define method pointer signature when using C++03
    typedef void (Object::*Method)();
#endif

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
#if !(defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L))
    // must define method pointer signature when using C++03
    typedef void (TestSuite1::*TestMethod)();
#endif

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

        Test::assertTrue("1 + 1 == 3 is true",
                1 + 1 == 3); // fails

        Test::assertEqual<int>("1 + 1 == 2",
                1 + 1, 2);

        Test::assertEqual<int>("1 + 1 == 3",
                1 + 1, 3); // fails

        Test::assertEqual<std::string>("\"abc\" == \"abc\"",
                "abc", "abc");

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
        // use lambdas with C++11
        Test::assertWontThrow("won't throw exceptions",
                [&] () { _object.instanceMethodNoException(); });

        Test::assertThrows<std::logic_error>("throws logic_error",
                [&] () { _object.instanceMethodThrowsException(); });

        Test::assertWontThrow("throws logic_error",
                [&] () { _object.instanceMethodThrowsException(); }); // fails

        Test::assertThrows<std::logic_error>("doesn't throw logic_error",
                [&] () { _object.instanceMethodNoException(); }); // fails

        // This assert fails as the exception thrown is not std::runtime_error.
        Test::assertThrows<std::runtime_error>("throws logic_error instead of runtime_error",
                [&] () { _object.instanceMethodThrowsException(); }); // fails

#else
        // use method pointers and templates with C++03
        Test::assertWontThrow<TestSuite1, TestMethod>
            ("won't throw exceptions",
                *this, &TestSuite1::testObjectWontThrow);

        Test::assertThrows<TestSuite1, TestMethod, std::logic_error>
            ("throws logic_error",
                *this, &TestSuite1::testObjectThrows);

        Test::assertWontThrow<TestSuite1, TestMethod>
            ("throws logic_error",
                *this, &TestSuite1::testObjectThrows); // fails

        Test::assertThrows<TestSuite1, TestMethod, std::logic_error>
            ("doesn't throw logic_error",
                *this, &TestSuite1::testObjectWontThrow); // fails

        // You can also use object-under-test methods directly.
        // This assert fails as the exception thrown is not std::runtime_error.
        Test::assertThrows<Object, Object::Method, std::runtime_error>
            ("throws logic_error instead of runtime_error",
                _object, &Object::instanceMethodThrowsException); // fails
#endif

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

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
    // When using C++11, exception assertions can be used outside
    // test suites, otherwise only inside.
    Test::assertThrows<std::logic_error>("throws std::logic_error",
           testExceptionGlobal);
#endif

    Test::assertTrue("1 - 1 == 0 is true",
            1 - 1 == 0);

    // Example of running a test suite.
    // Also demonstrate explicitly initializing the observer.

    Test::Controller &c = Test::Controller::instance();
    c.setObserver(Test::observer_transferable_ptr(new Test::StdOutView));

    c.addTestSuite("testsuite1", Test::Suite::instance<TestSuite1>);

    return c.run();
}
