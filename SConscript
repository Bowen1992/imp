Import('env')

# Uncomment if you need to check for an external library on the system when
# building this module.
#
# If the library is needed to build your module,
# specify required_dependencies=['ExampleLib'] in the IMPModuleBuild call below.
#
# If it is only optional, use 'optional_dependencies' instead, and protect
# C++ code that optionally uses that dependency
# with "#ifdef IMP_USE_EXAMPLELIB".
#
# See IMP.gsl for a working example of an external library dependency
# (it requires GSL, the GNU Scientific Library).
#
#import scons_tools.dependency
#scons_tools.dependency.add_external_library(env, "ExampleLib",
#                                            "example_lib_name",
#                                            "example_lib_header.h")


env.IMPModuleBuild(version='SVN',
                   # The required modules are the other IMP modules that must be built to use this one
                   required_modules=['container', 'core', 'atom','domino'])
