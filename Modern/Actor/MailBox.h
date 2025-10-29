#pragma once
#include <optional>
#include <string>
#include <variant>
#include <mutex>
#include <queue>

// Message 는 그냥 우선 막 구현...
struct StopMsg {};
struct Ping { int n; };
struct Pong { int n; };
struct Text { std::string s; };

using Message = std::variant<StopMsg, Ping, Pong, Text>;

class Mailbox 
{
public:
    void Push(Message m) 
    {
        {
            std::scoped_lock lk(_lock);
            _messageQueue.push(std::move(m));
        }
        _condition.notify_one();
    }

    std::optional<Message> WaitPop(std::stop_token stoken) 
    {
        std::unique_lock lk(_lock);
        _condition.wait
        (lk, [&]
        {
            return !_messageQueue.empty() || stoken.stop_requested();
        });

        if (stoken.stop_requested() && _messageQueue.empty())
        {
            return std::nullopt;
        }

        Message m = std::move(_messageQueue.front());
        _messageQueue.pop();
        return m;
    }

    std::optional<Message> TryPop() 
    {
        std::scoped_lock lk(_lock);
        if (_messageQueue.empty())
        {
            return std::nullopt;
        }

        Message m = std::move(_messageQueue.front()); 
        _messageQueue.pop();

        return m;
    }

private:
    std::mutex _lock;
    std::condition_variable _condition;
    std::queue<Message> _messageQueue;
};