
#include <mplot++/utilities.h>
#include <iostream>

using namespace mplotpp;
namespace py = pybind11;

int
main()
{
  py::scoped_interpreter guard;

  std::cout << str(arange(-3, 3, 1)) << std::endl;
  std::cout << str(arange(-3.0, 3.0, 0.5)) << std::endl;
  std::cout << str(arange(-3.0, 3.000000001, 0.5)) << std::endl;
  std::cout << str(arange(3.0, -3.0, 0.5)) << std::endl;
  std::cout << str(arange(3.0, -3.0, -0.5)) << std::endl;
  std::cout << str(arange(3.0, -3.0, 0.0)) << std::endl;

}