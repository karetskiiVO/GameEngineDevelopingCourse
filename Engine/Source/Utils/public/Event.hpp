#pragma once

#include <utility>
#include <vector>
template<typename Signature>
class Event;

// TODO: Fix it
template<typename Ret, typename ...Args>
class Event<Ret(Args...)> {
public:
    virtual Ret Invoke(Args... args) {}

    Ret operator() (Args... args) { return Invoke(std::forward(args)...); }
};

template<typename Signature>
class Delegate;

template<typename Ret, typename ...Args>
class Delegate<Ret(Args...)> {
public:
    using EventType = Event<Ret(Args...)>;

    class BindInfo {};

private:
    std::vector<EventType&> handles;

public:
    BindInfo Bind(EventType& event) { handles.push_back(event); return {}; }

    

};
