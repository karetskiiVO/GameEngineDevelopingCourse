#pragma once

#include "Types.hpp"
#include <cstddef>
#include <format>
#include <iostream>
#include <stdexcept>

template <size_t count>
struct PanicFormatLine { static constexpr std::string value = PanicFormatLine<count-1>::value + " {}"; };
template <>
struct PanicFormatLine<1> { static constexpr std::string value = "PANIC: {}"; };
template <>
struct PanicFormatLine<0> { static constexpr std::string value = "PANIC"; };

template <typename ...Args>
void panic(Args... args) {
    constexpr usize count = sizeof...(Args);
    auto val = std::format(PanicFormatLine<count>::value, args...);
    std::cerr << val << std::endl;
    throw std::runtime_error(val);
}

template <typename ...Args>
void panicf(const std::string& format, Args... args) {
    auto val = std::format(format, args...);
    std::cerr << val << std::endl;
    throw std::runtime_error(val);
}