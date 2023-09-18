cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")

project(bip39_cpp VERSION 0.5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_MNTOOL "build mnemonic-toolbox binary" OFF)
option(BUILD_TEST "generate test binaries" OFF)

find_package(unofficial-utf8proc CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

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
target_link_libraries(bip39_cpp PRIVATE utf8proc OpenSSL::Crypto OpenSSL::SSL)
target_include_directories(bip39_cpp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${utf8proc_SOURCE_DIR}
    ${OpenSSL_INCLUDE_DIR}
)

# bip39 test
if (BUILD_TEST)
    find_package(GTest CONFIG REQUIRED)
    set(TEST_SRCS
        ./src/test/test.cpp
    )
    add_executable(bip39_test ${TEST_SRCS})
    target_include_directories(
        bip39_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${googletest_SOURCE_DIR}/googletest/include
    )
    target_link_libraries(bip39_test PRIVATE bip39_cpp GTest::gtest GTest::gtest_main)

    enable_testing()
    add_test(NAME BIP39Test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/bip39_test)
endif()

# bip39 tool-box
if (BUILD_MNTOOL)
    find_package(cxxopts CONFIG REQUIRED)
    set(MNEMONICTOOL_SRC ./src/mnemonic-toolbox/main.cpp)
    add_executable(mntool ${MNEMONICTOOL_SRC})
    target_include_directories(mntool PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(mntool PRIVATE bip39_cpp cxxopts::cxxopts)
endif()
