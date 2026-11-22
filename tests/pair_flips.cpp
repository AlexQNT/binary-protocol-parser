#include <iostream>
#include <vector>
#include <cstdint>
#include <cstddef>
#include "parser.h"

int run(const std::vector<std::uint8_t>& stream) {
    Parser parser;
    int accepted=0;
    for (std::size_t i = 0; i < stream.size(); ++i) {
        FeedResult result = feed(parser, stream[i]);
        if (result == FeedResult::Packet) {
            ++accepted;
        }
    }
    return accepted;
}

int main() {
    std::vector<std::uint8_t> good = {0xAA, 0x55, 0x01, 0x05, 0x04};
    std::size_t total_bits = good.size() * 8;
    int pairs = 0;
    int accepted = 0;
    for (std::size_t a = 16; a < total_bits; ++a) {
        for (std::size_t b = a + 1; b < total_bits; ++b) {
            std::vector<std::uint8_t> bad = good;
            bad[a / 8] ^= static_cast<std::uint8_t>(1 << (a % 8));
            bad[b / 8] ^= static_cast<std::uint8_t>(1 << (b % 8));
            ++pairs;
            if (run(bad) == 1) {
                ++accepted;
            }
        }
    }
    std::cout << "pairs: " << pairs << '\n';
    std::cout << "accepted: " << accepted << '\n';
    return 0;
}