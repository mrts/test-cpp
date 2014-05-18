#ifndef TESTCPP_ASSERT_IMPL_H__
#define TESTCPP_ASSERT_IMPL_H__

class AssertTrue : public AssertType
{
public:
    // or just use lowcase classes and typeid().name?
    virtual std::string name() { return "assertTrue"; }
    virtual std::string label() { return ""; }
};

class AssertEqual : public AssertType
{
public:
    virtual std::string name() { return "assertEqual"; }
    virtual std::string label() { return ""; }
};

class AssertNotEqual : public AssertType
{
public:
    virtual std::string name() { return "assertNotEqual"; }
    virtual std::string label() { return ""; }
};

class AssertThrows : public AssertType
{
public:
    virtual std::string name() { return "assertThrows"; }
    virtual std::string label() { return ""; }
};

class AssertWontThrow : public AssertType
{
public:
    virtual std::string name() { return "assertWontThrow"; }
    virtual std::string label() { return ""; }
};


inline void assertTrueImpl(const std::string& label, bool ok,
        const char* const function, const char* const file, int line)
{
    Controller &c = Controller::instance();
    asserttype_transferable_ptr assertType(new AssertTrue());
    c.beforeAssert(assertType, label, function, file, line);
    c.afterAssert(ok);
}

template <typename FirstCompareType, typename SecondCompareType>
void assertEqualImpl(const std::string& label,
        const FirstCompareType& a, const SecondCompareType& b,
        const char* const function, const char* const file, int line)
{
    Controller &c = Controller::instance();
    asserttype_transferable_ptr assertType(new AssertEqual());
    c.beforeAssert(assertType, label, function, file, line);

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
    asserttype_transferable_ptr assertType(new AssertNotEqual());
    c.beforeAssert(assertType, label, function, file, line);

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
    asserttype_transferable_ptr assertType(new AssertThrows());
    c.beforeAssert(assertType, label, function, file, line);

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
    asserttype_transferable_ptr assertType(new AssertWontThrow());
    c.beforeAssert(assertType, label, function, file, line);

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
