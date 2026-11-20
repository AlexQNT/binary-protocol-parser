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
    check(failures, "C", {0xAA, 0x55, 0x04, 0x10, 0x21, 0x30, 0x40, 0x44}, 0);
    check(failures, "D", {0xAA, 0x55, 0x00, 0x00}, 1);
    check(failures, "E", {0xAA, 0x55, 0x20, 0x10, 0x20, 0x30}, 0);
    check(failures, "F", {0x3C, 0x91, 0xAA, 0x55, 0x01, 0x7E, 0x7F}, 1);
    check(failures, "G", {0xAA, 0x55, 0x04, 0xAA, 0x55, 0x01, 0x02, 0xF8}, 1);
    check(failures, "H", {0xAA, 0x55, 0x04, 0x10, 0x20, 0xAA, 0x55, 0x02, 0x0F, 0xF0, 0xFD}, 0);
    check(failures, "I", {0xAA, 0x55, 0x01, 0x05, 0x04, 0xAA, 0x55, 0x01, 0x07, 0x06}, 2);
    check(failures, "J", {0xAA, 0x55, 0x01, 0x05, 0x04, 0xAA, 0x55, 0x01, 0x07, 0x00}, 1);
    std::cout << failures << " failed\n";
    if (failures == 0) {
        return 0;
    }
    return 1;
}
