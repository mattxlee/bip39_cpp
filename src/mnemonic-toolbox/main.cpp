#include <iostream>

#include <cxxopts.hpp>

int main(int argc, char *argv[]) {
    cxxopts::Options opts("Mnemonic tool", "A tool to generate mnemonic passphrase");
    opts.add_options()
        ("h,help", "Show help")
        ;
    auto result = opts.parse(argc, argv);
    if (result.count("help") > 0) {
        std::cout << opts.help() << std::endl;
        return 0;
    }
    return 0;
}
