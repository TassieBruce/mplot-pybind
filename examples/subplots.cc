
/// @cond
#include <mplot++/utilities.h>
#include <pybind11/embed.h>
#include <pybind11/numpy.h>
#include <vector>
/// @endcond

using namespace mplotpp;
namespace py = pybind11;
using namespace py::literals;

int
main()
{
  py::scoped_interpreter guard;

  constexpr size_t size = 100;
  py::array_t<double> x(size);
  for (ssize_t i = 0; i < x.size(); ++i) {
    x.mutable_at(i) = 2.0 * i / double(x.size() - 1);
  }
  py::array_t<double> x_2(size), x_3(size), x_4(size);
  for (size_t i = 0; i < size; ++i) {
    x_2.mutable_at(i) = pow(x.at(i), 2);
    x_3.mutable_at(i) = pow(x.at(i), 3);
    x_4.mutable_at(i) = pow(x.at(i), 4);
  }

  py::module_ plt = py::module_::import("matplotlib.pyplot");

  auto [fig, ax] = tuple<2>(plt.attr("subplots")(2, 2));
  auto [row0, row1] = tuple<2>(ax);
  auto [ax00, ax01] = tuple<2>(row0);
  auto [ax10, ax11] = tuple<2>(row1);

  fig.attr("suptitle")("Subplots");
  ax00.attr("plot")(x, x);
  ax00.attr("text")(0.05,
                    0.95,
                    "$x$",
                    "ha"_a = "left",
                    "va"_a = "top",
                    "transform"_a = ax00.attr("transAxes"));
  ax01.attr("plot")(x, x_2);
  ax01.attr("text")(0.05,
                    0.95,
                    "$x^2$",
                    "ha"_a = "left",
                    "va"_a = "top",
                    "transform"_a = ax01.attr("transAxes"));
  ax10.attr("plot")(x, x_3);
  ax10.attr("text")(0.05,
                    0.95,
                    "$x^3$",
                    "ha"_a = "left",
                    "va"_a = "top",
                    "transform"_a = ax10.attr("transAxes"));
  ax11.attr("plot")(x, x_4);
  ax11.attr("text")(0.05,
                    0.95,
                    "$x^4$",
                    "ha"_a = "left",
                    "va"_a = "top",
                    "transform"_a = ax11.attr("transAxes"));

  ax10.attr("set_xlabel")("$x$");
  ax11.attr("set_xlabel")("$x$");

  ax00.attr("set_ylabel")("$f(x)$");
  ax10.attr("set_ylabel")("$f(x)$");
  plt.attr("show")();
}