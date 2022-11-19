#pragma once

#include <chrono>
#include <cstdint>
#include <iostream>
#include <ratio>

////////////////////////////////////////////////////////////////////////////////
// Function Timers
//
// This is a hacky profiling library that is used to easily determine the
// difference in speed of a repeatedly called piece of code. There are some
// caviats to this, which is that it only really measures the code that runs
// between the start and end calls but not the function call itself so it
// does not do a good job at catching extranious copy calls from things like
// function arguments.
//
// Usage: (any number from 0-9)
//     start_timer_0();
//     ...
//     stop_timer_0();
//     ...
//     print_timer_0();
////////////////////////////////////////////////////////////////////////////////

#define TIMER_PROTOTYPE(TIMER) \
    extern std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<int64_t, std::ratio<1, 1000000000>>> begin_##TIMER; \
    extern std::chrono::duration<int64_t, std::ratio<1, 1000000000>> duration_##TIMER; \
    extern uint64_t call_count_##TIMER; \
    inline void start_timer_##TIMER() { \
        begin_##TIMER = std::chrono::high_resolution_clock::now(); \
    } \
    inline void stop_timer_##TIMER() { \
        auto end = std::chrono::high_resolution_clock::now(); \
        duration_##TIMER += end - begin_##TIMER; \
        call_count_##TIMER += 1; \
    } \
    void print_timer_##TIMER(); \
    void print_timer_##TIMER(std::ostream&);

TIMER_PROTOTYPE(0)
TIMER_PROTOTYPE(1)
TIMER_PROTOTYPE(2)
TIMER_PROTOTYPE(3)
TIMER_PROTOTYPE(4)
TIMER_PROTOTYPE(5)
TIMER_PROTOTYPE(6)
TIMER_PROTOTYPE(7)
TIMER_PROTOTYPE(8)
TIMER_PROTOTYPE(9)
