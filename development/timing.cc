
#include <ctime>
#include <iostream>
#include <pybind11/embed.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

/*
  Get cpu time in seconds.
*/
double
cpuTime()
{
  return double(clock()) / double(CLOCKS_PER_SEC);
}

/*
  This shows that for large vectors there is considerable time saving in
  constructing py::array_t objects to pass to python rather than using
  std::vector and relying on copying.

  Sample output (non-optimised, debug):

    size = 10000000

    Time to initialise py::array_t = 0.314505
    Time to get len(arr) = 1.2e-05
      init + call time = 0.314517
    arrlen = 10000000

    Time to initialise std::vector = 0.223642
    Time to get len(vec) = 0.768485
      init + call time = 0.992127
    veclen = 10000000

  Sample output (optimised):

    size = 10000000

    Time to initialise py::array_t = 0.116793
    Time to get len(arr) = 1.6e-05
      init + call time = 0.116809
    arrlen = 10000000

    Time to initialise std::vector = 0.03023
    Time to get len(vec) = 0.32423
      init + call time = 0.35446
    veclen = 10000000

*/
int
main()
{
  constexpr size_t size = 10000000;
  std::cout << "size = " << size << std::endl;

  py::scoped_interpreter guard;

  py::module_ builtins = py::module_::import("builtins");
  py::object len = builtins.attr("len");

  std::cout << std::endl;
  double start = cpuTime();
  py::array_t<double> arr(size);
  auto unchecked = arr.mutable_unchecked();
  for (ssize_t i = 0; i < unchecked.size(); ++i) {
    unchecked[i] = i;
  }

  double inittime = cpuTime() - start;
  std::cout << "Time to initialise py::array_t = " << inittime << std::endl;

  start = cpuTime();
  auto arrlen = len(arr);
  double calltime = cpuTime() - start;
  std::cout << "Time to get len(arr) = " << calltime << std::endl;
  std::cout << "  init + call time = " << inittime + calltime << std::endl;
  std::cout << "arrlen = " << arrlen.cast<size_t>() << std::endl;

  std::cout << std::endl;
  start = cpuTime();
  std::vector<double> vec(size);
  for (ssize_t i = 0; i < arr.size(); ++i) {
    vec[i] = i;
  }
  inittime = cpuTime() - start;;
  std::cout << "Time to initialise std::vector = " << inittime << std::endl;

  start = cpuTime();
  auto veclen = len(vec);
  calltime = cpuTime() - start;
  std::cout << "Time to get len(vec) = " << calltime << std::endl;
  std::cout << "  init + call time = " << inittime + calltime << std::endl;
  std::cout << "veclen = " << veclen.cast<size_t>() << std::endl;
}