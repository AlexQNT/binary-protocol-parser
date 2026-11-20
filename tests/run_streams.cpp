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
void check(int& failures, const char* label, const std::vector<std::uint8_t>& stream, int expected) {
    int got = run(stream);
    if (got == expected) {
        std::cout << "PASS " << label << '\n';
    } else {
     std::cout << "FAIL " << label << " expected " << expected << " got " << got << '\n';
        ++failures;
    }
}

 int main() {
    int failures = 0;
    check(failures, "A", {0xAA, 0x55, 0x04, 0x10, 0x20, 0x30, 0x40, 0x44}, 1);
    check(failures, "B", {0xAA, 0x55, 0x04, 0x10, 0x20, 0x30, 0x40, 0x45}, 0);
    check(failures, "L1", {0xAA, 0x55, 0x01, 0x05, 0x04}, 1);
    std::cout << failures << " failed\n";
    if (failures == 0) {
        return 0;
    }
    return 1;
}
