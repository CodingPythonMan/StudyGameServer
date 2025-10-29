#pragma once
#include "ActorRef.h"
#include <functional>

class Actor {
public:
    using Behavior = std::function<void(const ActorRef&, const ActorRef&, Message&)>;

    Actor(std::string name, Behavior behavior)
        : name_(std::move(name))
        , mailbox_(std::make_shared<Mailbox>())
        , behavior_(std::move(behavior))
    {
    }

    ~Actor() {
        stop();
    }

    ActorRef ref() const 
    {
        return ActorRef{ mailbox_, name_ };
    }

    void start() 
    {
        if (thread_) return;
        stop_source_ = std::stop_source();
        thread_ = std::make_unique<std::jthread>([this](std::stop_token stoken) {
            run_loop(stoken);
        }, stop_source_.get_token());
    }

    void stop() 
    {
        if (!thread_)
        {
            return;
        }

        stop_source_.request_stop();
        mailbox_->Push(StopMsg{});
        if (thread_->joinable())
        {
            thread_->join();
        }

        thread_.reset();
    }

private:
    void run_loop(std::stop_token stoken) 
    {
        ActorRef self = ref();
        ActorRef nobody{ std::make_shared<Mailbox>(), std::string("<nobody>") };
        while (true) {
            auto opt = mailbox_->WaitPop(stoken);
            if (!opt.has_value()) break;
            Message msg = std::move(*opt);

            bool should_break = false;
            std::visit([&](auto&& v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, StopMsg>) {
                    should_break = stoken.stop_requested();
                }
            }, msg);

            try 
            {
                behavior_(self, nobody, msg);
            }
            catch (const std::exception& e) 
            {
                std::cerr << "[" << name_ << "] exception in behavior: " << e.what() << "\n";
            }

            if (should_break) break;
            if (stoken.stop_requested()) break;
        }
    }

    std::string name_;
    std::shared_ptr<Mailbox> mailbox_;
    Behavior behavior_;
    std::unique_ptr<std::jthread> thread_;
    std::stop_source stop_source_;
};