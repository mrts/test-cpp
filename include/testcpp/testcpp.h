/**
 * A simple yet complete testing framework.
 *
 * Copyright (c) 2007, 2011, 2012 Mart Sõmermaa.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the MIT License as published by the Open Source
 * Initiative group on the web at
 * http://opensource.org/licenses/MIT
 *
 * Version: 3.1.0
 */

#ifndef TESTCPP_H__
#define TESTCPP_H__

#include <utilcpp/declarations.h>
#if !(defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L))
  #include <utilcpp/scoped_ptr.h>
#endif

#include <string>
#include <vector>
#include <memory>
#include <utility>

#include <exception>

namespace Test
{

class Suite;

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
  typedef std::unique_ptr<Suite> suite_transferable_ptr;
  typedef std::unique_ptr<Suite> suite_scoped_ptr;
#else
  typedef std::auto_ptr<Suite> suite_transferable_ptr;
  typedef utilcpp::scoped_ptr<Suite> suite_scoped_ptr;
#endif

/**
 * Inteface for test suites. Test suite is a collection of assert()s in the
 * test() method.
 *
 * For fixture behaviour setup things in constructor and tear down in
 * destructor.
 */
class Suite
{
    UTILCPP_DECLARE_INTERFACE(Suite)

public:
    /** Implement this. */
    virtual void test() = 0;

    /** Factory method for creating concrete testsuites. */
    template <class ConcreteSuiteType>
    static suite_transferable_ptr instance()
    {
        return suite_transferable_ptr(new ConcreteSuiteType());
    }
};

void assertTrueImpl(const std::string& label, bool ok,
        const char* const function, const char* const file, int line);

template <typename FirstCompareType, typename SecondCompareType>
void assertEqualImpl(const std::string& label,
        const FirstCompareType& a, const SecondCompareType& b,
        const char* const function, const char* const file, int line);

template <typename FirstCompareType, typename SecondCompareType>
void assertNotEqualImpl(const std::string& label,
        const FirstCompareType& a, const SecondCompareType& b,
        const char* const function, const char* const file, int line);

template <class TestSuiteType,
          typename TestMethodType,
          typename ExceptionType>
void assertThrowsImpl(const std::string &label,
        TestSuiteType& testSuiteObject, TestMethodType testFunction,
        const char* const function, const char* const file, int line);

template <class TestSuiteType,
          typename TestMethodType>
void assertWontThrowImpl(const std::string &label,
        TestSuiteType& testSuiteObject, TestMethodType testFunction,
        const char* const function, const char* const file, int line);
}

// macros are in the global namespace

#ifdef _WIN32
#define function__ __FUNCTION__
#else
#define function__ __PRETTY_FUNCTION__
#endif

#define TESTCPP_TYPEDEF_TESTMETHOD(classname__) \
    typedef void (classname__::*TestMethod)();

// http://stackoverflow.com/questions/5530505/why-does-this-variadic-argument-count-macro-fail-with-vc
#define EXPAND_MACRO(x__) x__
#define GET_MACRO_OVERLOAD(_1, _2, _3, _4, _5, _6, NAME, ...) NAME

#define assertTrue1(ok__) \
    Test::assertTrueImpl(#ok__, (ok__), function__, __FILE__, __LINE__)

#define assertTrue2(label__, ok__) \
    Test::assertTrueImpl(label__, (ok__), function__, __FILE__, __LINE__)

#define assertTrue(...) \
    EXPAND_MACRO(GET_MACRO_OVERLOAD(__VA_ARGS__, _, _, _, _, \
            assertTrue2, assertTrue1)(__VA_ARGS__))


#define assertFalse1(ok__) \
    Test::assertTrueImpl("!("#ok__")", !(ok__), function__, __FILE__, __LINE__)

#define assertFalse2(label__, ok__) \
    Test::assertTrueImpl(label__, !(ok__), function__, __FILE__, __LINE__)

#define assertFalse(...) \
    EXPAND_MACRO(GET_MACRO_OVERLOAD(__VA_ARGS__, _, _, _, _, \
            assertFalse2, assertFalse1)(__VA_ARGS__))


#define assertEqual1(a__, b__) \
    Test::assertEqualImpl(#a__ " == " #b__, (a__), (b__), \
            function__, __FILE__, __LINE__)

#define assertEqual2(label__, a__, b__) \
    Test::assertEqualImpl(label__, (a__), (b__), function__, __FILE__, __LINE__)

#define assertEqual(...) \
    EXPAND_MACRO(GET_MACRO_OVERLOAD(__VA_ARGS__, _, _, _, \
            assertEqual2, assertEqual1)(__VA_ARGS__))


#define assertNotEqual1(a__, b__) \
    Test::assertNotEqualImpl(#a__ " != " #b__, (a__), (b__), \
            function__, __FILE__, __LINE__)

#define assertNotEqual2(label__, a__, b__) \
    Test::assertNotEqualImpl(label__, (a__), (b__), \
            function__, __FILE__, __LINE__)

#define assertNotEqual(...) \
    EXPAND_MACRO(GET_MACRO_OVERLOAD(__VA_ARGS__, _, _, _, \
            assertNotEqual2, assertNotEqual1)(__VA_ARGS__))


#define assertThrows1(suitetype__, functiontype__, exceptiontype__, \
        object__, functionptr__) \
    Test::assertThrowsImpl<suitetype__, functiontype__, exceptiontype__> \
    (#functionptr__ " throws " #exceptiontype__, \
     object__, functionptr__, function__, __FILE__, __LINE__)

#define assertThrows2(label__, suitetype__, functiontype__, exceptiontype__, \
        object__, functionptr__) \
    Test::assertThrowsImpl<suitetype__, functiontype__, exceptiontype__> \
    (label__, object__, functionptr__, function__, __FILE__, __LINE__)

