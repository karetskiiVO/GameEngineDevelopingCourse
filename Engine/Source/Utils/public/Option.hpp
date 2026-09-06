#pragma once

#ifndef UTILS_OPTION_hpp
#define UTILS_OPTION_hpp

#include "OneOf.hpp"
#include <cstddef>
#include <cassert>

class None {};

template <typename T>
struct Some {
    T value;

    Some() = default;
    Some(const T& v) : value(v) {
        static_assert(offsetof(Some, value) == 0, "Something did wrong");
    }
    Some(T&& v) : value(std::move(v)) {}

    template <typename... Args>
    explicit Some(Args&&... args) : value(std::forward<Args>(args)...) {}
};

template <typename T>
class Option : public OneOf<Some<T>, None> {
    using Base = OneOf<Some<T>, None>;
    Base* Super() { return static_cast<Base*>(this); }

public:
    using Item = T;

    class IteratorType {
        Option* opt;

        IteratorType(Option* option) : opt(option) {}
    public:
        using Item = T;

        Option<T> Next() {
            if (opt == nullptr) return None();
            auto oldOpt = opt;
            opt = nullptr;

            return *oldOpt;
        }

        friend class Option;
    };

    Option() : Base(None{}) {}
    Option(const T& value) : Base(Some<T>(value)) {}
    Option(T&& value) : Base(Some<T>(std::move(value))) {}
    Option(Some<T> some) : Base(std::move(some)) {}
    Option(None) : Base(None{}) {}

    bool IsSome() const { return Is<Some<T>>(); }
    bool IsNone() const { return Is<None>(); }
    bool HasValue() const { return IsSome(); }

    T& Value() { return Super()->template Get<Some<T>>(); }
    const T& Value() const { return Super()->template Get<Some<T>>(); }

    bool TryGet(T*& out) {
        return Super()->template TryGet<Some<T>>(reinterpret_cast<Some<T>*>(out));
    }

    IteratorType Iterator() { return IteratorType(this); }
};

/*
template <typename T>
    requires std::movable<T>
class Option {
    using Storage = std::aligned_storage_t<sizeof(T), alignof(T)>;
    Storage storage;
    bool hasValue = false;

    T* Ptr() noexcept { return reinterpret_cast<T*>(&storage); }
    const T* Ptr() const noexcept { return reinterpret_cast<const T*>(&storage); }

    template <typename... Args>
    void Construct(Args&&... args) {
        new (&storage) T(std::forward<Args>(args)...);
        hasValue = true;
    }

public:
    Option() = default;

    Option(class None _) {}
    Option(const T& value) { Construct(value); }
    Option(T&& value) { Construct(std::move(value)); }
    Option(const Option& other) {
        if (other.hasValue) Construct(*other.Ptr());
    }

    // TODO: implement this
    Option& operator=(const Option& other) = delete;
    Option& operator=(Option&& other) = delete;
    Option& operator=(const T& value) = delete;
    Option& operator=(T&& value) = delete;

    static const Option None = ::None();
    static Option Some(T&& value) { return Option(value); }

    bool HasValue() const { return hasValue; }

    bool TryGet(T*& value) {
        if (!hasValue) {
            value = nullptr;
        } else {
            value = Ptr();
        }
    }
    bool TryGet(const T*& value) const {
        if (!hasValue) {
            value = nullptr;
        } else {
            value = Ptr();
        }
    }

    T& Value() {
        assert(hasValue && "Try to get value from Option without value inside");
        return *Ptr();
    }
    const T& Value() const {
        assert(hasValue && "Try to get value from Option without value inside");
        return *Ptr();
    }

    void Reset() {
        if (!hasValue) return;
        Value()->~T();
        hasValue = false;
    }

    template <typename... Args>
    T& Emplace(Args&&... args) {
        Reset();
        Construct(std::forward<Args>(args)...);
        return **this;
    }

    // TODO: Traits
    template<typename Func, typename Result>
    Option<Result> Match(Func&& func) const {
        return  hasValue ? func(*Ptr()) : None;
    }

    // TODO: Map & other metatypes
};
*/



#endif // Utils/Option.hpp