#ifndef TESTCPP_THROWS_H__
#define TESTCPP_THROWS_H__

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
template <typename ExceptionType>
void assertThrows(const std::string &label,
        std::function<void (void)> testFunction)
#else
template <class TestSuiteType,
          typename TestMethodType,
          typename ExceptionType>
void assertThrows(const std::string &label,
                  TestSuiteType& testSuiteObject,
                  TestMethodType testFunction)
#endif
{
    Controller& c = Controller::instance();

    c._observer->onAssertExceptionBegin(label);

    try {
#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
        testFunction();
#else
        // call object method
        ((testSuiteObject).*(testFunction))();
#endif
    } catch (const ExceptionType& e) {
        c._observer->onAssertExceptionEndWithExpectedException(e);
        return;
    } catch (const std::exception& e) {
        ++c._curTestSuiteErrs;
        c._observer->onAssertExceptionEndWithUnexpectedException(e);
        return;
    } catch (...) {
        ++c._curTestSuiteErrs;
        c._observer->onAssertExceptionEndWithEllipsisException();
        return;
    }

    ++c._curTestSuiteErrs;
    c._observer->onAssertEnd(false);
}

#if !(defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L))
template <class TestSuiteType,
          typename TestMethodType>
void assertWontThrow(const std::string &label,
                  TestSuiteType& testSuiteObject,
                  TestMethodType testFunction)
{
    Controller &c = Controller::instance();
    c._observer->onAssertNoExceptionBegin(label);
    try {
        ((testSuiteObject).*(testFunction))();
    } catch (const std::exception& e) {
        ++c._curTestSuiteErrs;
        c._observer->onAssertNoExceptionEndWithStdException(e);
        return;
    } catch (...) {
        ++c._curTestSuiteErrs;
        c._observer->onAssertNoExceptionEndWithEllipsisException();
        return;
    }

    c._observer->onAssertEnd(true);
}
#endif

template <typename CompareType>
void assertEqual(const std::string& label,
        const CompareType& a, const CompareType& b)
{
    Controller& c = Controller::instance();

    c._observer->onAssertBegin(label);

    bool ok = (a == b);

    if (!ok)
        ++c._curTestSuiteErrs;

    c._observer->onAssertEnd(ok);
}

template <typename CompareType>
void assertNotEqual(const std::string& label,
        const CompareType& a, const CompareType& b)
{
    Controller& c = Controller::instance();

    c._observer->onAssertBegin(label);

    bool ok = (a != b);

    if (!ok)
        ++c._curTestSuiteErrs;

    c._observer->onAssertEnd(ok);
}

#endif /* TESTCPP_THROWS_H */
