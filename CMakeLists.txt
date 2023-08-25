cmake_minimum_required(VERSION 3.5)

project(bip39_cpp VERSION 0.5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(TEST_SRCS ./src/test.cpp)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.14.0
)

FetchContent_Declare(
    utf8proc
    GIT_REPOSITORY https://github.com/JuliaStrings/utf8proc
    GIT_TAG v2.8.0
)

FetchContent_MakeAvailable(googletest utf8proc)

include(ExternalProject)

ExternalProject_Add(
    openssl
    PREFIX ${CMAKE_BINARY_DIR}/openssl
    GIT_REPOSITORY https://github.com/openssl/openssl
    GIT_TAG openssl-3.1.2
    BUILD_IN_SOURCE true
    CONFIGURE_COMMAND "./Configure"
    INSTALL_COMMAND ""
)
ExternalProject_Get_Property(openssl SOURCE_DIR)
set(OpenSSL_INCLUDE_DIR "${SOURCE_DIR}/include")
set(OpenSSL_CRYPTO_LIB "${SOURCE_DIR}/libcrypto.a")
set(OpenSSL_SSL_LIB "${SOURCE_DIR}/libssl.a")

add_executable(bip39_test ${TEST_SRCS})
add_dependencies(bip39_test openssl)
target_include_directories(
    bip39_test PRIVATE
    ./include
    ${OpenSSL_INCLUDE_DIR}
    ${tinyformat_SOURCE_DIR}
    ${googletest_SOURCE_DIR}/googletest/include
)
target_link_libraries(bip39_test PRIVATE gtest gtest_main utf8proc ${OpenSSL_CRYPTO_LIB} ${OpenSSL_SSL_LIB})

enable_testing()
add_test(NAME BIP39Test COMMAND ${CMAKE_BINARY_DIR}/bip39_test)
