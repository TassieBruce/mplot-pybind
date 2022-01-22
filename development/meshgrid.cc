
#include <Eigen/Dense>
#include <iostream>
#include <mplot++/utilities.h>
#include <tuple>

namespace ei = Eigen;
namespace mp = mplotpp;

template<class T>
std::tuple<Eigen::Array<T, Eigen::Dynamic, Eigen::Dynamic>,
           Eigen::Array<T, Eigen::Dynamic, Eigen::Dynamic>>
meshgrid(const Eigen::Array<T, Eigen::Dynamic, 1>& x,
         const Eigen::Array<T, Eigen::Dynamic, 1>& y)
{
  Eigen::Array<T, Eigen::Dynamic, Eigen::Dynamic> X(y.size(), x.size());
  Eigen::Array<T, Eigen::Dynamic, Eigen::Dynamic> Y(y.size(), x.size());
  for (ssize_t i = 0; i < X.rows(); ++i) {
    X.row(i) = x;
  }
  for (ssize_t j = 0; j < Y.cols(); ++j) {
    Y.col(j) = y;
  }

  return std::make_tuple(X, Y);
}

int
main()
{
  ei::ArrayXd x = mp::arange(-1.0, 1.01, 1.0);
  ei::ArrayXd y = mp::arange(-2.0, 2.01, 1.0);
  std::cout << "x = " << x.transpose() << std::endl;
  std::cout << "y = " << y.transpose() << std::endl;

  {
    ei::ArrayXXd X(y.size(), x.size());
    for (ssize_t i = 0; i < X.rows(); ++i) {
      X.row(i) = x;
    }
    std::cout << "X =\n" << X << std::endl;

    ei::ArrayXXd Y(y.size(), x.size());
    for (ssize_t j = 0; j < Y.cols(); ++j) {
      Y.col(j) = y;
    }
    std::cout << "Y =\n" << Y << std::endl;
  }

  {
    auto [X, Y] = meshgrid(x, y);
    std::cout << "X =\n" << X << std::endl;
    std::cout << "Y =\n" << Y << std::endl;
    std::cout << "X + 1 =\n" << (X + 1) << std::endl;
    auto Z1 = (-X.pow(2) - Y.pow(2)).exp();
    std::cout << "Z1 =\n" << Z1 << std::endl;
    auto Z2 = (-(X - 1).pow(2) - (Y - 1).pow(2)).exp();
    std::cout << "Z2 =\n" << Z2 << std::endl;
    auto Z = (Z1 - Z2) * 2;
    std::cout << "Z =\n" << Z << std::endl;
  }
}