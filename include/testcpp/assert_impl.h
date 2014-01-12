#ifndef TESTCPP_THROWS_H__
#define TESTCPP_THROWS_H__

inline void assertTrueImpl(const std::string& label, bool ok,
        const char* const function, const char* const file, int line)
{
    Controller &c = Controller::instance();
    c.beforeAssert(label, function, file, line);
    c.afterAssert(ok);
}

template <typename FirstCompareType, typename SecondCompareType>
void assertEqualImpl(const std::string& label,
        const FirstCompareType& a, const SecondCompareType& b,
        const char* const function, const char* const file, int line)
{
    Controller &c = Controller::instance();
    c.beforeAssert(label, function, file, line);

    bool ok = (a == b);

    c.afterAssert(ok);
}

// need separate assertNotEqual because operator!= may be overriden
template <typename FirstCompareType, typename SecondCompareType>
void assertNotEqualImpl(const std::string& label,
        const FirstCompareType& a, const SecondCompareType& b,
        const char* const function, const char* const file, int line)
{
    Controller &c = Controller::instance();
    c.beforeAssert(label, function, file, line);

    bool ok = (a != b);

    c.afterAssert(ok);
}

template <class TestSuiteType,
          typename TestMethodType,
          typename ExceptionType>
void assertThrowsImpl(const std::string &label,
                  TestSuiteType& testSuiteObject,
                  TestMethodType testFunction,
        const char* const function, const char* const file, int line)
{
    Controller& c = Controller::instance();
    c.beforeAssertException(label, function, file, line);

    try {
        // call object method
        ((testSuiteObject).*(testFunction))();
        c.afterAssert(false);
    } catch (const ExceptionType& e) {
        c.onAssertExceptionEndWithExpectedException(e);
    } catch (const std::exception& e) {
        c.onAssertExceptionEndWithUnexpectedException(&e);
    } catch (...) {
        c.onAssertExceptionEndWithUnexpectedException();
    }
}

template <class TestSuiteType,
          typename TestMethodType>
void assertWontThrowImpl(const std::string &label,
        TestSuiteType& testSuiteObject, TestMethodType testFunction,
        const char* const function, const char* const file, int line)
{
    Controller& c = Controller::instance();
    c.beforeAssertNoException(label, function, file, line);

    try {
        ((testSuiteObject).*(testFunction))();
        c.afterAssert(true);
    } catch (const std::exception& e) {
        c.onAssertNoExceptionEndWithException(&e);
    } catch (...) {
        c.onAssertNoExceptionEndWithException();
    }
}

#endif /* TESTCPP_THROWS_H */
