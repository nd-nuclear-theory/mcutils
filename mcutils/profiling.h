/****************************************************************

  profiling.h

  Simple walltime stopwatch.

  Mark A. Caprio
  University of Notre Dame

  - 2/23/11 (mac): Created.
  - 6/9/17 (mac):
    + Move into mcutils namespace.
    + Start clock on timer construction.
    + Allow accumulation of intervals.
    + Provide accessor for raw clocks.
  - 12/23/17 (mac): Add SteadyTimer based on C++11 chrono library.

                                  
****************************************************************/

#ifndef PROFILING_H_
#define PROFILING_H_

#include <cassert>
#include <ctime>
#include <chrono>

#include "deprecated.h"

namespace mcutils
{
  ////////////////////////////////////////////////////////////////
  // legacy timer
  ////////////////////////////////////////////////////////////////

  
  class DEPRECATED("use SteadyTimer instead")
  Timer
  // Timer using legacy C clock().
  //
  // Warning: Does not provide reliable walltime (e.g., frequently off
  // by factor of 2 in Haswell runs).
  {

    public:
    ////////////////////////////////
    // constructors
    ////////////////////////////////

    // default constructor
    Timer() : start_time_(0), elapsed_time_(0)
      // Initialize elapsed time to zero and "start clock".
      {
        Start();
      }; 

    ////////////////////////////////
    // stopwatch
    ////////////////////////////////

    void Start()
    // Set starting time from which next increment will be measured.
    {
      start_time_ = std::clock();
    };

    void Stop()
    // Increment aggregate elapsed time by time since last start call.
    {
      elapsed_time_ += std::clock() - start_time_;
    };

    clock_t ElapsedClocks() const {
      return elapsed_time_;
    };
  
    double ElapsedTime() const {
      return (static_cast<double>(ElapsedClocks())) / CLOCKS_PER_SEC;
    };

    ////////////////////////////////
    // 
    ////////////////////////////////

    private:

    clock_t start_time_;
    clock_t elapsed_time_;

  };

  ////////////////////////////////////////////////////////////////
  // steady clock timer
  ////////////////////////////////////////////////////////////////

  class SteadyTimer
  // Timer using C++11 chrono library steady_clock.
  //
  // The interface to this timer is like that of a traditional
  // stopwatch (a real physical one).  It comes to you set to zero.
  // You have to start it to set it going (SteadyTimer::Start).  You
  // can stop it (SteadyTimer::Stop) and later restart it, in which
  // case the time just keeps accumulating.  And you can look at the
  // elapsed time at any time, whether the stopwatch is stopped or
  // running.  You can also reset it (SteadyTimer::Reset), which we
  // take to mean we stop the stopwatch (this might or might not be
  // the interpretation on a physical stopwatch) and reset the elapsed
  // time to zero.
  //
  // Times are in seconds, represented as type double.
  {

    public:
    ////////////////////////////////
    // constructors
    ////////////////////////////////

    // default constructor
    SteadyTimer() : timer_is_running_(false),accumulated_time_(0)
      {}; 

    ////////////////////////////////
    // stopwatch
    ////////////////////////////////

    void Start()
    // Start/resume stopwatch.
    //
    // Sets starting time from which next increment will be measured.
    {
      assert(!timer_is_running_);
      timer_is_running_ = true;
      start_time_ = std::chrono::steady_clock::now();
    };

    void Reset()
    // Reset stopwatch to initial state.
    {
      timer_is_running_ = false;
      accumulated_time_ = 0;
    };

    void Stop()
    // Stop stopwatch and accumulate time since last start.
    {
      assert(timer_is_running_);
      timer_is_running_ = false;
      accumulated_time_ += TimeSinceLastStart();
    };

    double ElapsedTime() const
    // Elapsed time on stopwatch.
    //
    // This includes previously accumulated time plus any new elapsed
    // time since last start/resume.
    {
      double elapsed_time = accumulated_time_;
      if (timer_is_running_)
        elapsed_time += TimeSinceLastStart();
      return elapsed_time;
    };

    private:
    
    double TimeSinceLastStart() const
      // Peek at time since last start/restart.
      {
        std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> time_difference_duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time_);
        return time_difference_duration.count();
      }

    ////////////////////////////////
    // timing data
    ////////////////////////////////

    private:

    std::chrono::steady_clock::time_point start_time_;
    double accumulated_time_;
    bool timer_is_running_;

  };


}  // namespace

// legacy support for global definitions

#ifdef MCUTILS_ALLOW_LEGACY_GLOBAL
using mcutils::Timer;
#endif

#endif
