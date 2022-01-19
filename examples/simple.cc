
#include <mplot++/utilities.h>
#include <pybind11/stl.h>
#include <vector>

using namespace mplotpp;
namespace py = pybind11;


int
main()
{
  std::vector<double> x{0, 1, 2, 3};
  std::vector<double> y{0, 1, 4, 9};

  py::scoped_interpreter guard;
  py::module_ plt = py::module_::import("matplotlib.pyplot");

  auto [fig, ax] = tuple<2>(plt.attr("subplots")());

  ax.attr("plot")(py::cast(x), py::cast(y), "r");
  plt.attr("show")();
}