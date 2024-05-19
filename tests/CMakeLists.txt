find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(utils/)
add_executable(end-to-end-test end-to-end/test.cpp)

target_link_libraries(end-to-end-test PRIVATE ${GTEST_LIBRARIES})

include(GoogleTest)
gtest_discover_tests(end-to-end-test)
