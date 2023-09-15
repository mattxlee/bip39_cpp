cmake_minimum_required(VERSION 3.5)

project(bip39_cpp VERSION 0.5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_MNTOOL "build mnemonic-toolbox binary" OFF)
option(BUILD_OPENSSL "download and build openssl from github" OFF)
option(BUILD_TEST "generate test binaries" OFF)

include(FetchContent)

FetchContent_Declare(
    utf8proc
    GIT_REPOSITORY https://github.com/JuliaStrings/utf8proc
    GIT_TAG v2.8.0
)

if (BUILD_MNTOOL)
    FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts
        GIT_TAG v3.1.1
    )
    FetchContent_MakeAvailable(cxxopts)
endif()

if (BUILD_TEST)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
endif()

FetchContent_MakeAvailable(utf8proc)

if (BUILD_OPENSSL)
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
else()
    find_package(OpenSSL REQUIRED)
    set(OpenSSL_CRYPTO_LIB OpenSSL::Crypto)
    set(OpenSSL_SSL_LIB OpenSSL::SSL)
    include_directories(/usr/local/opt/openssl/include)
endif()

# bip39 library
set(BIP39_CPP_SRC
    ./src/bit_opts.cpp
    ./src/byteswap.cpp
    ./src/mnemonic.cpp
    ./src/random.cpp
    ./src/sha256.cpp
    ./src/toolbox.cpp
    ./src/langs.cpp
)
add_library(bip39_cpp ${BIP39_CPP_SRC})
if (BUILD_OPENSSL)
    add_dependencies(bip39_cpp openssl)
endif()
target_link_libraries(bip39_cpp PRIVATE utf8proc ${OpenSSL_CRYPTO_LIB} ${OpenSSL_SSL_LIB})
target_include_directories(bip39_cpp PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${utf8proc_SOURCE_DIR}
    ${OpenSSL_INCLUDE_DIR}
    ${tinyformat_SOURCE_DIR}
)

# bip39 test
if (BUILD_TEST)
    set(TEST_SRCS
        ./src/test/test.cpp
    )
    add_executable(bip39_test ${TEST_SRCS})
    target_include_directories(
        bip39_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${googletest_SOURCE_DIR}/googletest/include
    )
    target_link_libraries(bip39_test PRIVATE bip39_cpp gtest gtest_main)

    enable_testing()
    add_test(NAME BIP39Test COMMAND ${CMAKE_BINARY_DIR}/bip39_test)
endif()

# bip39 tool-box
if (BUILD_MNTOOL)
    set(MNEMONICTOOL_SRC ./src/mnemonic-toolbox/main.cpp)
    add_executable(mntool ${MNEMONICTOOL_SRC})
    target_include_directories(mntool PRIVATE ${CMAKE_SOURCE_DIR}/include ${cxxopts_SOURCE_DIR}/include)
    target_link_libraries(mntool PRIVATE bip39_cpp)
endif()
