#include "tick_engine.h"

TickEngine::TickEngine(int initial_value)
        : value_(initial_value) {}

void TickEngine::Wait() {
    std::unique_lock<std::mutex> UniqueLock(mutex_);
    if (value_ == 0) {
        condition_variable_.wait(UniqueLock);
    }
    --value_;
}

void TickEngine::Tick() {
    std::lock_guard<std::mutex> LockGuard(mutex_);
    ++value_;
    condition_variable_.notify_all();
}
