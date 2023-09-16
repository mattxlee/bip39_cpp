# BIP39 C++ implementation

Another BIP39 implementation written in C++.

[BIP039](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki)

Quote from BIP039 website:

This BIP describes the implementation of a mnemonic code or mnemonic sentence -- a group of easy to remember words -- for the generation of deterministic wallets.

## Dependencies

### Tools need to be installed

* CMake - You need to install CMake 3.5 or higher version in order to generate the compile script.

* unzip - Install unzip allow vcpkg download and extract files

* pkg-config - This tool is also required by Vcpkg.

### Download this repo and initialize

Vcpkg is added as a submodule to manage required libraries, you need to initialize the submodules after the repo is cloned. You also need to initialize vcpkg from the dir. Following the instructions below to do the initialization.

```bash
git clone https://github.com/mattxlee/bip39_cpp && cd bip39_cpp && git submodule update --init # initialize git repo
cd vcpkg && ./bootstrap-vcpkg.sh # initialize vcpkg
```

## Components included

* A library - Linking with this library to build your own mnemonic tool.

* Test cases - All test cases are copied from BIP39 official website, and they should pass.

* Mnemonic sentences generating tool - Provide a tool can generate new mnemonic sentences with different lengths or languages.

## Compiling instructions

Before you do this step, the vcpkg should be initialized successfully and all the required tools are installed as well.

1. Clone this repo: `git clone https://github.com/mattxlee/bip39_cpp`

2. Create build dir inside the project root: `mkdir build && cd build`

3. Prepare Makefile in easy way: `cmake ..`

The script will will not build the binary of test cases in default, you can add arg: `-DBUILD_TEST=1` to prpeare the build script. After the compiling procedure, the test program should be ready from this path `build/bip39_test`. Run the program directly or run `make test` you should get the test result.

4. Run this command: `make` to start the compiling procedure, and the required binaries should be ready after the compiling procedure is finished without any error.

## How to use the mnemonic generating tool

Combine the arg `-DBUILD_MNTOOL=1` to prepare the build script before starting the compiling procedure. The binary file of mnemonic generating tool should be ready from the path `build/mntool`.

* Simply run commmand: `build/mntool` to generate a new mnemonic 24 sentences in english.

* Specify the number of sentences by adding arg: `-n [number]`. For example: run `build/mntool -n 12` will generate a mnemonic with 12 sentences.

* Specify the language name of the new mnemonic by adding arg: `-l [lang name]`, the default language name is `english`.

* Check help doc by adding arg: `-h`, you can find all supported language names from the help doc.

## How to use the library

The main class to convert between entropy and words is `Mnemonic`, the class is included by `src/mnemonic.hpp`. An object of `Mnemonic` can also create a seed with a passphrase string.

### Create Mnemonic object from words

Construct an object of class `Mnemonic` with `word list` and `language name`

```C++
Mnemonic(WordList const& word_list, std::string lang)
```

* The number of words must equals to 12, 15, 18, 21 or 24, otherwise the constructor will throw an exception

* There 10 languages the library currently support, those are: chinese_simplified, chinese_traditional, czech, english, french, italian, japanese, korean, portuguese, spanish. The constructor will throw an exception when the provided argument `lang` is not one of them.

### Create Mnemonic object from entropy

```C++
Mnemonic(std::vector<uint8_t> entropy, std::string lang)
```

* The number of entropy must equals to 128, 160, 192, 224, 256. or the constructor will fail and throw an exception

* The language name must be valid otherwise an exception will be thrown.

### Create seed with passphrase

```C++
std::vector<uint8_t> CreateSeed(std::string_view passphrase)
```

The method `Mnemonic::CreateSeed` will use current entropy(mnemonic words) to create a seed with a passphrase string. You also can pass an empty string as the passphrase, but we strongly suggest a stronger passphrase should be passed to this method. A 512-bit(64 bytes) seed buffer will be returned.

### Entropy and words

Once the mnemonic object is created, entropy and words are both valid, you can retrieve these two properties by calling the following methods:

```C++
WordList const& GetWordList() const
```

```C++
std::vector<uint8_t> const& GetEntropyData() const
```

## Test

All the test cases are downloaded from [https://github.com/trezor/python-mnemonic/blob/master/vectors.json](https://github.com/trezor/python-mnemonic/blob/master/vectors.json) and it has been converted into C++ source file `src/test.cpp`
