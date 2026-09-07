#include "OneOf.hpp"
#include "Option.hpp"
#include "Iterator.hpp"

struct A {
    virtual int func() = 0;
};

struct A1 : A {
    int func() override { return 1; }
};

struct A2 : A {
    int func() override { return 2; }
};

int add (int x) { return x + 1; }
bool odd (int x) { return x % 2 == 0; }

void Test() {
    {
        OneOf<A1, A2> oneof = A1();
        oneof.As<A>().func();
    }

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

    Option<int> a = Some(1);
    for (auto& elem : a) {}

    a.Iterator()
        >> Map(&add)
        >> Filter(&odd);
}