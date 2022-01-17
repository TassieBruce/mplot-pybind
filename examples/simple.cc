
/// @cond
#include <mplot++/utilities.h>
#include <pybind11/stl.h>
#include <vector>
/// @endcond 

using namespace mplotpp;
namespace py = pybind11;


/**
   @brief A simple plot

   In python, the following plots a very simple graph:
   @include simple.py

   The c++ translation is nearly as simple

   @include simple.cc

   Some notes on the `c++` translation:

   - `py::scoped_interpreter guard;`

     This initialises the embedded python interpreter.  It will be finalised
     when the variable `guard` does out of scope

   - `py::module_ plt = py::module_::import("matplotlib.pyplot");`

     Equivalent to `import matplotlib.pyplot as plt` in python

   - `plt.attr("subplots")()`

     Eqivalent to `plt.subplots()` in python.  `plt.attr("subplots")` gets the
     attribute `"subplots"` of `plt` which is than called using `operator()`
     with no arguments.

   - `tuple<2>(plt.attr("subplots")())`

     The `mplotpp::tuple<N>(pyobj)` template function converts any python
     sequence object to a `std::tuple`.  A runtime error will be thrown if
     `pyobj` is not a sequence (list, tuple, etc.) of length `N`.

   - `py::cast(x)`

     Copy the data from `std::vector<double> x` to a new python list.  This
     requires `#include <pybind11/stl.h>`

   - `ax.attr("plot")(py::cast(x), py::cast(y), "r");`

     Call the `plot` method of `ax` with three positional arguments.
*/
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