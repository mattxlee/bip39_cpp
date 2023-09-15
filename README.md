# BIP39 C++ implementation

Another BIP39 implementation written in C++.

[BIP039](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki)

Quote from BIP039 website:

This BIP describes the implementation of a mnemonic code or mnemonic sentence -- a group of easy to remember words -- for the generation of deterministic wallets.

## Dependencies

The build script is written in CMake, it is required before compiling bip39_cpp. The following libraries might be downloaded during the compiling procedure.

* utf8proc

* OpenSSL

* Google test (optional)

* cxxopts (optional)

## Components included

* A library - Linking with this library to build your own mnemonic tool.

* Test cases - All test cases are copied from BIP39 official website, and they should pass.

* Mnemonic sentences generating tool - Provide a tool can generate new mnemonic sentences with different lengths or languages.

## Compiling instructions

1. Clone this repo: `git clone https://github.com/mattxlee/bip39_cpp`

2. Create build dir inside the project root: `mkdir build && cd build`

3. Prepare Makefile in easy way: `cmake ..`

You need to ensure OpenSSL is installed on your system, or you can add arg: `-DBUILD_OPENSSL=1` to download OpenSSL during the make procedure. In default, it will not build the binary of test cases, you can add arg: `-DBUILD_TEST=1` to build it. You also can build the mnemonic generating tool by adding `-DBUILD_MNTOOL=1`.

4. Build: `make`

## How to use the mnemonic generating tool

Combine the arg `-DBUILD_MNTOOL=1` to generate the binary file of mnemonic generating tool.

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
