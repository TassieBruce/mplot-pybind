
#include <cmath>
#include <mplot++/utilities.h>
#include <pybind11/eigen.h>

namespace mp = mplotpp;
namespace py = pybind11;
using namespace py::literals;

template<class T>
inline T
sqr(T x)
{
  return x * x;
}

int
main()
{
  py::scoped_interpreter guard;

  double delta = 0.025;
  auto x = mp::arange(-3.0, 3.0001, delta);
  auto y = mp::arange(-2.0, 2.0001, delta);

  /*
    mplotpp::meshgrid() behaves a lot like numpy.meshgrid.  Here X will be
    sized as if declared by 
    ```
    Eigen::ArrayXXd X(y.size(), x.size());
    ```
    and similarly for Y.  Each row of X is a copy of x and each column of Y is
    a copy of y.
  */
  auto [X, Y] = mp::meshgrid(x, y);
  auto Z1 = (-X.pow(2) - Y.pow(2)).exp();
  auto Z2 = (-(X - 1).pow(2) - (Y - 1).pow(2)).exp();
  auto Z = (Z1 - Z2) * 2;

  py::module_ plt = py::module_::import("matplotlib.pyplot");

  auto [fig, ax] = mp::tuple<2>(plt.attr("subplots")());
  auto CS = ax.attr("contourf")(X, Y, Z);
  auto cbar = fig.attr("colorbar")(CS);
  cbar.attr("ax").attr("set_ylabel")("Height");
  ax.attr("set_xlabel")("$x$");
  ax.attr("set_ylabel")("$y$");
  ax.attr("set_title")("Shaded contours");

  plt.attr("show")();
}