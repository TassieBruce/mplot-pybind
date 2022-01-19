
#include <cmath>
#include <mplot++/utilities.h>
#include <pybind11/numpy.h>

using namespace mplotpp;
namespace py = pybind11;
using namespace py::literals;

int
main()
{
  using array = py::array_t<double>;

  py::scoped_interpreter guard;

  // Import modules
  auto plt = py::module_::import("matplotlib.pyplot");
  auto cm = py::module_::import("matplotlib").attr("cm");
  auto LinearLocator =
    py::module_::import("matplotlib.ticker").attr("LinearLocator");

  auto [fig, ax] = tuple<2>(
    plt.attr("subplots")("subplot_kw"_a = py::dict("projection"_a = "3d")));

  // Make data.
  double delta = 0.25;
  array xpts = arange(-5.0, 5.01, delta);
  array ypts = arange(-5.0, 5.01, delta);
  array Z({ ypts.size(), xpts.size() });
  // Unlike contour plots, surface plots require meshgrid-like X and Y
  array X({ ypts.size(), xpts.size() });
  array Y({ ypts.size(), xpts.size() });

  for (size_t i = 0; i < ypts.size(); ++i) {
    double y = ypts.at(i);
    for (size_t j = 0; j < xpts.size(); ++j) {
      double x = xpts.at(j);
      Z.mutable_at(i, j) = sin(sqrt(x * x + y * y));
      X.mutable_at(i, j) = x;
      Y.mutable_at(i, j) = y;
    }
  }

  // Plot the surface.
  auto surf = ax.attr("plot_surface")(X,
                                      Y,
                                      Z,
                                      "cmap"_a = cm.attr("coolwarm"),
                                      "linewidth"_a = 0,
                                      "antialiased"_a = false);
  // Customize the z axis.
  ax.attr("set_zlim")(-1.0, 1.0);
  ax.attr("zaxis").attr("set_major_locator")(LinearLocator(5));
  // A StrMethodFormatter is used automatically
  ax.attr("zaxis").attr("set_major_formatter")("{x:.01f}");

  // Add a color bar which maps values to colors.
  fig.attr("colorbar")(surf, "shrink"_a = 0.5, "aspect"_a = 5);

  plt.attr("show")();
}