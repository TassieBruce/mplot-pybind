
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

int main()
{
  py::scoped_interpreter guard;

  constexpr size_t size = 100;
  py::array_t<double> x(size);
  for (ssize_t i = 0; i < x.size(); ++i) {
    x.mutable_at(i) = 2.0 * i / double(x.size() - 1);
  }
  py::array_t<double> x_2(size), x_3(size);
  for (size_t i = 0; i < size; ++i) {
    x_2.mutable_at(i) = pow(x.at(i), 2);
    x_3.mutable_at(i) = pow(x.at(i), 3);
  }

  py::module_ plt = py::module_::import("matplotlib.pyplot");

  auto [fig, ax] = tuple<2>(plt.attr("subplots")("figsize"_a=list(5, 2.7)));

  ax.attr("plot")(x, x, "label"_a="linear");
  ax.attr("plot")(x, x_2, "label"_a="quadratic");
  ax.attr("plot")(x, x_3, "label"_a="cubic");
  ax.attr("set_xlabel")("x label");
  ax.attr("set_ylabel")("y label");
  ax.attr("set_title")("Multiple curves");
  ax.attr("legend")();
  plt.attr("show")();
}