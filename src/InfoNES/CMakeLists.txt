aux_source_directory(. libInfoNES_SRC)
add_library (libInfoNES ${libInfoNES_SRC})
target_compile_options(libInfoNES PRIVATE -DENABLE_NES_AUDIO=1)
