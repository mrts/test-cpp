#ifndef TESTCPP_THROWS_H__
#define TESTCPP_THROWS_H__

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
  #include <functional>
#endif

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
        c._observer->onAssertExceptionEnd(true, e.what());
        return;
    }

    ++c._curTestErrs;
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
#endif

template <typename CompareType>
void assertEqual(const std::string& label,
        const CompareType& a, const CompareType& b)
{
    Controller& c = Controller::instance();

    c._observer->onAssertBegin(label);

    bool ok = (a == b);

    if (!ok)
        ++c._curTestErrs;

    c._observer->onAssertEnd(ok);
}

#endif /* TESTCPP_THROWS_H */
