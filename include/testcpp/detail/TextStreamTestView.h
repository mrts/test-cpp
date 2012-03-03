#ifndef TEXTSTREAMTESTVIEW_H__
#define TEXTSTREAMTESTVIEW_H__

#include <testcpp/testcpp.h>

#include <string>

namespace Test
{

/** TextStreamTestView views test messages as a stream of text
 * (as opposed to XML etc). */
class TextStreamTestView: public Observer
{
protected:
    enum EndLine { END_LINE };
    enum Tab { TAB };

public:
    virtual void flush() = 0;
    virtual TextStreamTestView& operator<< (const std::string& msg) = 0;
    virtual TextStreamTestView& operator<< (int msg) = 0;
    virtual TextStreamTestView& operator<< (EndLine) = 0;
    virtual TextStreamTestView& operator<< (Tab) = 0;

    virtual void onTestBegin(const std::string& suiteName,
            int num, int total)
    {
        *this << "Test suite '" << suiteName << "' (#" << num
            << "/" << total << "):" << END_LINE;
        _suiteName = suiteName;
    }

    virtual void onTestEnd(int numErrs,
            const std::string &exceptionType,
            const std::string &exceptionMsg)
    {
        if (! exceptionType.empty()) {
            *this << TAB << "Unhandled exception '"
                << exceptionType << "'";
            if (!exceptionMsg.empty())
                *this << " with message: " << exceptionMsg;
            *this << END_LINE << TAB << "FAIL due to exception with "
                << numErrs << " non-exception errors"
                << END_LINE;
            return;
        }

        if (numErrs == 0)
            *this << TAB << "OK";
        else
            *this << TAB << "FAIL with " << numErrs
                << " non-exception errors";
        *this << END_LINE;
    }

    virtual void onAllTestsEnd(int numTests, int numErrs, int numExcept)
    {
        *this << "Total test suites run: " << numTests
            << ", # of errors: " << numErrs
            << ", # of uncaught exceptions: " << numExcept
            << END_LINE;

        flush();
    }

    virtual void onAssertBegin(const std::string& testlabel)
    { *this << TAB << "test '" << testlabel << "': ... "; }

    virtual void onAssertExceptionBegin(const std::string& testlabel)
    { *this << TAB << "test exception '" << testlabel << "': ... "; }

    virtual void onAssertNoExceptionBegin(const std::string& testlabel)
    { *this << TAB << "test no exception '" << testlabel << "': ... "; }

    virtual void onAssertEnd(bool ok)
    { *this << (ok ? "OK" : "FAIL") << END_LINE; }

    virtual void onAssertExceptionEnd(bool ok, const std::string& exceptionMsg)
    {
        *this << (ok ? "OK"
                : (std::string("FAIL with message: ") + exceptionMsg))
            << END_LINE;
    }

protected:
    std::string _suiteName;
};

}

#endif /* TEXTSTREAMTESTVIEW_H */
