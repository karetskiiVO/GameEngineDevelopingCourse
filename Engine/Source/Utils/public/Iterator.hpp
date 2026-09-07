#include "Option.hpp"
#include <utility>

template <typename T>
concept IteratorTrait = requires (T it) {
    typename T::Item;
    { it.Next() } -> std::same_as<Option<typename T::Item>>;
};

template <typename TSrc>
concept IteratorSource = requires (TSrc itsrc) {
    typename TSrc::Item;
    { itsrc.Iterator() } -> IteratorTrait;
    requires std::same_as<typename decltype(itsrc.Iterator())::Item, typename TSrc::Item>;
};

template <typename Iter>
class IteratorRangeAdaptor {
public:
    using Item = typename Iter::Item;
    using pointer = Item*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

    explicit IteratorRangeAdaptor(Iter&& iter) : iter(iter), current(iter.Next()) {}
    IteratorRangeAdaptor() = default;

    bool Ends() const { return !current.HasValue(); }

    Item& operator*() { return current.Value(); }
    const Item& operator*() const { return current.Value(); }

    IteratorRangeAdaptor& operator++() {
        current = iter.Next();
        return *this;
    }

    void operator++(int) { ++(*this); }

private:
    Iter iter;
    Option<Item> current;
};

template <typename Iter>
struct IteratorRangeSentinel {};
template <typename Iter>
bool operator==(const IteratorRangeAdaptor<Iter>& it, IteratorRangeSentinel<Iter>) { return it.Ends(); }
template <typename Iter>
bool operator==(IteratorRangeSentinel<Iter>, const IteratorRangeAdaptor<Iter>& it) { return it.Ends(); }
template <typename Iter>
bool operator!=(const IteratorRangeAdaptor<Iter>& it, IteratorRangeSentinel<Iter> sent) { return !(it == sent); }
template <typename Iter>
bool operator!=(IteratorRangeSentinel<Iter> sent, const IteratorRangeAdaptor<Iter>& it) { return !(it == sent); }

template <IteratorTrait Iter>
auto begin(Iter& it) { return IteratorRangeAdaptor(it); }
template <IteratorTrait Iter>
auto end(Iter&) { return IteratorRangeSentinel<Iter>{}; }

template <IteratorSource IterSource>
auto begin(IterSource& itsrc) { return IteratorRangeAdaptor(itsrc.Iterator()); }
template <IteratorSource IterSource>
auto end(IterSource& itsrc) { return IteratorRangeSentinel<decltype(itsrc.Iterator())>{}; }

template <typename Func>
class Map {
    Func func;
public:
    template <IteratorTrait Iter> requires Call<Func, typename Iter::Item>
    class Iterator {
        Iter iter;
        Func func;
    public:
        using Item = ReturnTypeOfCall<Func, typename Iter::Item>;

        Iterator(Iter&& iter, Func&& func) : iter(std::move(iter)), func(std::move(func)) {}

        Option<Item> Next() {
            return iter
                .Next()
                .Match(
                    [&](Some<Item>&& val) { return Option<Item>(Some(func(val))); },
                    [&](None&&)           { return Option<Item>(None()); }
                );
        }
    };

    Map(Func&& func) : func(std::move(func)) {}

    template <IteratorTrait Iter>
    using OperationResult = Iterator<Iter>;

    template <IteratorTrait Iter>
    Iterator<Iter> Apply(Iter&& iter) && {
        return Iterator<Iter>(std::forward<Iter>(iter), std::move(func));
    }
};

template <typename Func>
class Filter {
    Func func;
public:
    template <IteratorTrait Iter> requires Call<Func, typename Iter::Item>
    class Iterator {
        Iter iter;
        Func func;
    public:
        using Item = Iter::Item;

        Iterator(Iter&& iter, Func&& func) : iter(std::move(iter)), func(std::move(func)) {}

        Option<Item> Next() {
            while (true) {
                auto opt = iter.Next();

                if (opt.HasValue()) {
                    if (func(opt.Get())) {
                        return opt;
                    }
                } else {
                    return None();
                }
            }
        }
    };

    Filter(Func&& func) : func(std::move(func)) {}

    template <IteratorTrait Iter>
    using OperationResult = Iterator<Iter>;

    template <IteratorTrait Iter>
    Iterator<Iter> Apply(Iter&& iter) && {
        return Iterator<Iter>(std::forward<Iter>(iter), std::move(func));
    }
};

template <typename Op, typename Iter>
concept IteratorOperation = requires (Op op, Iter iter) {
    requires IteratorTrait<Iter>;
    { std::move(op).Apply(std::move(iter)) };
};

template <IteratorTrait Iter, typename Operation>
requires IteratorOperation<Operation, Iter>
auto operator>> (Iter&& iter, Operation&& operation) {
    return std::move(operation).Apply(std::forward<Iter>(iter));
}
