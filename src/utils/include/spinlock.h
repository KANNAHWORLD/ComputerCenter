#pragma once
#include <atomic>

typedef struct Spinlock{
    std::atomic<bool> _lock{false};
    bool lock(){
        while(_lock.exchange(true,  std::memory_order_acquire)){
            while(_lock.load(std::memory_order_relaxed)){}
        }
        return true;
    }

    void unlock(){
        _lock.store(false, std::memory_order_release);
    }

    bool trylock(){
        return !_lock.exchange(true, std::memory_order_acquire);
    }

} Spinlock;
