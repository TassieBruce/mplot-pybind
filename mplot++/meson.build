
head_ext = '.h'
source_ext = '.cc'

# There must be a source file and header file in current directory for
# every item in head_and_source.
head_and_source = [
]

# There must be a header file in current directory for every item in
# head_only.
head_only = [
]

# There must be a source file in current directory for every item in
# source_only.
source_only = []

# Produce lists headers and sources from head_and_source, head_only
# and source_only.
headers = []
sources = []
foreach f : head_and_source
  headers += (f + head_ext)
  sources += (f + source_ext)
  doxygen_input += join_paths(meson.current_source_dir(), f + head_ext)
endforeach
foreach f : head_only
  headers += (f + head_ext)
  doxygen_input += join_paths(meson.current_source_dir(), f + head_ext)
endforeach
foreach f : source_only
  sources += (f + source_ext)
endforeach

cc = meson.get_compiler('cpp')

# List all dependencies
deps = [
  dependency('python3-embed')
]

mplotpplib = static_library(
  'mplot++',
  sources: [sources, headers],
  dependencies: deps,
  include_directories : include_directories('..'),
  install: true
)

# Make sure headers get installed
install_headers(headers, subdir: 'mplot++')

# Create dependency for use by examples
mplotppdep = declare_dependency(dependencies: deps,
				     include_directories : include_directories('..'),
				     link_with : mplotpplib)

# Create the mplot++.pc file for pkg-config
pkg = import('pkgconfig')
pkg.generate(
  mplotpplib,
  description: 'mplot++ - ' + project_brief,
  version: meson.project_version()
)
