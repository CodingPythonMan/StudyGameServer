#pragma once
#include "Actor.h"

class ActorSystem 
{
public:
    template<typename BehaviorFunc>
    ActorRef Spawn(std::string name, BehaviorFunc behavior) 
    {
        auto actor = std::make_unique<Actor>(name, ToBehavior(behavior));
        ActorRef ref = actor->ref();
        actor->start();

        std::scoped_lock lk(_lock);
        _actors.push_back(std::move(actor));

        return ref;
    }

    void Shutdown()
    {
        std::scoped_lock lk(_lock);
        for (auto& a : _actors) 
        {
            a->stop();
        }
        _actors.clear();
    }

    ~ActorSystem() 
    {
        Shutdown();
    }

private:
    template<typename F>
    static Actor::Behavior ToBehavior(F f) 
    {
        return Actor::Behavior(f);
    }

    std::mutex _lock;
    std::vector<std::unique_ptr<Actor>> _actors;
};
