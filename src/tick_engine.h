#ifndef TICK_ENGINE_H
#define TICK_ENGINE_H


#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

class TickEngine {
public:
    explicit TickEngine(int initial_value = 0);
    ~TickEngine() = default;

    void Wait();
    void Tick();
private:
    int value_;
    std::mutex mutex_;
    std::condition_variable condition_variable_;
};


#endif
