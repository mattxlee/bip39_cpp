# BIP39 C++ implementation

Another BIP39 implementation written in C++

[BIP039](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki)

## Dependencies

These libraries will be downloaded during the compiling procedure automatically.

* OpenSSL

* utf8proc

## How to use

The main class to convert between entropy and words is `Mnemonic`, the class is included by `src/mnemonic.hpp`. An object of `Mnemonic` can also create a seed with a passphrase string.

### Create Mnemonic object from words

Construct object of class `Mnemonic` with `word list` and `language name`

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
