#include <cstdint>
#include <iostream>

int main() {
    constexpr std::uint8_t high_byte= 0x0A;
    constexpr std::uint8_t low_byte= 0x5C;
    std::uint16_t length= static_cast<std::uint16_t>((high_byte<<8) | low_byte);
    std::uint8_t split_high= static_cast<std::uint8_t>(length>>8);
    std::uint8_t split_low= static_cast<std::uint8_t>(length&0xFF);
    std::cout<<static_cast<std::uint16_t>(split_high)<<'\n';
    std::cout<<static_cast<std::uint16_t>(split_low)<<'\n';
    std::cout<<static_cast<std::uint16_t>(length)<<'\n';
}