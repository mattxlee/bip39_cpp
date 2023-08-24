#ifndef TOOLBOX_HPP
#define TOOLBOX_HPP

#include <cstdint>

#include <stdexcept>

#include <string>
#include <string_view>

#include <fstream>

#include <json/value.h>
#include <json/reader.h>

inline uint8_t CharToInt(char ch)
{
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    } else if (ch >= 'a' && ch <= 'f') {
        return ch - 'a' + 0x0a;
    }
    throw std::runtime_error("invalid hex char");
}

inline std::vector<uint8_t> ParseHex(std::string_view hex)
{
    if (hex.size() % 2 != 0) {
        throw std::runtime_error("invalid length of the incoming hex string");
    }
    std::vector<uint8_t> res;
    std::string str{hex};
    uint8_t val{0};
    bool paired{false};
    for (auto i = std::begin(str); i != std::end(str); ++i) {
        *i = std::tolower(*i);
        val += CharToInt(*i);
        if (paired) {
            res.push_back(val);
            val = 0;
            paired = false;
        } else {
            val <<= 4;
            paired = true;
        }
    }
    return res;
}

inline std::vector<std::string> ParseWords(std::string_view words, char delimiter = ' ')
{
    std::vector<std::string> res;
    int p{0};
    int f = words.find(delimiter);
    while (f != std::string_view::npos) {
        res.push_back(std::string(words.substr(p, f - p)));
        p = f + 1;
        f = words.find(delimiter, p);
    }
    res.push_back(std::string(words.substr(p)));
    return res;
}

inline Json::Value ReadTestJsonFile(std::string_view filepath)
{
    std::ifstream in(filepath);
    if (!in.is_open()) {
        throw std::runtime_error("cannot open test json to read");
    }
    in.seekg(0, std::ios::end);
    auto len = in.tellg();
    std::shared_ptr<char> pcontent(new char[len], [](char* p) { delete[] p; });
    in.seekg(0);
    in.read(pcontent.get(), len);
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    std::string json_str = pcontent.get();
    Json::Value root;
    std::string errs;
    if (!reader->parse(json_str.c_str(), json_str.c_str() + json_str.size(), &root, &errs)) {
        // throw errors
        throw std::runtime_error("cannot parse json string");
    }
    if (!root.isObject()) {
        throw std::runtime_error("wrong type of the json root");
    }
    return root;
}

#endif
