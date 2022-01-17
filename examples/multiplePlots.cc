
/// @cond
#include <mplot++/utilities.h>
#include <pybind11/embed.h>
#include <pybind11/numpy.h>
#include <iostream>
#include <vector>
/// @endcond 

using namespace mplotpp;
namespace py = pybind11;
using namespace py::literals;

/**
   @brief Multiple curves on the one axis

   Python:
   @include multiplePlots.py

   c++:
   @include multiplePlots.cc

   Notes:

   - `py::array_t<double> x(100);`

     Creates a `numpy` 1-d array in the python interpreter that is accessible
     from `c++`.  The embedded interpreter must have been initialised before
     this (via `py::scoped_interpreter`).  Element access via `at()` and
     `mutable_at()` methods is more clumsy than using `std::vector` but data
     does not need to be copied when later passing to `ax.attr("plot")`

   - `using namespace py::literals;`

     Gives access to [user-defined
     literals](https://en.cppreference.com/w/cpp/language/user_literal) that
     allow passing keyword arguments to `pybind11` functions.

   - `"figsize"_a=pyList(5, 2.7)`

     An example of passing a keyword argument.  The suffix '_a' creates a
     `py::arg` object which can be assigned a value and is then recognised as a
     keyword argument.  The mplotpp::pyList() function creates a python list.
     Each argument is converted to a `py::object`.
*/
int main()
{
  py::scoped_interpreter guard;

  py::array_t<double> x(100);
  for (size_t i = 0; i < x.size(); ++i) {
    x.mutable_at(i) = 2.0 * i / double(x.size() - 1);
  }
  py::array_t<double> x_2(x.size()), x_3(x.size());
  for (size_t i = 0; i < x.size(); ++i) {
    x_2.mutable_at(i) = pow(x.at(i), 2);
    x_3.mutable_at(i) = pow(x.at(i), 3);
  }

  py::module_ plt = py::module_::import("matplotlib.pyplot");

  auto [fig, ax] = tuple<2>(plt.attr("subplots")("figsize"_a=pyList(5, 2.7)));

  ax.attr("plot")(x, x, "label"_a="linear");
  ax.attr("plot")(x, x_2, "label"_a="quadratic");
  ax.attr("plot")(x, x_3, "label"_a="cubic");
  ax.attr("set_xlabel")("x label");
  ax.attr("set_ylabel")("y label");
  ax.attr("set_title")("Multiple curves");
  ax.attr("legend")();
  plt.attr("show")();


}