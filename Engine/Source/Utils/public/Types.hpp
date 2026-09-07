#pragma once

#ifndef UTILS_TYPES_hpp
#define UTILS_TYPES_hpp

#include <cstdint>
#include <cstddef>
#include <type_traits>


using usize = std::size_t;
// using ssize = std::ssize_t;

using byte = std::byte;

using int8  = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8  = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using float32 = float;
using float64 = double;

template <typename Func, typename Signature>
struct IsCallable : public std::false_type {};

template <typename Func, typename Ret, typename... Args>
struct IsCallable<Func, Ret(Args...)> : public std::is_invocable_r<Ret, Func, Args...> {};

template <typename Func, typename Signature>
concept Call = std::invocable<Func, Signature>;
template <typename Func, typename Arg>
using ReturnTypeOfCall = std::invoke_result_t<Func, Arg>;

#endif // Utils/Types.hpp