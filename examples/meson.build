
source_ext = '.cc'

examples = [
  'simple',
  'multiplePlots',
  'subplots',
  'contour',
  '3dsurface'
]

foreach f : examples
  executable(f, sources: [f + source_ext],
        dependencies: [mplotppdep])
endforeach