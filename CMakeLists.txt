CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
SET (CMAKE_BACKWARDS_COMPATIBILITY 2.6)

PROJECT(testcpp)

# --- libtestcpp

INCLUDE_DIRECTORIES("include")

FILE(GLOB LIBTESTCPP_SRC
	include/testcpp/[^.]*.h
	include/testcpp/detail/[^.]*.h
	include/utilcpp/[^.]*.h
	src/[^.]*.cpp)

ADD_LIBRARY(${PROJECT_NAME} STATIC ${LIBTESTCPP_SRC})

# --- testcpp-test

FILE (GLOB SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        test/src/[^.]*.cpp)

ADD_EXECUTABLE(${PROJECT_NAME}-test ${SRC})

TARGET_LINK_LIBRARIES(${PROJECT_NAME}-test ${PROJECT_NAME})

IF(WIN32)

  ADD_DEFINITIONS("/W4")

ELSE(WIN32)

  SET(CMAKE_CXX_COMPILER clang++)
  ADD_DEFINITIONS("-Wall -Werror -ferror-limit=5")

ENDIF(WIN32)
