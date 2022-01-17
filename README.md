# mplot-pybind
Graph plotting from C++ using pybind11 to access python matplotlib

## Introduction

This repository illustrates how to plot graphs from c++ using
[pybind11](https://github.com/pybind/pybind11) to access python
[matplotlib](https://matplotlib.org/).  It includes some examples and a small
c++ library that simplifies some aspects. 

## pybind11

The `pybind11` [documentation](https://pybind11.readthedocs.io/en/stable/index.html) descibes itself as

> pybind11 is a lightweight header-only library that exposes C++ types in Python
> and vice versa, mainly to create Python bindings of existing C++ code. Its
> goals and syntax are similar to the excellent Boost.Python library by David
> Abrahams: to minimize boilerplate code in traditional extension modules by
> inferring type information using compile-time introspection.

In addition to it's primary goal of creating python bindings of existing `c++`
code, `pybind11` has the ability to embed a python interpreter in a `c++`
program, and it is this feature that we will make use of here.

To install just the `c++` headers (omitting the python-side install), do the
following.

- Install the Python header files and static library.  On debian-based systems
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
  (`sudo` is not required in the final step if you have write access to `/usr/local`)

## The examples

The directory `examples/` of this repository contains examples of 