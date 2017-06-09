/****************************************************************

  profiling.h

  Simple timing with clock().

  Mark A. Caprio
  University of Notre Dame

  - 2/23/11 (mac): Created.
  - 6/9/17 (mac):
    + Move into mcutils namespace.
    + Start clock on timer construction.
    + Allow accumulation of intervals.
    + Provide accessor for raw clocks.
    + Upgrade to use C++ chrono

                                  
****************************************************************/

#ifndef PROFILING_H_
#define PROFILING_H_

#include <ctime>

////////////////////////////////////////////////////////////////
// simple clock timing
////////////////////////////////////////////////////////////////

namespace mcutils
{
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////

  class Timer{

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

}  // namespace

// legacy support for global definitions

#ifdef MCUTILS_ALLOW_LEGACY_GLOBAL
using mcutils::Timer;
#endif

#endif
