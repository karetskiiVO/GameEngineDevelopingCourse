#pragma once

// TODO: assert -> panic

#ifndef UTILS_ONEOF_hpp
#define UTILS_ONEOF_hpp

#include <cassert>

#include "Types.hpp"

template <typename U, typename... Ts>
concept OneOfType = (std::same_as<std::remove_cvref_t<U>, Ts> || ...);

template <typename ...T>
class OneOf {
    // Helpers -- template magic

    template <usize I, typename... Ts>
    struct TypeAtImpl;
    template <usize I, typename T0, typename... Ts>
    struct TypeAtImpl<I, T0, Ts...> : TypeAtImpl<I - 1, Ts...> {};
    template <typename T0, typename... Ts>
    struct TypeAtImpl<0, T0, Ts...> { using Type = T0; };
    template <usize I, typename... Ts>
    using TypeAt = TypeAtImpl<I, Ts...>::Type;

    template <typename U, typename... Ts> struct IndexOf;
    template <typename U, typename T0, typename... Ts>
    struct IndexOf<U, T0, Ts...> { static constexpr usize value = 1 + IndexOf<U, Ts...>::value; };
    template <typename U>
    struct IndexOf<U> { static_assert(sizeof(U) == 0, "Type not found in OneOf"); };

    template <typename... Ts>
    struct MaxSizeof;
    template <>
    struct MaxSizeof<> { static constexpr usize value = 0; };
    template <typename T0, typename... Ts>
    struct MaxSizeof<T0, Ts...> {
        static constexpr usize value = sizeof(T0) > MaxSizeof<Ts...>::value ? sizeof(T0) : MaxSizeof<Ts...>::value;
    };

    template <typename... Ts>
    struct MaxAllignOf;
    template <>
    struct MaxAllignOf<> { static constexpr usize value = 1; };
    template <typename T0, typename... Ts>
    struct MaxAllignOf<T0, Ts...> {
        static constexpr usize value = alignof(T0) > MaxAllignOf<Ts...>::value ? alignof(T0) : MaxAllignOf<Ts...>::value;
    };

    static constexpr usize storageSize = MaxSizeof<T...>::value;
    static constexpr usize storageAlign = MaxAllignOf<T...>::value;
    static constexpr usize typesCount = sizeof...(T);

    // end helpers

    static constexpr usize invalidType = static_cast<usize>(-1);

    alignas(storageAlign) unsigned char storage[1 > storageSize ? 1 : storageSize];
    usize typeIndex = invalidType;

protected:
    template <typename TargetT>
    TargetT* GetPtr() {
        return typeIndex == IndexOf<TargetT, T...>::value ? reinterpret_cast<TargetT*>(storage) : nullptr;
    }
    template <typename TargetT>
    const TargetT* GetPtr() const{
        return typeIndex == IndexOf<TargetT, T...>::value ? reinterpret_cast<TargetT*>(storage) : nullptr;
    }

private:
    template <typename TargetT>
    TargetT& PrivateGet() {
        auto ptr = GetPtr<TargetT>();
        assert(ptr != nullptr && "try to get value that not matches with type into OneOf");
        return *ptr;
    }
    template <typename TargetT>
    const TargetT& PrivateGet() const{
        auto ptr = GetPtr<TargetT>();
        assert(ptr != nullptr && "try to get value that not matches with type into OneOf");
        return *ptr;
    }

    template <typename TargetT, typename... Args>
    void Construct(Args&&... args) {
        static_assert(OneOfType<TargetT, T...>, "U must be one of the OneOf types");
        typeIndex = IndexOf<TargetT, T...>::value;
        new (storage) TargetT(std::forward<Args>(args)...);
    }

    void Destroy() {
        if (typeIndex < typesCount) {
            DestroyImpl();
            typeIndex = invalidType;
        }
    }

    template <usize index = 0>
    void DestroyImpl() {
        if constexpr (index < typesCount) {
            if (typeIndex == index) {
                using CurrentType = TypeAt<index, T...>::Type;
                GetPtr<CurrentType>()->~CurrentType();
            } else {
                Destroy<index + 1>();
            }
        }
    }

