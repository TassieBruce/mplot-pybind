
#include <mplot++/utilities.h>
#include <pybind11/eigen.h>
#include <iostream>

using namespace mplotpp;
namespace py = pybind11;

int
main()
{
  py::scoped_interpreter guard;

  py::print("arange(-3, 3, 1) = ", arange(-3, 3, 1));
  py::print("arange(-3.0, 3.0, 0.5) = ", arange(-3.0, 3.0, 0.5));
  py::print("arange(-3.0, 3.001, 0.5) = ", arange(-3.0, 3.001, 0.5));
  py::print("arange(3.0, -3.0, 0.5) = ", arange(3.0, -3.0, 0.5));
  py::print("arange(3.0, -3.0, -0.5) = ", arange(3.0, -3.0, -0.5));
  py::print("arange(3.0, -3.0, 0.0) = ", arange(3.0, -3.0, 0.0));

}