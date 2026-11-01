#include <iostream>
#include <cstdint>
#include <cstddef>
#include <array>


enum class ParserState{MarkByte1, MarkByte2, ReadLen, ReadData, ReadCheck};

constexpr std::size_t MAX_PAYLOAD=16;
struct Parser {
    ParserState state = ParserState::MarkByte1;
    std::uint8_t expected=0;
    std::uint8_t count=0;
    std::array<std::uint8_t,MAX_PAYLOAD> buffer{};
};



void feed(Parser& parser, std::uint8_t byte) {
    switch(parser.state) {
        case ParserState::MarkByte1:
            if (byte==0xAA) {
                parser.state= ParserState::MarkByte2;
            }else {
                parser.state= ParserState::MarkByte1;
            } return;
        case ParserState::MarkByte2:
            if (byte==0x55) {
                parser.state= ParserState::ReadLen;
            }else if (byte==0xAA) {
                parser.state= ParserState::MarkByte2;
            }else {
                parser.state= ParserState::MarkByte1;
            }
            return;
    }

}

int main() {
    Parser parser;
    constexpr std::uint8_t first  = 0x3C;
    constexpr std::uint8_t second = 0xAA;
    constexpr std::uint8_t third  = 0x3C;
    constexpr std::uint8_t fourth = 0xAA;
    constexpr std::uint8_t fifth = 0x55;
   feed(parser,first);
    std::cout<<static_cast<int>(parser.state)<<'\n';
    feed(parser,second);
    std::cout<<static_cast<int>(parser.state)<<'\n';
    feed(parser,third);
    std::cout<<static_cast<int>(parser.state)<<'\n';
    feed(parser,fourth);
    std::cout<<static_cast<int>(parser.state)<<'\n';
    feed(parser,fifth);
std::cout<<static_cast<int>(parser.state)<<'\n';
}