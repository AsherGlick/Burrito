#include "function_timers.hpp"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <ratio>

using namespace std;

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
// A CPP file is needed to support the static value store of the timers,
// otherwise each compiled object would have its own defintions for the data
////////////////////////////////////////////////////////////////////////////////

#define TIMER_IMPLEMENTATION(TIMER) \
    std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<int64_t, ratio<1, 1000000000>>> begin_##TIMER; \
    std::chrono::duration<int64_t, ratio<1, 1000000000>> duration_##TIMER; \
    uint64_t call_count_##TIMER = 0; \
    void print_timer_##TIMER(std::ostream &out) { \
        int64_t microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration_##TIMER).count(); \
        out << "Timer " << TIMER << " Total Duration: " << microseconds << endl; \
        out << "Timer " << TIMER << " Call Count " << call_count_##TIMER << endl; \
        out << "Timer " << TIMER << " Call Duration: " << float(microseconds) / float(call_count_##TIMER) << endl; \
    } \
    void print_timer_##TIMER() { \
        print_timer_##TIMER(cout); \
    }

TIMER_IMPLEMENTATION(0)
TIMER_IMPLEMENTATION(1)
TIMER_IMPLEMENTATION(2)
TIMER_IMPLEMENTATION(3)
TIMER_IMPLEMENTATION(4)
TIMER_IMPLEMENTATION(5)
TIMER_IMPLEMENTATION(6)
TIMER_IMPLEMENTATION(7)
TIMER_IMPLEMENTATION(8)
TIMER_IMPLEMENTATION(9)
