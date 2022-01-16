# Build Documentation
# maybe see https://gitlab.freedesktop.org/libinput/libinput/blob/a52f0db3c54b093a2c44dce37ea6dd5582a19c5a/doc/api/meson.build

doxygen = find_program('doxygen', required: false)

if get_option('enable-docs') and doxygen.found()
  message('Documentation enabled')
  conf_data = configuration_data()
  conf_data.set('DOXYGEN_INPUT', ' \\\n  '.join(doxygen_input))
  conf_data.set('DOXYGEN_OUTPUT', meson.current_build_dir())
  conf_data.set('DOXYGEN_README', readmefile)
  conf_data.set('DOXYGEN_NAME', meson.project_name())
  conf_data.set('DOXYGEN_VERSION', meson.project_version())
  conf_data.set('DOXYGEN_BRIEF', project_brief)

  doxyfile = configure_file(
    input: 'Doxyfile.in',
    output: 'Doxyfile',
    configuration: conf_data
  )

  run_target('doc',
              command: [doxygen, doxyfile]
  )
endif
