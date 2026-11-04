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
    std::uint8_t check=0;
    bool completed=false;
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
            }return;
        case ParserState::ReadLen:
            if (byte>MAX_PAYLOAD) {
                parser.state= ParserState::MarkByte1;
            }else if (byte==0) {
                parser.check=byte;
                parser.state= ParserState::ReadCheck;
            }else {
                parser.check=byte;
                    parser.expected=byte;
                parser.count=0;
                parser.state= ParserState::ReadData;
            }return;
        case ParserState::ReadData:
            parser.buffer[parser.count]=byte;
            parser.check ^= byte;
            ++parser.count;
            if (parser.count==parser.expected) {
                parser.state= ParserState::ReadCheck;
            }return;
        case ParserState::ReadCheck:
            parser.completed=(byte==parser.check);
            parser.state= ParserState::MarkByte1;
            return;

    }

}

int main() {
    Parser parser;
    std::array<std::uint8_t,8> stream={0xAA, 0x55, 0x04, 0x10, 0x20, 0x30, 0x40, 0x44};
    for (std::size_t i=0;i<stream.size();i++) {
        feed(parser,stream[i]);
        std::cout<<static_cast<int>(parser.state)<<'\n';
    }
    std::cout<<static_cast<int>(parser.buffer[0])<<'\n';
    std::cout<<static_cast<int>(parser.buffer[1])<<'\n';
    std::cout<<static_cast<int>(parser.buffer[2])<<'\n';
    std::cout<<static_cast<int>(parser.buffer[3])<<'\n';
    std::cout<<static_cast<int>(parser.completed)<<'\n';

}