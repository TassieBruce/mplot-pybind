
#include <mplot++/mplot++.h>
#include <pybind11/eigen.h>

namespace mp = mplotpp;
namespace py = pybind11;

/*
  Gives access to user-defined literals (see
  https://en.cppreference.com/w/cpp/language/user_literal) that allow passing
  keyword arguments to `pybind11` functions.
*/
using namespace py::literals;

int
main()
{
  py::scoped_interpreter guard;

  /*
    While `simple.cc` used std::vector to hold data to plot, it is usually more
    efficient to use arrays from the Eigen package.  Here we use
    mplotpp::arange() to create an Eigen array.  Since the arguments are of type
    double, x will be of type Eigen::ArrayXd.  Eigen arrays can be manipulated
    in ways very analagous to how one would use numpy arrays in python.  Most
    importantly, when used as arguments to a python object's method, the data is
    not copied but referenced.
  */
  auto x = mp::arange(0.0, 2.01, 0.02);
  auto x_2 = x.pow(2);
  auto x_3 = x.pow(3);

  py::module_ plt = py::module_::import("matplotlib.pyplot");

  /*
    An example of passing a keyword argument.  The suffix '_a' on string
    "figsize" creates a `py::arg` object which can be assigned a value and is
    then recognised as a keyword argument.  Here the value is created using
    mplotpp::list to create a python list.
  */
  auto [fig, ax] =
    mp::tuple<2>(plt.attr("subplots")("figsize"_a = mp::list(5, 2.7)));

  /*
    Another example of a keyword argument.  Note that the data from the Eigen
    Array x is not copied but referenced.
  */
  ax.attr("plot")(x, x, "label"_a = "linear");
  ax.attr("plot")(x, x_2, "label"_a = "quadratic");
  ax.attr("plot")(x, x_3, "label"_a = "cubic");
  ax.attr("set_xlabel")("x label");
  ax.attr("set_ylabel")("y label");
  ax.attr("set_title")("Multiple curves");
  ax.attr("legend")();
  plt.attr("show")();
}