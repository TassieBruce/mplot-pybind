
#include <mplot++/utilities.h>
#include <Eigen/Dense>
#include <iostream>
#include <pybind11/eigen.h>
#include <pybind11/embed.h>

namespace ei = Eigen;
namespace py = pybind11;
using namespace py::literals;
namespace mp = mplotpp;

int
main()
{
  ei::MatrixXd m(2, 2);
  m(0, 0) = 3;
  m(1, 0) = 2.5;
  m(0, 1) = -1;
  m(1, 1) = m(1, 0) + m(0, 1);
  std::cout << m << std::endl;

  ei::VectorXd v(2);
  v << 1, 2;

  py::scoped_interpreter guard;

  auto pym = py::cast(m);
  py::print("pym = ", pym);
  py::print("m = ", m);
  py::print("v = ", v);
  py::print("m*v = ", m * v);

  ei::ArrayXd x(4);
  x << 0, 1, 2, 3;
  ei::ArrayXd y = x.pow(2);

  std::cout << "x = " << x << std::endl;
  std::cout << "y = " << y << std::endl;

  py::module_ plt = py::module_::import("matplotlib.pyplot");

  auto [fig, ax] = mp::tuple<2>(plt.attr("subplots")());

  ax.attr("plot")(x, y, "r");
  plt.attr("show")();
}