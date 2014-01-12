#ifndef TESTCPP_THROWS_H__
#define TESTCPP_THROWS_H__

inline void assertTrueImpl(const std::string& testlabel, bool ok,
        const char* const /* function */, const char* const /* file */, int /* line */)
{
    Controller &c = Controller::instance();
    c.beforeAssert(testlabel);
    c.afterAssert(ok);
}

template <class TestSuiteType,
          typename TestMethodType,
          typename ExceptionType>
void assertThrowsImpl(const std::string &label,
                  TestSuiteType& testSuiteObject,
                  TestMethodType testFunction,
        const char* const /* function */, const char* const /* file */, int /* line */)
{
    Controller& c = Controller::instance();
    c.beforeAssertException(label);

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
                  TestSuiteType& testSuiteObject,
                  TestMethodType testFunction,
        const char* const /* function */, const char* const /* file */, int /* line */)
{
    Controller& c = Controller::instance();
    c.beforeAssertNoException(label);

    try {
        ((testSuiteObject).*(testFunction))();
        c.afterAssert(true);
    } catch (const std::exception& e) {
        c.onAssertNoExceptionEndWithException(&e);
    } catch (...) {
        c.onAssertNoExceptionEndWithException();
    }
}

template <typename FirstCompareType, typename SecondCompareType>
void assertEqualImpl(const std::string& label,
        const FirstCompareType& a, const SecondCompareType& b,
        const char* const /* function */, const char* const /* file */, int /* line */)
{
    Controller &c = Controller::instance();
    c.beforeAssert(label);

    bool ok = (a == b);

    c.afterAssert(ok);
}

template <typename CompareType>
void assertEqualImpl(const std::string& label,
        const CompareType& a, const CompareType& b,
        const char* const function, const char* const file, int line)
{
    assertEqualImpl<CompareType, CompareType>(label, a, b, function, file, line);
}

// need separate assertNotEqual because operator!= may be overriden
template <typename FirstCompareType, typename SecondCompareType>
void assertNotEqualImpl(const std::string& label,
        const FirstCompareType& a, const SecondCompareType& b,
        const char* const /* function */, const char* const /* file */, int /* line */)
{
    Controller &c = Controller::instance();
    c.beforeAssert(label);

    bool ok = (a != b);

    c.afterAssert(ok);
}

template <typename CompareType>
void assertNotEqualImpl(const std::string& label,
        const CompareType& a, const CompareType& b,
        const char* const function, const char* const file, int line)
{
    assertNotEqualImpl<CompareType, CompareType>(label, a, b, function, file, line);
}

#endif /* TESTCPP_THROWS_H */
