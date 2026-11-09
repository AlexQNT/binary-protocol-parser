#pragma once
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

enum class FeedResult{Pending,Packet, Rejected};
FeedResult feed(Parser& parser, std::uint8_t byte);