#define assertThrows(...) \
    EXPAND_MACRO(GET_MACRO_OVERLOAD(__VA_ARGS__, \
			assertThrows2, assertThrows1)(__VA_ARGS__))


#define assertWontThrow1(suitetype__, functiontype__, \
        object__, functionptr__) \
    Test::assertWontThrowImpl<suitetype__, functiontype__> \
    (#functionptr__ " won't throw", \
     object__, functionptr__, function__, __FILE__, __LINE__)

#define assertWontThrow2(label__, suitetype__, functiontype__, \
        object__, functionptr__) \
    Test::assertWontThrowImpl<suitetype__, functiontype__> \
    (label__, object__, functionptr__, function__, __FILE__, __LINE__)

#define assertWontThrow(...) \
    EXPAND_MACRO(GET_MACRO_OVERLOAD(__VA_ARGS__, _, \
            assertWontThrow2, assertWontThrow1)(__VA_ARGS__))

namespace Test
{

/**
 * Interface for observing test progress. Suitable for displaying results,
 * timing etc.
 */
class Observer
{
    UTILCPP_DECLARE_INTERFACE(Observer)

public:
    virtual void onTestSuiteBegin(const std::string& testSuiteLabel,
            int testSuiteNum, int testSuitesNumTotal) = 0;

    virtual void onTestSuiteEnd(int numErrs) = 0;
    virtual void onTestSuiteEndWithStdException(int numErrs, const std::exception& e) = 0;
    virtual void onTestSuiteEndWithEllipsisException(int numErrs) = 0;

    virtual void onAssertBegin(const std::string& testlabel,
        const char* const function, const char* const file, int line) = 0;
    virtual void onAssertExceptionBegin(const std::string& testlabel,
        const char* const function, const char* const file, int line) = 0;
    virtual void onAssertNoExceptionBegin(const std::string& testlabel,
        const char* const function, const char* const file, int line) = 0;

    virtual void onAssertEnd(bool ok) = 0;
    virtual void onAssertExceptionEndWithExpectedException(const std::exception& e) = 0;
    virtual void onAssertExceptionEndWithUnexpectedException(const std::exception& e) = 0;
    virtual void onAssertExceptionEndWithEllipsisException() = 0;
    virtual void onAssertNoExceptionEndWithStdException(const std::exception& e) = 0;
    virtual void onAssertNoExceptionEndWithEllipsisException() = 0;

    virtual void onAllTestSuitesBegin(int testSuitesNumTotal) = 0;
    virtual void onAllTestSuitesEnd(int lastTestSuiteNum, int testSuitesNumTotal, int numErrs, int numExcepts) = 0;
};


/** The singleton control class registers tests and controls execution. */
class Controller
{
public:
    static Controller& instance();

    typedef suite_transferable_ptr (*TestSuiteFactoryFunction)();
    typedef std::pair<std::string, TestSuiteFactoryFunction> LabelAndFactoryFunctionPair;

    void addTestSuite(const std::string &label, TestSuiteFactoryFunction ffn)
    { _testSuiteFactories.push_back(LabelAndFactoryFunctionPair(label, ffn)); }

    void setObserver(Observer* observer, bool takeOwnership = true)
    {
        if (!observer)
            throw std::runtime_error("Observer cannot be null");
        _doesOwnObserver = takeOwnership;
        _observer = observer;
    }

    int run();

    void beforeAssert(const std::string& label,
        const char* const function, const char* const file, int line)
    { _observer->onAssertBegin(label, function, file, line); }

    void afterAssert(bool ok)
    {
        if (!ok)
            ++_curTestSuiteErrs;
        _observer->onAssertEnd(ok);
    }

    void beforeAssertException(const std::string& label,
        const char* const function, const char* const file, int line)
    { _observer->onAssertExceptionBegin(label, function, file, line); }

    void onAssertExceptionEndWithExpectedException(const std::exception& e)
    { _observer->onAssertExceptionEndWithExpectedException(e); }

    void onAssertExceptionEndWithUnexpectedException(const std::exception* e = 0)
    {
        ++_curTestSuiteErrs;
        if (e)
            _observer->onAssertExceptionEndWithUnexpectedException(*e);
        else
            _observer->onAssertExceptionEndWithEllipsisException();
    }

    void beforeAssertNoException(const std::string& label,
        const char* const function, const char* const file, int line)
    { _observer->onAssertNoExceptionBegin(label, function, file, line); }

    void onAssertNoExceptionEndWithException(const std::exception* e = 0)
    {
        ++_curTestSuiteErrs;
        if (e)
            _observer->onAssertNoExceptionEndWithStdException(*e);
        else
            _observer->onAssertNoExceptionEndWithEllipsisException();
    }

private:
    Controller();
    Controller(const Controller&);
    Controller& operator=(const Controller&);
    ~Controller()
    {
        if (_doesOwnObserver)
            delete _observer;
    }

    Observer* _observer;
    bool _doesOwnObserver;

    std::vector<LabelAndFactoryFunctionPair> _testSuiteFactories;

    int _curTestSuite;
    int _curTestSuiteErrs;
    int _allTestErrs;
    int _allTestExcepts;
};

// include assert* implementation after Controller has been declared
#include <testcpp/assert_impl.h>

}

#endif /* TESTCPP_H */
