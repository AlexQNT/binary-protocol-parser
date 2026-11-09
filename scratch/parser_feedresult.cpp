#include <iostream>
#include <cstdint>
#include <cstddef>
#include <array>
#include <vector>


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

enum class FeedResult{Pending,Packet, Rejected};

FeedResult feed(Parser& parser, std::uint8_t byte) {
    switch(parser.state) {
        case ParserState::MarkByte1:
            if (byte==0xAA) {
                parser.state= ParserState::MarkByte2;
            }else {
                parser.state= ParserState::MarkByte1;
            } return FeedResult::Pending;
        case ParserState::MarkByte2:
            if (byte==0x55) {
                parser.state= ParserState::ReadLen;
            }else if (byte==0xAA) {
                parser.state= ParserState::MarkByte2;
            }else {
                parser.state= ParserState::MarkByte1;
            }return FeedResult::Pending;
        case ParserState::ReadLen:
            if (byte>MAX_PAYLOAD) {
                parser.state= ParserState::MarkByte1;
                return FeedResult::Rejected;
            }else if (byte==0) {
                parser.check=byte;
                parser.state= ParserState::ReadCheck;
                return FeedResult::Pending;
            }else {
                parser.check=byte;
                    parser.expected=byte;
                parser.count=0;
                parser.state= ParserState::ReadData;
                return FeedResult::Pending;
            }
        case ParserState::ReadData:
            parser.buffer[parser.count]=byte;
            parser.check ^= byte;
            ++parser.count;
            if (parser.count==parser.expected) {
                parser.state= ParserState::ReadCheck;
            }return FeedResult::Pending;
        case ParserState::ReadCheck:
            parser.completed=(byte==parser.check);
            parser.state= ParserState::MarkByte1;
            if (parser.completed) {
                return FeedResult::Packet;
            }else {
                return FeedResult::Rejected;
            }

    }return FeedResult::Pending;

}

bool run(const std::vector<std::uint8_t>& stream) {
    Parser parser;
    for (std::size_t i = 0; i < stream.size(); ++i) {
        feed(parser, stream[i]);
    }
    return parser.completed;
}

int main() {
    std::cout<<"A:"<< run(std::vector<std::uint8_t>{0xAA, 0x55, 0x04, 0x10, 0x20, 0x30, 0x40, 0x44})<<'\n';
    std::cout <<"B:"<< run(std::vector<std::uint8_t>{0xAA, 0x55, 0x04, 0x10, 0x20, 0x30, 0x40, 0x45})<<'\n';
    std::cout<<"C:"<< run(std::vector<std::uint8_t>{0xAA, 0x55, 0x04, 0x10, 0x21, 0x30, 0x40, 0x44})<<'\n';
    std::cout <<"D:"<< run(std::vector<std::uint8_t>{0xAA, 0x55, 0x00, 0x00})<<'\n';
    std::cout<<"E:"<< run(std::vector<std::uint8_t>{0xAA, 0x55, 0x20, 0x10, 0x20, 0x30})<<'\n';
    std::cout <<"F:"<< run(std::vector<std::uint8_t>{0x3C, 0x91, 0xAA, 0x55, 0x01, 0x7E, 0x7F})<<'\n';
    std::cout<<"G:"<< run(std::vector<std::uint8_t>{0xAA, 0x55, 0x04, 0xAA, 0x55, 0x01, 0x02, 0xF8})<<'\n';
    std::cout <<"H:"<< run(std::vector<std::uint8_t>{0xAA, 0x55, 0x04, 0x10, 0x20, 0xAA, 0x55, 0x02, 0x0F, 0xF0, 0xFD})<<'\n';

}