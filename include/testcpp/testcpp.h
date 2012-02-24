/**
 * A simple yet complete testing framework.
 *
 * Copyright (c) 2007, 2011, 2012 Mart Sõmermaa.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the MIT Licence as published by the Open Source
 * Initiative group on the web at
 * http://opensource.org/licenses/MIT
 *
 * Version: 3.0.0
 */

#ifndef TESTCPP_H__
#define TESTCPP_H__

#include <utilcpp/scoped_ptr.h>
#include <utilcpp/declarations.h>

#include <string>
#include <memory>
#include <map>


namespace Test
{

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
    static std::auto_ptr<Suite> instance()
    {
        return std::auto_ptr<Suite>(new ConcreteSuiteType());
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
    virtual void onTestBegin(const std::string& testlabel,
            int testNum, int total) = 0;

    virtual void onTestEnd(int numErrs,
            const std::string& exceptionType = "",
            const std::string& exceptionMsg = "") = 0;

    virtual void onAssertBegin(const std::string& testlabel) = 0;
    virtual void onAssertExceptionBegin(const std::string& testlabel) = 0;
    virtual void onAssertNoExceptionBegin(const std::string& testlabel) = 0;

    virtual void onAssertExceptionEnd(bool ok,
            const std::string& exceptionMsg = "") = 0;
    virtual void onAssertEnd(bool ok) = 0;

    virtual void onAllTestsEnd(int numTests, int numErrs, int numExcepts) = 0;
};


void assertTrue(const std::string& label, bool ok);

template <typename CompareType>
void assertEqual(const std::string& label,
        const CompareType& a, const CompareType& b);

void assertWontThrow(const std::string& label, void (*testfn)());

template <typename ExceptionType>
void assertThrows(const std::string& label, void (*testfn)());

/** The control class registers tests and controls execution. */
class Controller
{
    UTILCPP_DECLARE_SINGLETON(Controller)

public:
    typedef std::auto_ptr<Suite> (*TestSuiteFactoryFunction)();

    friend void assertTrue(const std::string &label, bool ok);

    template <typename CompareType>
    friend void assertEqual(const std::string& label,
            const CompareType& a, const CompareType& b);

    friend void assertWontThrow(const std::string &label, void (*testfn)());

    template <typename T>
    friend void assertThrows(const std::string &label, void (*testfn)());

    void addTestSuite(const std::string &label, TestSuiteFactoryFunction ffn)
    { _testFuncs[label] = ffn; }

    void setObserver(std::auto_ptr<Observer> v)
    { _observer = v; }

    int run();

private:
    utilcpp::scoped_ptr<Observer> _observer;
    std::map<std::string, TestSuiteFactoryFunction> _testFuncs;

    int _curTest;
    int _curTestErrs;
    int _allTestErrs;
    int _allTestExcepts;
};


template <typename ExceptionType>
void assertThrows(const std::string &label, void (*testfn)())
{
    Controller& c = Controller::instance();

    c._observer->onAssertExceptionBegin(label);

    try {
        testfn();
    } catch (const ExceptionType& e) {
        c._observer->onAssertExceptionEnd(true, e.what());
        return;
    }

    ++c._curTestErrs;
    c._observer->onAssertEnd(false);
}

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

}

#endif /* TESTCPP_H */
