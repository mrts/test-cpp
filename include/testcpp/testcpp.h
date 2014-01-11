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
#include <memory>
#include <map>

#include <typeinfo>
#include <exception>

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
  #include <functional>
#endif

namespace Test
{

class Suite;
class Observer;

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
  typedef std::unique_ptr<Suite> suite_transferable_ptr;
  typedef std::unique_ptr<Suite> suite_scoped_ptr;
  typedef std::unique_ptr<Observer> observer_transferable_ptr;
  typedef std::unique_ptr<Observer> observer_scoped_ptr;
#else
  typedef std::auto_ptr<Suite> suite_transferable_ptr;
  typedef utilcpp::scoped_ptr<Suite> suite_scoped_ptr;
  typedef std::auto_ptr<Observer> observer_transferable_ptr;
  typedef utilcpp::scoped_ptr<Observer> observer_scoped_ptr;
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

    virtual void onAssertBegin(const std::string& testlabel) = 0;
    virtual void onAssertExceptionBegin(const std::string& testlabel) = 0;
    virtual void onAssertNoExceptionBegin(const std::string& testlabel) = 0;

    virtual void onAssertEnd(bool ok) = 0;
    virtual void onAssertExceptionEndWithExpectedException(const std::exception& e) = 0;
    virtual void onAssertExceptionEndWithUnexpectedException(const std::exception& e) = 0;
    virtual void onAssertExceptionEndWithEllipsisException() = 0;
    virtual void onAssertNoExceptionEndWithStdException(const std::exception& e) = 0;
    virtual void onAssertNoExceptionEndWithEllipsisException() = 0;

    virtual void onAllTestSuitesBegin(int testSuitesNumTotal) = 0;
    virtual void onAllTestSuitesEnd(int lastTestSuiteNum, int testSuitesNumTotal, int numErrs, int numExcepts) = 0;
};


void assertTrue(const std::string& label, bool ok);

inline void assertFalse(const std::string& label, bool ok)
{ return assertTrue(label, !ok); }

template <typename CompareType>
void assertEqual(const std::string& label,
        const CompareType& a, const CompareType& b);

template <typename CompareType>
void assertNotEqual(const std::string& label,
        const CompareType& a, const CompareType& b);

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
// use lambdas with C++11
template <typename ExceptionType>
void assertThrows(const std::string &label,
        std::function<void (void)> testFunction);

void assertWontThrow(const std::string &label,
        std::function<void (void)> testFunction);

#else
template <class TestSuiteType,
          typename TestMethodType,
          typename ExceptionType>
void assertThrows(const std::string &label,
                  TestSuiteType& testSuiteObject,
                  TestMethodType testFunction);

template <class TestSuiteType,
          typename TestMethodType>
void assertWontThrow(const std::string &label,
                  TestSuiteType& testSuiteObject,
                  TestMethodType testFunction);
#endif

/** The control class registers tests and controls execution. */
class Controller
{
    UTILCPP_DECLARE_SINGLETON(Controller)

public:
    typedef suite_transferable_ptr (*TestSuiteFactoryFunction)();

    friend void assertTrue(const std::string &label, bool ok);

    template <typename CompareType>
    friend void assertEqual(const std::string& label,
            const CompareType& a, const CompareType& b);

    template <typename CompareType>
    friend void assertNotEqual(const std::string& label,
            const CompareType& a, const CompareType& b);

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
    template <typename ExceptionType>
    friend void assertThrows(const std::string &label,
        std::function<void (void)> testFunction);

    friend void assertWontThrow(const std::string &label,
            std::function<void (void)> testFunction);

#else
    template <class TestSuiteType,
              typename TestMethodType,
              typename ExceptionType>
    friend void assertThrows(const std::string &label,
            TestSuiteType& testSuite, TestMethodType testFunction);

    template <class TestSuiteType,
              typename TestMethodType>
    friend void assertWontThrow(const std::string &label,
            TestSuiteType& testSuite, TestMethodType testFunction);
#endif

    void addTestSuite(const std::string &label, TestSuiteFactoryFunction ffn)
    { _testSuiteFactories[label] = ffn; }

    void setObserver(observer_transferable_ptr v)
    {
#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
        _observer = std::move(v);
#else
        _observer = v;
#endif
    }

    int run();

private:
    observer_scoped_ptr _observer;
    std::map<std::string, TestSuiteFactoryFunction> _testSuiteFactories;

    int _curTestSuite;
    int _curTestSuiteErrs;
    int _allTestErrs;
    int _allTestExcepts;
};

// include assert* implementation after Controller has been declared
#include <testcpp/assert_impl.h>

}

#endif /* TESTCPP_H */
