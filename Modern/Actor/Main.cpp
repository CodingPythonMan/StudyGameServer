#include <iostream>
#include "ActorSystem.h"

int main() 
{
    ActorSystem sys;

    ActorRef pong = sys.Spawn("PONG", [&](const ActorRef& self, const ActorRef& sender, Message& m) 
    {
        std::visit([&](auto&& v) 
        {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, Ping>) 
            {
                std::cout << "[PONG] received Ping(" << v.n << ")\n";
                sender.Tell(Pong{ v.n + 1 });
            }
            else if constexpr (std::is_same_v<T, Text>) {
                std::cout << "[PONG] Text: " << v.s << "\n";
            }
            else if constexpr (std::is_same_v<T, StopMsg>) {
                std::cout << "[PONG] StopMsg\n";
            }
        }, m);
    });

    ActorRef ping = sys.Spawn("PING", [&](const ActorRef& self, const ActorRef& sender, Message& m)
    {
        std::visit([&](auto&& v) 
        {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, Pong>) {
                std::cout << "[PING] received Pong(" << v.n << ")\n";
            }
            else if constexpr (std::is_same_v<T, StopMsg>) {
                std::cout << "[PING] StopMsg\n";
            }
        }, m);
    });

    for (int i = 0; i < 5; ++i) 
    {
        std::cout << "[MAIN] send Ping(" << i << ") to PONG\n";

        pong.Tell(Ping{ i });
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        ping.Tell(Pong{ i + 100 });
    }

    pong.Tell(Text{ "hello from main" });

    std::this_thread::sleep_for(std::chrono::seconds(1));

    sys.Shutdown();
    std::cout << "ActorSystem shutdown complete.\n";
    return 0;
}