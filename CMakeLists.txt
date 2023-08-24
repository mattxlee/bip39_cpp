cmake_minimum_required(VERSION 3.5)

project(bip39_cpp VERSION 0.5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(TEST_SRCS ./src/test.cpp)

include(FetchContent)

find_package(OpenSSL REQUIRED)
find_package(unofficial-utf8proc CONFIG REQUIRED)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.14.0
)

FetchContent_MakeAvailable(googletest)

add_executable(bip39_test ${TEST_SRCS})
target_include_directories(
    bip39_test PRIVATE
    ./include
    ${tinyformat_SOURCE_DIR}
    ${googletest_SOURCE_DIR}/googletest/include
)
target_link_libraries(bip39_test PRIVATE gtest gtest_main OpenSSL::SSL OpenSSL::Crypto utf8proc)
