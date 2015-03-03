#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <chrono>
#include <iostream>
#include <boost/format.hpp>

class Timer
{
public:
    inline Timer(const std::string &name);
    inline double elapsedSec();
    inline void print();

private:
    typedef std::chrono::high_resolution_clock clock_t;

    std::string _name;
    clock_t::time_point _start;

    inline std::string format(double sec);
};

inline Timer::Timer(const std::string &name) : 
    _name(name), 
    _start(clock_t::now())
{
}

inline double Timer::elapsedSec()
{
    auto current = clock_t::now();
    return std::chrono::duration<double>(current - _start).count();
}

inline void Timer::print()
{
    std::cout << _name << ": " << format(elapsedSec()) << std::endl;
}

// reference: http://stackoverflow.com/questions/4727006/c-logging-and-performance-tuning-library

inline std::string Timer::format(double sec) 
{
    double d = sec;

    if (d < 0.00000001)
    {
        // show in ps with 4 digits
        return (boost::format("%0.4f ps") % (d * 1000000000000.0)).str();
    }
    if (d < 0.00001)
    {
        // show in ns
        return (boost::format("%0.0f ns")% (d * 1000000000.0)).str();
    }
    if (d < 0.001)
    {
        // show in us
        return (boost::format("%0.0f us") % (d * 1000000.0)).str();
    }
    if (d < 0.1)
    {
        // show in ms
        return (boost::format("%0.0f ms") % (d * 1000.0)).str();
    }
    if (d <= 60.0)
    {
        // show in seconds
        return (boost::format("%0.2f s") % d).str();
    }
    if (d < 3600.0)
    {
        // show in min:sec
        return (boost::format("%01.0f:%02.2f") % floor(d/60.0) % fmod(d,60.0)).str();
    }
    // show in h:min:sec
    return (boost::format("%01.0f:%02.0f:%02.2f") % floor(d/3600.0) % floor(fmod(d,3600.0)/60.0) % fmod(d,60.0)).str();
}

#endif // TIMER_HPP_