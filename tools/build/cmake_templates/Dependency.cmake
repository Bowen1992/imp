# Autogenerated file, run tools/build/setup_cmake.py to regenerate

if(NOT DEFINED %(pkgname)s_LIBRARIES)

set(CHECK_COMPILES_BODY "%(body)s")

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules("%(pkgname)s_PKGCONF" "%(pkg_config_name)s")

# Include dir
find_path("%(pkgname)s_INCLUDE_DIR"
  NAMES %(headers)s
  PATHS ${%(pkgname)s_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
foreach(lib %(libraries)s)
find_library("${lib}_LIBRARY"
  NAMES ${lib}
  PATHS ${%(pkgname)s_PKGCONF_LIBRARY_DIRS}
)
list(APPEND %(pkgname)s_LIBRARY "${${lib}_LIBRARY}")
endforeach(lib)


# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(%(pkgname)s_PROCESS_INCLUDES %(pkgname)s_INCLUDE_DIR)
set(%(pkgname)s_PROCESS_LIBS %(pkgname)s_LIBRARY)
libfind_process(%(pkgname)s)

if ("${%(pkgname)s_LIBRARY}" MATCHES ".*NOTFOUND.*"
    OR "${%(pkgname)s_INCLUDE_DIR}" MATCHES ".*NOTFOUND.*")
  %(on_failure)s
else()
  check_compiles("_found" "%(pkgname)s" "%(PKGNAME)s" "%(includes)s" "${%(pkgname)s_INCLUDE_DIR}" "${%(pkgname)s_LIBRARY}" %(pkgname)s_ok_ok)
  if(${%(pkgname)s_ok_ok} MATCHES "1")
    message(STATUS "Found %(pkgname)s at \"${%(pkgname)s_INCLUDE_DIR}\" and \"${%(pkgname)s_LIBRARY}\"")
  else()
    %(on_failure)s
  endif()
endif()
else()
message(STATUS "%(pkgname)s already setup")
%(on_setup)s
endif(NOT DEFINED %(pkgname)s_LIBRARIES)
