# mplot-pybind
Graph plotting from C++ using python matplotlib via pybind11.

## Introduction

This repository illustrates how to plot graphs from c++ using
[pybind11](https://github.com/pybind/pybind11) to access python
[matplotlib](https://matplotlib.org/).  It includes some examples and a small
c++ library that simplifies some aspects. 

Hopefully the included examples illustrate that it should be possible to simply translate virtually any python `matplotlib` recipe into `c++`.



## Prerequisites

On a Debian-based system, the following should install everything needed to
build and run the examples and library.
```
sudo apt install pybind11-dev python3-matplotlib meson
```
Of course you also need a `c++` compiler.  You can choose
```
sudo apt install g++
```
for the Gnu compiler or 
```
sudo apt install clang
```
for `clang++`.  Make sure the compiler is capable of the standard `c++17`, which
started with `g++` version 8 and `clang` version 5.  This is the case for either
compiler packaged with Debian 10 (buster, released in July 2019) or later and recent Debian-derived distributions such as Ubuntu.

## pybind11

The `pybind11`
[documentation](https://pybind11.readthedocs.io/en/stable/index.html) descibes
itself as

> pybind11 is a lightweight header-only library that exposes C++ types in Python
> and vice versa, mainly to create Python bindings of existing C++ code. Its
> goals and syntax are similar to the excellent Boost.Python library by David
> Abrahams: to minimize boilerplate code in traditional extension modules by
> inferring type information using compile-time introspection.

In addition to it's primary goal of creating python bindings of existing `c++`
code, `pybind11` has the (less well documented) ability to do the reverse and
embed a python interpreter in a `c++` program.  It is this feature that we will
make use of here.

The [boost python
library](https://www.boost.org/doc/libs/1_78_0/libs/python/doc/html/index.html)
is an alternative candidate to `pybind11`.  The deal maker for `pybind11` is its
ability for `pybind11::object::operator()` to handle an arbitrary number of
arguments and the use of user-defined string literals to handle python keyword
arguments in a syntactically nice way.  Since it is virtually impossible to use
the python `matplotlib` library without using keyword arguments, this is a very
useful feature for our purposes.

The only downside is that while `boost::python` allows implicit conversion of
many `c++` types to python objects (I assume `namespace py = pybind11;` and
`namespace bpy = boost::python;`)
```
bpy::object x = 3.1416;
bpy::object i = 42;
bpy::object s = "Hello World";
```
while in `pybind11` the casting needs to be explicit using `py::cast()`
```
auto x = py::cast(3.1416);
auto i = py::cast(42);
auto s = py::cast("Hello World");
```
where the compiler should determine the `auto` type to correspond to
`py::object`.


## The examples

### Compiling the examples

First, obtain a copy of this repository, either via `git clone` or downloading
and extracting the zip file.  In a terminal, change directory to the top level
of the extracted repository.  If you are using the Gnu compiler, do
```
$ meson builddir
```
If you are using the clang compiler, meson needs the environment variables
`CC=clang` and `CXX=clang++` set.  Either place these in your `~/.bashrc` file
(and then start a new terminal) for use whenever you use meson, or invoke meson
with
```
$ CC=clang CXX=clang++ meson builddir
```
Build the library of useful utilities and examples with
```
$ ninja -C builddir
```
To run a python example, for example
```
$ python3 ./examples/subplots.py
```
and for the `c++` version
```
$ ./builddir/examples/subplots
```

If you find the small library of utilities useful, it can be installed to
`/usr/local` using
```
$ ninja -C builddir install
```
The documentation to the utility library `mplot++` is built using
```
$ ninja -C builddir doc
```
and accessed, for example, by
```
$ firefox ./builddir/doc/html/index.html
```

### Notes on the examples

The directory `examples/` of this repository contains sample plots created both
in python and `c++`.  Here are some notes on features found in the `c++
examples.

- `py::scoped_interpreter guard;`

  This initialises the embedded python interpreter.  It will be finalised
  when the variable `guard` does out of scope

- `py::module_ plt = py::module_::import("matplotlib.pyplot");`

  Equivalent to '`import matplotlib.pyplot as plt`' in python

- `plt.attr("subplots")()`

  Equivalent to '`plt.subplots()`' in python.  In `pybind11` (here aliased as
  `py`) the class `py::module_` inherits from `py::object` which holds a
  reference counted reference to a python object.  A method of `py::object` is
  `attr(const char* name)` which finds the python attribute named `name` and
  returns it as a `py::object`.  The class `py::object` also has an `operator()`
  method that accepts an arbitrary number of arguments of any type.  Each
  argument is cast to python (using the function `py::cast()`) and the resulting
  argument list passed to the python object's `__call__` method.

  Hence this example calls method `subplots` of module `plt` with no arguments.

- `using namespace py::literals;`

  Gives access to [user-defined
  literals](https://en.cppreference.com/w/cpp/language/user_literal) that
  allow passing keyword arguments to `pybind11` functions.

- `ax.attr("plot")(x, x, "label"_a="linear");`

  An example of passing a keyword argument.  The suffix '_a' creates a
  `py::arg` object which can be assigned a value and is then recognised as a
  keyword argument.

- `py::cast(x)`

  Create a `py::object` from many `c++` objects.  To cast a standard library
  container (e.g., `std::vector<double>`, which is cast to a python list) the
  statement `#include <pybind11/stl.h>` is required

- `tuple<2>(plt.attr("subplots")())`

  The `mplotpp::tuple<N>(pyobj)` template function converts any python sequence
  object to a `std::tuple`.  A runtime error will be thrown if `pyobj` is not a
  sequence (list, tuple, etc.) of length `N`.  The elements of the tuple can
  then be extracted using [structured
  binding](https://en.cppreference.com/w/cpp/language/structured_binding) (a
  feature introduced in `c++17`)

- `auto [fig, ax] = tuple<2>(plt.attr("subplots")(2, 2));`

  In this case, the `ax` object will appear to python as a *2x2* numpy array.
  But to `c++` it is just a `py::object`.  The elements can be accessed using
  repeated application of `operator[]` but only if the integer argument is first
  cast to python, for example with
  ```
  ax[py::cast(0)][py::cast(1)];
  ```
  or perhaps more conveniently with something like
  ```
  py::object _0 = py::cast(0);
  py::object _1 = py::cast(1);
  ax[_0][_1];
  ```
  However, it may also be easier to use `mplotpp::tuple` and do 
  ```
  auto [row0, row1] = tuple<2>(ax);
  auto [a00, a01] = tuple<2>(row0);
  auto [a10, a11] = tuple<2>(row1);
  ```
  which is the approach used in `examples/subplots.cc`

- `py::array_t<double> x(100);`

  Creates a `numpy` 1-d array in the python interpreter that is accessible from
  `c++`.  The embedded interpreter must have been initialised before this (via
  `py::scoped_interpreter`).  Bound-checked element access is via `x.at(i)` or
  `x.mutable_at(i)` if an element is to be modified.  Data does not need to be
  copied when later passing to the embedded python, for example as an argument
  to `ax.attr("plot")()`.  See following notes for unchecked element access.

- `auto x_unch = x.unchecked()`

  where `x` is a `py::array_t` as in the previous note.  Creates a proxy object
  `x_unch` which allows unchecked *immutable* element access to `x` using either
  `x_unch[i]` or `x_unch(i)`

- `py::array_t<double> Z({10, 20});`

  Creates a *10x20* `numpy` array in the python interpreter.  Bound-checked
  element access is via `Z.at(i, j)` or `Z.mutable_at(i, j)`.

- `auto Z_unch = Z.mutable_unchecked()`

  where `Z` is a `py::array_t` as in the previous note.  Creates a proxy object
  `Z_unch` which allows unchecked *mutable* element access to `Z` using
  `Z_unch(i, j)`

- For contour plots, the *z* data is contained in a *MxN* array where *M* is the
  number of *y* points and *N* is the number of *x* points.  In python, the most
  efficient way to do this is to first use `numpy.meshgrid()` to create expanded
  arrays *X* and *Y* that are each *MxN* such that *X* has *M* rows, each of
  which is a copy of *x* and *Y* has *N* columns, each of which is a copy of
  *y*.  The *z* data in *Z* can then be efficiently computed using vectorised
  functions that act on *X* and *Y* to produce *Z*.  For example
  ```
  x = np.arange(-3.0, 3.01, 0.2)
  y = np.arange(-2.0, 2.01, 0.2)
  X, Y = np.meshgrid(x, y)
  Z = np.exp(-(X**2 + Y**2))
  ```
  This is done because accessing individual elements of *x*, *y* and *Z* and
  performing arithmetic on those elements is very inefficient in python since
  each operation requires dealing with non-trivial python objects rather than
  machine-level floating point numbers.  However, `c++` does not have those
  overheads so direct access is the most efficient, avoiding the need to create
  unnecessary duplication of the *x* and *y* values.

  Unfortunately, 3D surface plots cannot use the 1-d arrays of just the *x* and
  *y* points but require the full *MxN* arrays of duplicated values.

## Final notes

If a later version of `pybind11` than that available via the package manager is
required, the latest `c++` header library (omitting the python-side install),
can be installed via the following steps.  Note that the version installed above
via the package manager is all that is required for the examples here.

- Install the Python header files and static library.  On Debian-based systems
  this is
  ```
  $ sudo apt install libpython3-dev
  ```
- Download and install the `pybind11` headers
  ```
  $ git clone https://github.com/pybind/pybind11.git
  $ cd pybind11/
  $ mkdir build
  $ cd build
  $ cmake .. -DPYBIND11_TEST=OFF
  $ sudo make install
  ```
  (`sudo` is not required in the final step if you have write access to
  `/usr/local`)