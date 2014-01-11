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

    enum ColorOk { OK };
    enum ColorFail { FAIL };
    enum ColorNormal { NORMAL };

public:

    virtual void flush() = 0;
    virtual TextStreamTestView& operator<< (const std::string& msg) = 0;
    virtual TextStreamTestView& operator<< (int msg) = 0;
    virtual TextStreamTestView& operator<< (EndLine) = 0;
    virtual TextStreamTestView& operator<< (Tab) = 0;

    virtual TextStreamTestView& operator<< (const TextStreamTestView&)
    { return *this; };

    virtual TextStreamTestView& operator<< (ColorOk) { return *this; }
    virtual TextStreamTestView& operator<< (ColorFail) { return *this; }
    virtual TextStreamTestView& operator<< (ColorNormal) { return *this; }

    virtual void onAllTestSuitesBegin(int total)
    { *this << "Start running " << total << " test suites" << END_LINE; }

    virtual void onAllTestSuitesEnd(int lastTestSuiteNum,
            int testSuitesNumTotal, int numErrs, int numExcepts)
    {
        *this << "Did run " << lastTestSuiteNum
              << " of " << testSuitesNumTotal << " total test suites"
              << ", # of errors: " << numErrs
              << ", # of uncaught exceptions: " << numExcepts
              << END_LINE;

        *this << "Test run result: "
              << outputOkOrFail(numExcepts + numErrs == 0)
              << END_LINE;

        flush();
    }

    virtual void onTestSuiteBegin(const std::string& suiteName,
            int num, int total)
    {
        *this << "Test suite '" << suiteName << "' (#" << num
              << "/" << total << "):" << END_LINE;
        _suiteName = suiteName;
    }

    virtual void onTestSuiteEnd(int numErrs)
    {
        *this << TAB << "---" << END_LINE;

        *this << TAB << "Test suite " << outputOkOrFail(numErrs == 0);

        if (numErrs)
            *this << " with " << numErrs << " non-exception errors";

        *this << END_LINE;
    }

    virtual void onTestSuiteEndWithStdException(int numErrs, const std::exception& e)
    {
        outputSeparator();

        *this << TAB << FAIL << "Unhandled exception" << NORMAL;

        outputException(e);

        *this << TAB << "Test suite "
              << FAIL << "FAIL" << NORMAL
              << " due to exception with "
              << numErrs << " non-exception errors" << END_LINE;
    }

    virtual void onTestSuiteEndWithEllipsisException(int numErrs)
    {
        outputSeparator();

        *this << TAB << FAIL << "Unhandled non-standard exception" << NORMAL;

        *this << END_LINE << TAB << "Test suite "
              << FAIL << "FAIL" << NORMAL
              << " due to exception with "
              << numErrs << " non-exception errors" << END_LINE;
    }

    virtual void onAssertBegin(const std::string& testlabel)
    { *this << TAB << "test '" << testlabel << "': ... "; }

    virtual void onAssertExceptionBegin(const std::string& testlabel)
    { *this << TAB << "test exception '" << testlabel << "': ... "; }

    virtual void onAssertNoExceptionBegin(const std::string& testlabel)
    { *this << TAB << "test no exception '" << testlabel << "': ... "; }

    virtual void onAssertEnd(bool ok)
    { *this << outputOkOrFail(ok) << END_LINE; }

    virtual void onAssertExceptionEndWithExpectedException(const std::exception& e)
    { assertExceptionEnd(true, e); }

    virtual void onAssertExceptionEndWithUnexpectedException(const std::exception& e)
    { assertExceptionEnd(false, e); }

    virtual void onAssertExceptionEndWithEllipsisException()
    {
        *this << FAIL << "FAIL" << NORMAL
              << ": unexpected non-standard exception" << END_LINE;
    }

    virtual void onAssertNoExceptionEndWithStdException(const std::exception& e)
    {
        *this << FAIL << "FAIL" << NORMAL
              << ": unexpected exception";

        outputException(e);
    }

    virtual void onAssertNoExceptionEndWithEllipsisException()
    {
        onAssertExceptionEndWithEllipsisException();
    }

protected:

    std::string _suiteName;

private:

    void outputSeparator()
    {
        *this << TAB << "---" << END_LINE;
    }

    void outputException(const std::exception& e)
    {
        const std::string &exceptionType(typeid(e).name());
        *this << " '" << exceptionType << "'";

        const std::string& exceptionMsg(e.what());
        if (!exceptionMsg.empty()) {
            *this << END_LINE;
            outputExceptionMessage(exceptionMsg);
        }

        *this << END_LINE;
    }


    TextStreamTestView& outputOkOrFail(bool ok)
    {
        if (ok)
            *this << OK  << "OK";
        else
            *this << FAIL << "FAIL";

        *this << NORMAL;
        return *this;
    }

    void assertExceptionEnd(bool ok, const std::exception& e)
    {
        *this << outputOkOrFail(ok);

        if (!ok) {
            const std::string &exceptionType(typeid(e).name());
            *this << ": unexpected exception '" + exceptionType + "'";
        }

        *this << END_LINE;
        outputExceptionMessage(e.what());
        *this << END_LINE;
    }

    void outputExceptionMessage(const std::string& exceptionMsg)
    { *this << TAB << TAB << "(message: '" << exceptionMsg << "')"; }

};

}

#endif /* TEXTSTREAMTESTVIEW_H */
