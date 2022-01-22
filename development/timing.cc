
#include <ctime>
#include <iostream>
#include <pybind11/embed.h>
#include <pybind11/numpy.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include <Eigen/Dense>

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
  std::vector and relying on copying.  Using Eigen::Array is even more
  efficient.

  Sample output (non-optimised, debug):

    size = 10000000

    Time to initialise py::array_t = 0.30408 Time to get len(arr) = 9e-06 init +
    call time = 0.304089 arrlen = 10000000

    Time to initialise std::vector = 0.080597 Time to get len(vec) = 0.776537
    init + call time = 0.857134 veclen = 10000000

    Time to initialise Eigen::ArrayXd = 0.182463 Time to get len(eig) = 0.04671
    init + call time = 0.229173 eiglen = 10000000


  Sample output (optimised):

    size = 10000000

    Time to initialise py::array_t = 0.110545 Time to get len(arr) = 8e-06 init
    + call time = 0.110553 arrlen = 10000000

    Time to initialise std::vector = 0.019063 Time to get len(vec) = 0.319836
    init + call time = 0.338899 veclen = 10000000

    Time to initialise Eigen::ArrayXd = 0.013564 Time to get len(eig) = 0.025976
    init + call time = 0.03954 eiglen = 10000000


*/
int
main()
{
  constexpr size_t size = 10000000;
  std::cout << "size = " << size << std::endl;

  py::scoped_interpreter guard;

  py::module_ builtins = py::module_::import("builtins");
  py::object len = builtins.attr("len");

  // py::array_t
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

  // std::vector
  std::cout << std::endl;
  start = cpuTime();
  std::vector<double> vec(size);
  for (size_t i = 0; i < vec.size(); ++i) {
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

  // Eigen::ArrayXd
  std::cout << std::endl;
  start = cpuTime();
  Eigen::ArrayXd eig(size);
  for (ssize_t i = 0; i < eig.size(); ++i) {
    eig.coeffRef(i) = i;
  }
  inittime = cpuTime() - start;;
  std::cout << "Time to initialise Eigen::ArrayXd = " << inittime << std::endl;

  start = cpuTime();
  auto eiglen = len(eig);
  calltime = cpuTime() - start;
  std::cout << "Time to get len(eig) = " << calltime << std::endl;
  std::cout << "  init + call time = " << inittime + calltime << std::endl;
  std::cout << "eiglen = " << eiglen.cast<size_t>() << std::endl;
}