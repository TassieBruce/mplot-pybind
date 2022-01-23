// Copyright (c) by TassieBruce
// Distributed under the MIT License

#pragma once

#include <limits>
#include <pybind11/embed.h>
#include <pybind11/numpy.h>
#include <Eigen/Dense>
#include <stdexcept>
#include <string>

namespace mplotpp {

/**
   @private
   @brief Helper struct for function list
*/
struct PyListGenerator
{
  pybind11::list list;

  template<class T>
  PyListGenerator& operator+=(const T& obj)
  {
    list.append(pybind11::cast(obj));
    return *this;
  }
};

/**
   @brief Convert a list of c++ objects to a python list

   @tparam Args The parameter pack allowing multiple arguments
   @param args A variable length list of arguments of arbitrary type.  Each
   argument must be convertible to `pybind11::object` using `pybind11::cast()`
   @return  A python list containing the arguments
*/
template<typename... Args>
pybind11::list
list(Args... args)
{
  auto generator = (PyListGenerator() += ... += args);
  return generator.list;
}

/**
   @private
   @brief Implementation of tuple()

   @tparam Is Parameter pack of indices to extract
   @param obj The python object to extract from.
   @return A `std::tuple` containing `obj[Is...]`
*/
template<size_t... Is>
auto
tuple_impl(const pybind11::object& obj, std::index_sequence<Is...>)
{
  return std::make_tuple(pybind11::object(obj[pybind11::cast(Is)])...);
}

/**
   @brief If possible, convert a python object to a `c++` N-tuple of python
   objects

   For example, if `v` is a `pybind11::object` that wraps a python list of
   length 2, the two values can be extracted using
   ```
   auto [a, b] = tuple<2>(v);
   ```
   where `a` and `b` are each `pybind11::object` instances.  If `v` does not
   wrap a sequence of length 2, the above will throw `std::runtime_error`

   @tparam N The size of the tuple to extract, which must be known at compile
   time.
   @param obj The python object to extract from.  This should be a sequence of
   length `N`, where by sequence we mean any python object that defines the
   `__getitem__` attribute.
   @return A `std::tuple` containing `N` `pybind11::object` objects
   @throw std::runtime_error if obj does not wrap a python sequence of length
   `N`.
*/
template<size_t N>
auto
tuple(const pybind11::object& obj)
{
  if (not(pybind11::hasattr(obj, "__getitem__") and pybind11::len(obj) == N)) {
    throw(std::runtime_error("Expected python sequence of length " +
                             std::to_string(N) + " but have " + 
                             pybind11::str(obj).cast<std::string>()));
  }
  return tuple_impl(obj, std::make_index_sequence<N>{});
}

/**
   @brief Return evenly spaced values within a given interval.

   This mimics the behaviour of python's numpy.arange()

   @tparam T The data type
   @param start Start of interval. The interval includes this value.
   @param stop End of interval. For integral types, the interval does not
   include this value.  For floating point types, the interval should not
   normally include this value but there may be some cases where it is included
   due to rounding errors.
   @param step Spacing between values. For any output `out`, this is the
   distance between two adjacent values, `out[i+1] - out[i]`. The default step
   size is 1.
   @return An Eigen::Array containing the desired range.
   @throw std::domain_error if step is zero.
*/
template<class T>
Eigen::Array<T,Eigen::Dynamic,1>
arange(T start, T stop, T step = 1)
{
  if (step == 0) {
    throw(std::domain_error("step is zero in arange()"));
  }
  size_t N = 0;
  if ((stop > start and step > 0) or (stop < start and step < 0)) {
    if (std::numeric_limits<T>::is_integer) {
      N = (stop - start) / step;
    } else {
      N =
        size_t((stop - start) / step - 10 * std::numeric_limits<T>::epsilon()) +
        1;
    }
  }
  Eigen::Array<T,Eigen::Dynamic,1> result(N);
  T x = start;
  for (ssize_t i = 0; i < result.size(); ++i) {
    result.coeffRef(i) = x;
    x += step;
  }

  return result;
}

/**
   @brief Get coordinate arrays from coordinate vectors.

   This mimics the behaviour of python's numpy.meshgrid().  Example usage is
   ```
   auto x = mplotpp::arange(-2.0, 2.01, 0.1);
   auto y = mplotpp::arange(-3.0, 3.01, 0.1);
   auto [X, Y] = mplotpp::meshgrid(x, y);
   ```

   @tparam T The data type
   @param x A 1-D array representing the x-coordinates of a grid
   @param y A 1-D array representing the y-coordinates of a grid
   @return  A tuple (X, Y) where X and Y are each 2-D array's of size MxN where
   M is the length of y and N is the length of x.  Each row of X is a copy of x
   and each column of Y is a copy of y.
*/
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

}