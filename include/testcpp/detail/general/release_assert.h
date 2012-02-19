#ifndef TESTCPP_RELEASE_ASSERT_H__
#define TESTCPP_RELEASE_ASSERT_H__

#include <stdexcept>
#include <string>

#define TESTCPP_RELEASE_ASSERT(cond, msg) \
    if (!(cond)) { \
        std::string fn(__FUNCTION__); \
        std::string m(msg);\
        std::string c(#cond); \
        throw std::logic_error(fn + ": " + c + ": " + m); \
    } else (void) 0

#endif /* TESTCPP_RELEASE_ASSERT_H */
