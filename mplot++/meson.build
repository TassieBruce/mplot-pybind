# install header-only library

headers = [
  'mplot++.h'
]

# Make sure all headers are processed by doxygen
foreach f : headers
  doxygen_input += join_paths(meson.current_source_dir(), f)
endforeach

deps = [
  dependency('python3-embed'),
  dependency('eigen3') 
]

# Dependency for use later
mplotppdep = declare_dependency(
              dependencies: deps,
				      include_directories : include_directories('..')
)

# Make sure headers get installed
install_headers(headers, subdir: 'mplot++')

# Create the mplot++.pc file for pkg-config
pkg = import('pkgconfig')
pkg.generate(
  name: 'mplot++',
  requires: deps,
  description: 'Help for plotting from c++ using matplotlib and pybind11',
  extra_cflags: ['-fvisibility=hidden'],
  version: meson.project_version()
)
