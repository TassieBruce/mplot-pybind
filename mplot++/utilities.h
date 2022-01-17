#pragma once

#include <pybind11/embed.h>
#include <string>

namespace mplotpp {

/**
   @brief Get the str representation of a python object

   @param obj The python object
   @return  The result of `str(obj)` in python
*/
std::string
str(const pybind11::object& obj);

/**
   @brief Get the repr representation of a python object

   @param obj The python object
   @return  The result of `repr(obj)` in python
*/
std::string
repr(const pybind11::object& obj);

/**
   @private
   @brief Helper struct for function pyList
*/
struct PyListGenerator
{
  pybind11::list list;

  template<class T>
  PyListGenerator& operator+=(const T& obj);
};

template<class T>
PyListGenerator&
PyListGenerator::operator+=(const T& obj)
{
  list.append(pybind11::cast(obj));
  return *this;
}

/**
   @brief Convert a list of c++ objects to a python list

   @tparam Args The parameter pack allowing multiple arguments
   @param args A variable length list of arguments of arbitrary type.  Each
   argument must be convertible to `pybind11::object` using `pybind11::cast()`
   @return  A python list containing the arguments
*/
template<typename... Args>
pybind11::list
pyList(Args... args)
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
   where `a` and `b` are each `pybind11::object` instances.  If `v` does
   not wrap a sequence of length 2, the above will throw `mplotpp::Error`

   @tparam N The size of the tuple to extract, which must be known at compile
   time.
   @param obj The python object to extract from.  This should be a sequence of
   length `N`, where by sequence we mean any python object that defines the
   `__getitem__` attribute.
   @return A `std::tuple` containing `N` `pybind11::object` objects
   @throw Error if obj does not wrap a python sequence of length `N`.
*/
template<size_t N>
auto
tuple(const pybind11::object& obj)
{
  if (not(pybind11::hasattr(obj, "__getitem__") and pybind11::len(obj) == N)) {
    throw(std::runtime_error("Expected python sequence of length " +
                             std::to_string(N) + " but have " + str(obj)));
  }
  return tuple_impl(obj, std::make_index_sequence<N>{});
}

}