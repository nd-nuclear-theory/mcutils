/****************************************************************
  profiling_test.cpp

  Mark A. Caprio
  University of Notre Dame

****************************************************************/

#include <iostream>

#include "mcutils/profiling.h"

void DoDelay()
{
  const int iterations = 100000000;
  float x=0;
  for (int i=0; i<iterations; ++i) {x+=0.1;};
  std::cout << x << std::endl;  // needed to avoid loop being optimized away!
}

void TestTimer()
{
  // legacy clock timer
  std::cout << "Timer" << std::endl;

  mcutils::Timer timer;
  DoDelay();
  timer.Stop();
  std::cout << "(Time: " << timer.ElapsedTime() << ")" << std::endl;

  timer.Start();
  DoDelay();
  timer.Stop();
  std::cout << "(Time: " << timer.ElapsedTime() << ")" << std::endl;

  std::cout << std::endl;
}

void TestSteadyTimer()
{
  // steady_clock timer
  std::cout << "SteadyTimer" << std::endl;

  mcutils::SteadyTimer steady_timer;
  steady_timer.Start();
  DoDelay();
  steady_timer.Stop();
  std::cout << "(Time: " << steady_timer.ElapsedTime() << ")" << std::endl;

  steady_timer.Start();
  DoDelay();
  // TEST OMITTING Stop: steady_timer.Stop();
  std::cout << "(Time: " << steady_timer.ElapsedTime() << ")" << std::endl;

  std::cout << std::endl;
}

void CompareTimers()
{
  // steady_clock timer
  std::cout << "Compare" << std::endl;

  mcutils::Timer timer;
  mcutils::SteadyTimer steady_timer;
  timer.Start();
  steady_timer.Start();
  DoDelay();
  timer.Stop();
  steady_timer.Stop();
  std::cout << "(Time: timer " << timer.ElapsedTime() << ", steady_timer " << steady_timer.ElapsedTime() << ")" << std::endl;

  std::cout << std::endl;
}

int main(int argc, char **argv)
{

  TestTimer();
  TestSteadyTimer();
  CompareTimers();


  // termination
  std::exit(EXIT_SUCCESS);
}
