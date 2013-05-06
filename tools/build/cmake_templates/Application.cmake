# Autogenerated file, run tools/build/setup_cmake.py to regenerate

imp_get_process_exit_code("setup_application %(name)s" status ${PROJECT_BINARY_DIR}
                          COMMAND ${PROJECT_SOURCE_DIR}/tools/build/setup_application.py
                          --name=%(name)s
                          --datapath=${IMP_DATAPATH}
                           --source=${PROJECT_SOURCE_DIR})
if(${status} EQUAL 0)
message("Application IMP.%(name)s ok")
include_directories(%(includepath)s)
link_directories(%(libpath)s)

%(bins)s

set(pybins %(pybins)s)
foreach (pybin ${pybins})
  install(PROGRAMS ${pybin} DESTINATION ${CMAKE_INSTALL_BINDIR})
endforeach(pybin)

set(pytests %(pytests)s)
foreach (test ${pytests})
  GET_FILENAME_COMPONENT(name ${test} NAME)
  add_test("%(name)s.${name}" ${IMP_TEST_SETUP} python ${test})
  set_tests_properties("%(name)s.${name}" PROPERTIES LABELS "IMP.%(name)s;test")
endforeach(test)


elseif(${status} EQUAL 1)
message("Application %(name)s disabled")
else()
message(FATAL_ERROR "setup_application failed ${status}")
endif()
