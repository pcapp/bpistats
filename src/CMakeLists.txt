file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/example.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(stats stats.cpp bpistats.cpp)