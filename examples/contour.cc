
#include <cmath>
#include <mplot++/utilities.h>
#include <pybind11/numpy.h>

using namespace mplotpp;
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
  using array = py::array_t<double>;

  py::scoped_interpreter guard;

  double delta = 0.025;
  array X = arange(-3.0, 3.0001, delta);
  array Y = arange(-2.0, 2.0001, delta);
  array Z({ Y.size(), X.size() });

  auto X_unch = X.unchecked();
  auto Y_unch = Y.unchecked();
  auto Z_mut = Z.mutable_unchecked();
  for (size_t i = 0; i < Y.size(); ++i) {
    double y = Y_unch[i];
    for (size_t j = 0; j < X.size(); ++j) {
      double x = X_unch[j];
      Z_mut(i, j) = (exp(-sqr(x) - sqr(y)) - exp(-sqr(x - 1) - sqr(y - 1))) * 2;
    }
  }

  py::module_ plt = py::module_::import("matplotlib.pyplot");

  auto [fig, ax] = tuple<2>(plt.attr("subplots")());
  auto CS = ax.attr("contourf")(X, Y, Z);
  auto cbar = fig.attr("colorbar")(CS);
  cbar.attr("ax").attr("set_ylabel")("Height");
  ax.attr("set_xlabel")("$x$");
  ax.attr("set_ylabel")("$y$");
  ax.attr("set_title")("Shaded contours");

  plt.attr("show")();
}