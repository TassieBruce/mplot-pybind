
#include <cmath>
#include <mplot++/utilities.h>
#include <pybind11/eigen.h>

namespace mp = mplotpp;
namespace py = pybind11;
using namespace py::literals;

int
main()
{
  py::scoped_interpreter guard;

  // Import modules
  auto plt = py::module_::import("matplotlib.pyplot");
  auto cm = py::module_::import("matplotlib").attr("cm");
  auto LinearLocator =
    py::module_::import("matplotlib.ticker").attr("LinearLocator");

  auto [fig, ax] = mp::tuple<2>(
    plt.attr("subplots")("subplot_kw"_a = py::dict("projection"_a = "3d")));

  // Make data.
  double delta = 0.25;
  auto x = mp::arange(-5.0, 5.01, delta);
  auto y = mp::arange(-5.0, 5.01, delta);
  auto [X, Y] = mp::meshgrid(x, y);
  auto Z = (X.pow(2) + Y.pow(2)).sqrt().sin();

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