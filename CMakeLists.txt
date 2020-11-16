cmake_minimum_required(VERSION 3.0.0)
project(dbtest VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(submodules/soci)
include_directories(${SOCI_CORE_INCLUDE_DIRS} ${SOCI_POSTGRESQL_INCLUDE_DIRECTORIES} ${CMAKE_BINARY_DIR}/submodules/soci/include)

add_executable(dbtest main.cpp)
target_link_libraries(dbtest soci_core soci_postgresql dl pq)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)