#ifndef WORDS_HPP
#define WORDS_HPP

#include <array>
#include <map>

#include <string>
#include <string_view>

#include <fstream>

#include <stdexcept>

namespace bip39 {

int const NUM_WORDS = 2048;
using WordList = std::array<std::string, NUM_WORDS>;

class WordListLoader {
public:
    WordListLoader(std::string path_to_datafiles)
        : path_to_datafiles_(std::move(path_to_datafiles))
    {
    }

    WordList const& Load(std::string const& language)
    {
        auto it = word_list_.find(language);
        if (it != std::cend(word_list_)) {
            return it->second;
        }
        auto filepath = path_to_datafiles_ + "/" + (language + ".txt");
        std::ifstream in(filepath);
        if (!in.is_open()) {
            throw std::runtime_error("cannot open data file");
        }
        int i{0};
        WordList list;
        while (!in.eof()) {
            std::string line;
            std::getline(in, line);
            if (!line.empty()) {
                list[i++] = std::move(line);
            }
        }
        word_list_.insert(std::make_pair(language, std::move(list)));
        return word_list_[language];
    }

private:
    std::map<std::string, WordList> word_list_;
    std::string path_to_datafiles_;
};

} // namespace bip39

#endif
