#include <iostream>
#include <string>

#include <cxxopts.hpp>

#include "random.h"
#include "toolbox.h"
#include "mnemonic.h"

int Run_GenNewMnemonic(int num_sentences, std::string_view lang)
{
    // Generate a new mnemonic sentences
    if (!bip39::Mnemonic::IsValidNumMnemonicSentences(num_sentences)) {
        throw std::runtime_error("invalid number of mnemonic sentences");
    }

    int bits_ent = bip39::Mnemonic::GetEntBitsByNumMnemonicSentences(num_sentences);
    int num_bytes = bits_ent / 8;
    bip39::RandomBytes rnd(num_bytes);
    auto ent = rnd.Random();

    bip39::Mnemonic mnemonic(ent, std::string(lang));
    auto word_list = mnemonic.GetWordList();
    std::string mnemonic_sentences = bip39::GenerateWords(word_list, bip39::GetDelimiterByLang(lang));
    std::cout << mnemonic_sentences << std::endl;
    return 0;
}

int main(int argc, char *argv[])
{
    cxxopts::Options opts("Mnemonic tool", "A tool to generate mnemonic passphrase");
    opts.add_options()
        ("h,help", "Show help")
        ("n,num", "The number of the new mnemonic sentences", cxxopts::value<int>()->default_value("24"))
        ("l,lang", "The language of the new mnemonic", cxxopts::value<std::string>()->default_value("english"))
        ;
    auto result = opts.parse(argc, argv);
    if (result.count("help") > 0) {
        std::cout << opts.help() << std::endl;
        return 0;
    }
    auto num_sentences = result["num"].as<int>();
    auto lang = result["lang"].as<std::string>();
    int res{0};
    try {
        res = Run_GenNewMnemonic(num_sentences, lang);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return res;
}
