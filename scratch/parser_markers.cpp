#include <iostream>
#include <cstdint>

enum class ParserState{MarkByte1, MarkByte2, ReadLen, ReadData, ReadCheck};
ParserState next_state(ParserState state, std::uint8_t byte) {
    switch(state) {
        case ParserState::MarkByte1:
            if (byte==0xAA) {
                return ParserState::MarkByte2;
            }else {
                return ParserState::MarkByte1;
            }
        case ParserState::MarkByte2:
            if (byte==0x55) {
                return ParserState::ReadLen;
            }else if (byte==0xAA) {
                return ParserState::MarkByte2;
            }else {
                return ParserState::MarkByte1;
            }
    }
    return state;
}

int main() {
    ParserState state = ParserState::MarkByte1;
    constexpr std::uint8_t first  = 0x3C;
    constexpr std::uint8_t second = 0xAA;
    constexpr std::uint8_t third  = 0x3C;
    constexpr std::uint8_t fourth = 0xAA;
    constexpr std::uint8_t fifth = 0x55;
    state = next_state(state, first);
    std::cout<<static_cast<int>(state)<<'\n';
    state = next_state(state, second);
    std::cout<<static_cast<int>(state)<<'\n';
    state = next_state(state, third);
    std::cout<<static_cast<int>(state)<<'\n';
    state = next_state(state, fourth);
    std::cout<<static_cast<int>(state)<<'\n';
    state = next_state(state, fifth);
    std::cout<<static_cast<int>(state)<<'\n';

}