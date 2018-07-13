find_package(GTest REQUIRED)

add_executable(test_all TestAll.cpp ${SRC_DIR}/bpistats.cpp)

target_include_directories(test_all PRIVATE ${SRC_DIR})
target_include_directories(test_all PRIVATE ${GTEST_INCLUDE_DIRS})

target_link_libraries(test_all PUBLIC ${GTEST_BOTH_LIBRARIES})