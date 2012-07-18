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

    virtual void onTestBegin(const std::string& suiteName,
            int num, int total)
    {
        *this << "Test suite '" << suiteName << "' (#" << num
              << "/" << total << "):" << END_LINE;
        _suiteName = suiteName;
    }

    virtual void onTestEnd(int numErrs,
            const std::string &exceptionMsg,
            const std::string &exceptionType)
    {
        *this << TAB << "---" << END_LINE;

        if (!exceptionType.empty()) {

            *this << TAB << FAIL << "Unhandled exception" << NORMAL
                  << " '" << exceptionType << "'";

            if (!exceptionMsg.empty())
                *this << " with message: '" << exceptionMsg << "'";

            *this << END_LINE << TAB << "Test suite "
                  << FAIL << "FAIL" << NORMAL
                  << " due to exception with "
                  << numErrs << " non-exception errors" << END_LINE;

            return;
        }

        *this << TAB << "Test suite " << outputOkOrFail(numErrs == 0);

        if (numErrs)
            *this << " with " << numErrs << " non-exception errors";

        *this << END_LINE;
    }

    virtual void onAllTestsEnd(int numTests, int numErrs, int numExcept)
    {
        *this << "Total test suites run: " << numTests
              << ", # of errors: " << numErrs
              << ", # of uncaught exceptions: " << numExcept
              << END_LINE;

        *this << "Test run result: "
              << outputOkOrFail(numExcept + numErrs == 0)
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
    {
        *this << outputOkOrFail(ok) << END_LINE;
    }

    virtual void onAssertExceptionEnd(bool ok,
            const std::string &exceptionMsg,
            const std::string &exceptionType)
    {
        *this << outputOkOrFail(ok);

        if (!ok)
            *this << ": unexpected exception '" + exceptionType + "'";

        *this << END_LINE << TAB << TAB
              << "(message: '" << exceptionMsg << "')" << END_LINE;
    }

protected:

    std::string _suiteName;

private:

    TextStreamTestView& outputOkOrFail(bool ok)
    {
		if (ok)
			*this << OK  << "OK";
		else
			*this << FAIL << "FAIL";

		*this << NORMAL;
		return *this;
	}
};

}

#endif /* TEXTSTREAMTESTVIEW_H */
