#include "parser.h"
std::uint8_t check_update(std::uint8_t check, std::uint8_t byte) {
#ifdef USE_CRC8
    check ^= byte;
    for (int i = 0; i < 8; ++i) {
        if (check & 0x80) {
            check = static_cast<std::uint8_t>((check << 1) ^ 0x07);
        } else {
            check = static_cast<std::uint8_t>(check << 1);
        }
    }
    return check;
#else
    return static_cast<std::uint8_t>(check ^ byte);
#endif
}
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
                parser.check=check_update(0, byte);
                parser.state= ParserState::ReadCheck;
                return FeedResult::Pending;
            }else {
                parser.check=check_update(0, byte);
                parser.expected=byte;
                parser.count=0;
                parser.state= ParserState::ReadData;
                return FeedResult::Pending;
            }
        case ParserState::ReadData:
            parser.buffer[parser.count]=byte;
            parser.check = check_update(parser.check, byte);
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