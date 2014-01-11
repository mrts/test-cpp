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
        ++c._curTestSuiteErrs;
    c._observer->onAssertEnd(ok);
}


#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
void assertWontThrow(const std::string &label,
        std::function<void (void)> testFunction)
{
    Controller &c = Controller::instance();
    c._observer->onAssertNoExceptionBegin(label);
    try {
        testFunction();
    } catch (const std::exception& e) {
        ++c._curTestSuiteErrs;
        c._observer->onAssertExceptionEnd(false, e.what(), typeid(e).name());
        return;
    } catch (...) {
        ++c._curTestSuiteErrs;
        c._observer->onAssertExceptionEnd(false, "<<no message>>", "non-std::exception");
        return;
    }

    c._observer->onAssertEnd(true);
}
#endif

Controller::Controller() :
    _observer(new StdOutView),
    _testSuiteFactories(),
    _curTestSuite(0),
    _curTestSuiteErrs(0),
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
    _curTestSuite = 0;
    size_t testSuiteCount = _testSuiteFactories.size();

    _observer->onAllTestSuitesBegin(testSuiteCount);

    typedef std::map<std::string, TestSuiteFactoryFunction>::iterator
        TestSuiteFactoryIter;

    for (TestSuiteFactoryIter i = _testSuiteFactories.begin(), end = _testSuiteFactories.end();
            i != end; ++i) {

        _curTestSuiteErrs = 0;
        ++_curTestSuite;

        _observer->onTestSuiteBegin(i->first, _curTestSuite, testSuiteCount);

        try {
            // create the test instance and take ownership
            suite_scoped_ptr testsuite(i->second());
            testsuite->test();
            _observer->onTestSuiteEnd(_curTestSuiteErrs);
        } catch (const std::exception &e) {
            _observer->onTestSuiteEndWithStdException(_curTestSuiteErrs, e);
            ++_allTestExcepts;
        } catch (...) {
            _observer->onTestSuiteEndWithEllipsisException(_curTestSuiteErrs);
            ++_allTestExcepts;
        }

        _allTestErrs += _curTestSuiteErrs;
    }

    _observer->onAllTestSuitesEnd(_curTestSuite, testSuiteCount, _allTestErrs, _allTestExcepts);

    return _allTestErrs;
}

} // namespace
