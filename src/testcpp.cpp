#include <testcpp/testcpp.h>
#include <testcpp/StdOutView.h>

#include <typeinfo>

namespace Test
{

void assertTrue(const std::string &testlabel, bool ok)
{
	Controller &c = Controller::instance();
	c._observer->onAssertBegin(testlabel);
	if (! ok)
		++c._curTestErrs;
	c._observer->onAssertEnd(ok);
}

void assertWontThrow(const std::string &testlabel, void (*testfn)())
{
	Controller &c = Controller::instance();
	c._observer->onAssertNoExceptionBegin(testlabel);
	try {
		testfn();
    } catch (const std::exception& e) {
		++c._curTestErrs;
        c._observer->onAssertExceptionEnd(false, e.what());
        return;
	} catch (...) {
		++c._curTestErrs;
		c._observer->onAssertExceptionEnd(false, "caught non-std::exception");
		return;
	}

	c._observer->onAssertEnd(true);
}

Controller::Controller() :
    _observer(new StdOutView),
    _testFuncs(),
    _curTest(0),
    _curTestErrs(0),
    _allTestErrs(0),
    _allTestExcepts(0)
{}

Controller& Controller::instance()
{
    static Controller instance;
	return instance;
}

int Controller::run()
{
	_curTest = 0;

    typedef std::map<std::string, TestSuiteFactoryFunction>::iterator
        TestSuiteFactoryIter;

	for (TestSuiteFactoryIter i = _testFuncs.begin(), end = _testFuncs.end();
            i != end; ++i) {

		_curTestErrs = 0;
		++_curTest;

		_observer->onTestBegin(i->first, _curTest, _testFuncs.size());

		try {
			// create the test instance and take ownership
			scoped_pointer<Suite> test(i->second());
			test->test();
			_observer->onTestEnd(_curTestErrs);
		} catch (const std::exception &e) {
			_observer->onTestEnd(_curTestErrs, typeid(e).name(), e.what());
			++_allTestExcepts;
		} catch (...) {
			_observer->onTestEnd(_curTestErrs, "caught non-std::exception");
			++_allTestExcepts;
		}

		_allTestErrs += _curTestErrs;
	}

	_observer->onAllTestsEnd(_testFuncs.size(), _allTestErrs, _allTestExcepts);

	return _allTestErrs;
}

} // namespace
