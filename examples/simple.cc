
#include <mplot++/mplot++.h>
#include <pybind11/stl.h> // Needed to cast from std::vector to python list
#include <vector>

namespace mp = mplotpp;
namespace py = pybind11;

int
main()
{
  /*
    Create some data to plot.  We'll see later that it's often better to use
    Eigen::VectorXd than std::vector<double>
  */
  std::vector<double> x{0, 1, 2, 3};
  std::vector<double> y{0, 1, 4, 9};

  /*
    Initialise the embedded python interpreter.  It will be finalised
    when the variable `guard` goes out of scope
  */
  py::scoped_interpreter guard;

  /*
    Equivalent to '`import matplotlib.pyplot as plt`' in python
  */
  py::module_ plt = py::module_::import("matplotlib.pyplot");

  /*
    `plt.attr("subplots")()` is equivalent to '`plt.subplots()`' in python.  In
    `pybind11` (here aliased as `py`) the class `py::module_` inherits from
    `py::object` which holds a reference counted reference to a python object.
    A method of `py::object` is `attr(const char* name)` which finds the python
    attribute named `name` and returns it as a `py::object`.  The class
    `py::object` also has an `operator()` method that accepts an arbitrary
    number of arguments of any type.  Each argument is cast to python (using the
    function `py::cast()`) and the resulting argument list passed to the python
    object's `__call__` method.  

    The `mplotpp::tuple<N>(pyobj)` template function converts any python
    sequence object to a `std::tuple`.  A runtime error will be thrown if
    `pyobj` is not a sequence (list, tuple, etc.) of length `N`.  The elements
    of the tuple can then be extracted using structured binding
    (https://en.cppreference.com/w/cpp/language/structured_binding) (a feature
    introduced in `c++17`)
  */
  auto [fig, ax] = mp::tuple<2>(plt.attr("subplots")());

  /*
    Call method `plot` of python object `ax` with three arguments.
    `py::cast(x)`  Create a `py::object` from a `c++` object.  To cast a
    standard library container (e.g., `std::vector<double>`, which is cast to a
    python list) the statement `#include <pybind11/stl.h>` is required.  The
    data in `x` will be copied.
  */
  ax.attr("plot")(py::cast(x), py::cast(y), "r");
  plt.attr("show")();
}