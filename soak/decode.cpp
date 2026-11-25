#include <iostream>
#include <fstream>
#include <cstdint>
#include "parser.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "usage: decode <file>\n";
        return 1;
    }
    std::ifstream in(argv[1], std::ios::binary);
    if (!in) {
        std::cout << "cannot open " << argv[1] << '\n';
        return 1;
    }
    Parser parser;
    int good = 0;
    int rejected = 0;
    long bytes = 0;
    char c;
    while (in.get(c)) {
        FeedResult r = feed(parser, static_cast<std::uint8_t>(c));
        ++bytes;
        if (r == FeedResult::Packet) {
            ++good;
        }
        if (r == FeedResult::Rejected) {
            ++rejected;
        }
    }
    std::cout << "bytes: " << bytes << '\n';
    std::cout << "good: " << good << '\n';
    std::cout << "rejected: " << rejected << '\n';
    return 0;
}