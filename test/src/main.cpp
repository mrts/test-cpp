#include <testcpp/testcpp.h>
#include <testcpp/StdOutView.h>

#include <stdexcept>

void testExceptionGlobal()
{ throw std::logic_error("exception message"); }

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

		testException();

        Test::assertTrue("not reached", true);
	}
};

int main()
{
    // Example of running tests outside of a suite.
    // Controller initializes the StdOutView observer by default.

    Test::assertThrows<std::logic_error>("throws std::logic_error",
           testExceptionGlobal);

    Test::assertWontThrow("throws std::logic_error",
            testExceptionGlobal); // fails

    Test::assertTrue("1 - 1 == 0 is true",
            1 - 1 == 0);

    Test::assertTrue("1 - 2 == 0 is true", 1 - 2 == 0); // fails

    Test::Controller &c = Test::Controller::instance();

    // example of explicitly initializing the observer
    c.setObserver(std::auto_ptr<Test::Observer>(new Test::StdOutView));

    c.addTestSuite("testsuite1", Test::Suite::instance<TestSuite1>);

    return c.run();
}