    template <usize index = 0>
    void ConstructFrom(OneOf&& src) {
        if constexpr (index == typesCount) {
            return;
        } else {
            if (typeIndex == index) {
                using CurrentType = TypeAt<index, T...>;
                auto&& srcValue = src.PrivateGet<CurrentType>();
                Construct<CurrentType>(std::forward<decltype(srcValue)>(srcValue));
            } else {
                ConstructFrom<index + 1>(std::forward(src));
            }
        }
    }

    template <usize index = 0, typename Tuple>
    decltype(auto) MatchImpl(Tuple&& funcs) {
        if constexpr (index == typesCount - 1) {
            using CurrentType = TypeAt<index, T...>;
            return std::invoke(
                std::get<index>(std::forward<Tuple>(funcs)),
                Get<CurrentType>()
            );
        } else {
            if (typeIndex == index) {
                using CurrentType = TypeAt<index, T...>;
                return std::invoke(
                    std::get<index>(std::forward<Tuple>(funcs)),
                    Get<CurrentType>()
                );
            } else {
                return MatchImpl<index + 1>(std::forward<Tuple>(funcs));
            }
        }
    }

public:
    template <typename TargetT> requires OneOfType<TargetT, T...>
    OneOf(TargetT&& value) { Construct<std::remove_cvref_t<TargetT>>(std::forward<TargetT>(value)); }
    OneOf(OneOf&& other) : typeIndex(other.typeIndex) { ConstructFrom(std::move(other)); }

    ~OneOf () { Destroy(); }

    OneOf& operator=(const OneOf& other) {
        if (this != &other) {
            Destroy();
            typeIndex = other.typeIndex;
            ConstructFrom(other);
        }
        return *this;
    }
    OneOf& operator=(OneOf&& other) noexcept {
        if (this != &other) {
            Destroy();
            typeIndex = other.typeIndex;
            ConstructFrom(std::move(other));
        }
        return *this;
    }

    template <typename TargetT> requires OneOfType<TargetT, T...>
    OneOf& operator=(TargetT&& value) {
        using DecayedTargetT = std::remove_cvref_t<TargetT>;
        if (auto ptr = GetPtr<TargetT>()) {
            *ptr = std::forward<TargetT>(value);
        } else {
            Destroy();
            Construct<DecayedTargetT>(std::forward<DecayedTargetT>(value));
        }

        return *this;
    }

    template <typename TargetT> requires OneOfType<TargetT, T...>
    bool Is() const { return typeIndex == IndexOf<TargetT, T...>::value; }

    template <typename TargetT> requires OneOfType<TargetT, T...>
    TargetT& Get() & { return PrivateGet<TargetT>(); }

    template <typename TargetT> requires OneOfType<TargetT, T...>
    const TargetT& Get() const & { return PrivateGet<TargetT>(); }

    template <typename TargetT> requires OneOfType<TargetT, T...>
    TargetT&& Get() && { return std::move(PrivateGet<TargetT>()); }

    template <typename TargetT> requires OneOfType<TargetT, T...>
    bool TryGet(TargetT*& out) {
        auto ptr = GetPtr<TargetT>();
        if (ptr != nullptr) {
            out = ptr;
            return true;
        }
        out = nullptr;
        return false;
    }

    template <typename... Funcs>
    decltype(auto) Match(Funcs&&... funcs) {
        static_assert(sizeof...(funcs) == typesCount, "Match requires exactly one function per alternative");
        return MatchImpl(std::forward_as_tuple(std::forward<Funcs>(funcs)...));
    }
    template <typename... Funcs>
    decltype(auto) Match(Funcs&&... funcs) const {
        static_assert(sizeof...(funcs) == typesCount, "Match requires exactly one function per alternative");
        return MatchImpl(std::forward_as_tuple(std::forward<Funcs>(funcs)...));
    }

    template <typename... Funcs>
    void Switch(Funcs&&... funcs) {
        static_assert(sizeof...(Funcs) == typesCount, "Switch requires exactly one action per alternative");
        MatchImpl(std::forward_as_tuple(std::forward<Funcs>(funcs)...));
    }
    template <typename... Funcs>
    void Switch(Funcs&&... funcs) const {
        static_assert(sizeof...(Funcs) == typesCount, "Switch requires exactly one action per alternative");
        MatchImpl(std::forward_as_tuple(std::forward<Funcs>(funcs)...));
    }
};

#endif // Utils/OneOf.hpp