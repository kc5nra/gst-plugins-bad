openal_dep = dependency('openal', version: '>= 1.14', required: get_option('openal'))

if openal_dep.found()
  gstopenal = library('gstopenal',
    'gstopenal.c', 'gstopenalsink.c', 'gstopenalsrc.c',
    c_args: gst_plugins_bad_args,
    include_directories: [configinc, libsinc],
    dependencies: [gstaudio_dep, openal_dep],
    install: true,
    install_dir: plugins_install_dir,
  )
  pkgconfig.generate(gstopenal, install_dir: plugins_pkgconfig_install_dir)
endif
