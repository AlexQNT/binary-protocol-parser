#include <cstdint>
#include <iostream>

int main() {
    constexpr std::uint8_t a=0x5C;
    constexpr std::uint8_t b=0x0A;
    std::uint16_t little_end=static_cast<std::uint16_t>((b<<8)|a);
    std::uint16_t big_end=static_cast<std::uint16_t>((a<<8)|b);
    std::cout<<little_end<<'\n';
    std::cout<<big_end<<'\n';
}