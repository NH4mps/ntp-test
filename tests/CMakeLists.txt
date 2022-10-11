ENABLE_TESTING()

ADD_EXECUTABLE(DealsClosingTest 
    DealsClosingTest.cpp
    ${SERVER_SOURCE_DIR}/Responds.cpp
    ${SERVER_SOURCE_DIR}/Responds.hpp)
TARGET_LINK_LIBRARIES(DealsClosingTest PRIVATE 
    GTest::gtest_main sqlite_orm::sqlite_orm)
target_include_directories(DealsClosingTest PRIVATE 
    ${SERVER_SOURCE_DIR} ${COMMON_DIR})

INCLUDE(GoogleTest)
gtest_discover_tests(DealsClosingTest)
