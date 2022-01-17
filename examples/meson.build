
source_ext = '.cc'

examples = [
  'simple',
  'multiplePlots'
]

foreach f : examples
  executable(f, sources: [f + source_ext],
        dependencies: [mplotppdep])
  doxygen_input += join_paths(meson.current_source_dir(), f + source_ext)
endforeach