#ifndef WORDS_HPP
#define WORDS_HPP

#include <vector>
#include <map>

#include <string>
#include <string_view>

#include <fstream>

#include <stdexcept>

namespace bip39 {

using WordList = std::vector<std::string>;

class WordListLoader {
public:
    WordListLoader(std::string path_to_datafiles)
        : path_to_datafiles_(std::move(path_to_datafiles))
    {
    }

    WordList const& Load(std::string const& lang)
    {
        auto it = word_list_.find(lang);
        if (it != std::cend(word_list_)) {
            return it->second;
        }
        auto filepath = path_to_datafiles_ + "/" + (lang + ".txt");
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
                list.push_back(std::move(line));
            }
        }
        word_list_.insert(std::make_pair(lang, std::move(list)));
        return word_list_[lang];
    }

private:
    std::map<std::string, WordList> word_list_;
    std::string path_to_datafiles_;
};

} // namespace bip39

#endif
