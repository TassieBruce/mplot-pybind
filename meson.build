
project('mplot++', 'cpp',
	default_options : ['cpp_std=c++17', 'werror=true'],
        version: '0.2.0',
        meson_version: '>=0.54.1')

project_brief = 'A C++ interface to python\'s matplotlib using pybind11'

author = 'com.gmail.tassiebruce'

inc_dir = include_directories('.')

# subdir meson.build files should append files to be documented to doxygen_input
readmefile = join_paths(meson.current_source_dir(), 'README.md')
doxygen_input = [readmefile]

subdir('mplot++')
subdir('examples')
subdir('doc')