
#include <mplot++/mplot++.h>
#include <pybind11/eigen.h>

namespace mp = mplotpp;
namespace py = pybind11;
using namespace py::literals;

int
main()
{
  py::scoped_interpreter guard;

  auto x = mp::arange(0.0, 2.01, 0.02);
  auto x_2 = x.pow(2);
  auto x_3 = x.pow(3);
  auto x_4 = x.pow(4);

  py::module_ plt = py::module_::import("matplotlib.pyplot");

  /*
    Here the `ax` object will appear to python as a *2x2* numpy array.  But to
    `c++` it is just a `py::object`.  The elements can be accessed using
    repeated application of `operator[]` but only if the integer argument is
    first cast to python, for example with
    ```
    ax[py::cast(0)][py::cast(1)];
    ```
    or perhaps more conveniently with something like
    ```
    py::object _0 = py::cast(0);
    py::object _1 = py::cast(1);
    ax[_0][_1];
    ```
    However, I find it easier to use `mplotpp::tuple` to first extract the rows
    and then the individual axes from the rows.
  */
  auto [fig, ax] = mp::tuple<2>(plt.attr("subplots")(2, 2));
  auto [row0, row1] = mp::tuple<2>(ax);
  auto [ax00, ax01] = mp::tuple<2>(row0);
  auto [ax10, ax11] = mp::tuple<2>(row1);

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