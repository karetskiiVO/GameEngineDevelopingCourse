#include "Option.hpp"

template <typename T>
concept Iterator = requires (T it) {
    typename T::Item;
    { it.Next() } -> std::same_as<Option<typename T::Item>>;
};

template <typename TSrc>
concept IteratorSource = requires (TSrc itsrc) {
    typename TSrc::Item;
    { itsrc.Iterator() } -> Iterator;
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

template <Iterator Iter>
auto begin(Iter& it) { return IteratorRangeAdaptor(it); }
template <Iterator Iter>
auto end(Iter&) { return IteratorRangeSentinel<Iter>{}; }

template <IteratorSource IterSource>
auto begin(IterSource& itsrc) { return IteratorRangeAdaptor(itsrc.Iterator()); }
template <IteratorSource IterSource>
auto end(IterSource& itsrc) { return IteratorRangeSentinel<decltype(itsrc.Iterator())>{}; }
