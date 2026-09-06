#include "OneOf.hpp"
#include "Option.hpp"
#include "Iterator.hpp"

void Test() {
    OneOf<int, std::string, float> oneof = 2;

    oneof.Is<std::string>();
    oneof.Get<int>();
    oneof.Match(
        [] (int x) -> int { return 1; },
        [] (std::string x)   -> int { return 2; },
        [] (float x)    -> int { return 3; }
    );

    oneof.Switch(
        [] (auto x) {},
        [] (auto x) {},
        [] (auto x) {}
    );

    Option<int> a = None();
    for (auto& elem : a) {}
}