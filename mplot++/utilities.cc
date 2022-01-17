
#include "mplot++/utilities.h"


namespace mplotpp {

std::string
str(const pybind11::object& obj)
{
  return pybind11::str(obj).cast<std::string>();
}

std::string
repr(const pybind11::object& obj)
{
  return pybind11::repr(obj).cast<std::string>();
}

}