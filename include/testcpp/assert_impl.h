#ifndef TESTCPP_THROWS_H__
#define TESTCPP_THROWS_H__

template <class TestSuiteType,
          typename TestMethodType,
          typename ExceptionType>
void assertThrows(const std::string &label,
                  TestSuiteType& testSuiteObject,
                  TestMethodType testFunction)
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
void assertWontThrow(const std::string &label,
                  TestSuiteType& testSuiteObject,
                  TestMethodType testFunction)
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
void assertEqual(const std::string& label,
        const FirstCompareType& a, const SecondCompareType& b)
{
    Controller &c = Controller::instance();
    c.beforeAssert(label);

    bool ok = (a == b);

    c.afterAssert(ok);
}

template <typename CompareType>
void assertEqual(const std::string& label,
        const CompareType& a, const CompareType& b)
{
    assertEqual<CompareType, CompareType>(label, a, b);
}

template <typename FirstCompareType, typename SecondCompareType>
void assertNotEqual(const std::string& label,
        const FirstCompareType& a, const SecondCompareType& b)
{
    Controller &c = Controller::instance();
    c.beforeAssert(label);

    bool ok = (a != b);

    c.afterAssert(ok);
}

template <typename CompareType>
void assertNotEqual(const std::string& label,
        const CompareType& a, const CompareType& b)
{
    assertNotEqual<CompareType, CompareType>(label, a, b);
}

#endif /* TESTCPP_THROWS_H */
