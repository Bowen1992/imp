# include IMP build utility functions:
import scons_tools
import scons_tools.boost
import scons_tools.cgal
import scons_tools.swig
import scons_tools.standards
import scons_tools.endian
import scons_tools.gcc
import scons_tools.modeller_test
import scons_tools.doxygen
import scons_tools.application
import scons_tools.test
import scons_tools.config_py
import scons_tools.build_summary
import atexit
from SCons import Script

# We need scons 0.98 or later
EnsureSConsVersion(0, 98)

# Set up build environment:
vars = Variables('config.py')
scons_tools.add_common_variables(vars, "imp")
env = scons_tools.MyEnvironment(variables=vars,
                                tools=["default", "swig"],
                                toolpath=["scons_tools"])
env['IMP_ENABLED']=[]
env['IMP_DISABLED']=[]
env['IMP_BUILD_SUMMARY']=""
env['IMP_CONFIGURATION']=[]
env['IMP_VARIABLES']=vars
env['IMP_EXTERNAL_LIBS']=[]

if not env.GetOption('help'):

    if env.get('repository', None) is not None:
        Repository(env['repository'])

    env.AddMethod(scons_tools.imp_module.IMPModuleBuild)
    env.AddMethod(scons_tools.application.IMPCPPApplication)
    env.AddMethod(scons_tools.application.IMPApplicationTest)
    env.Append(BUILDERS={'IMPApplicationRunTest': scons_tools.test.UnitTest})
    env['IMP_MODULES_ALL']=[]

    if not env.GetOption('clean'):
        if not env.get('COMPILER_OK', None):
            Exit("""
No working compiler found. Please make sure that g++ or another
compiler recognized by scons can be found in your path and that all
the passed compiler options (cxxflags, linkflags) are correct.
""")

    scons_tools.boost.configure_check(env, '1.33')
    if not env.GetOption('clean'):
        if env.get('BOOST_VERSION', None) == None:
            Exit("""
Boost version is required to build IMP, but it could not be found on your system.

In particular, if you have Boost installed in a non-standard location, please use the 'includepath' option to add this location to the search path.  For example, a Mac using Boost installed with MacPorts will have the Boost headers in /opt/local/include, so edit (or create) config.py and add the line

includepath='/opt/local/include'

You can see the produced config.log for more information as to why boost failed to be found.
""")

scons_tools.checks.handle_optional_lib(env, "ANN", "ANN",
                                       "ANN/ANN.h")
scons_tools.checks.handle_optional_lib(env, "NetCDF", "netcdf_c++",
                                       "netcdfcpp.h", extra_libs=['netcdf'])
scons_tools.checks.handle_optional_lib(env, "GSL", "gsl",
                                       "gsl/gsl_multimin.h", extra_libs=['gslcblas'])
scons_tools.checks.handle_optional_lib(env, "CGAL", "CGAL",
                                       ['CGAL/Gmpq.h', 'CGAL/Lazy_exact_nt.h'],
                                       body='CGAL_assertion(1); CGAL::Lazy_exact_nt<CGAL::Gmpq> q;',
                                       extra_libs=['gmp', 'mpfr', 'm','boost_thread-mt', 'boost_thread', 'pthread'])
scons_tools.checks.handle_optional_lib(env, "Boost.FileSystem", "boost_filesystem",
                                       'boost/filesystem/path.hpp',
                                       extra_libs=['libboost_system'],
                                       alternate_name=['boost_filesystem-mt'])
scons_tools.checks.handle_optional_lib(env, "Boost.ProgramOptions", "boost_program_options",
                                       'boost/program_options.hpp',
                                       extra_libs=['libboost_system'],
                                       alternate_name=['boost_program_options-mt'])



if not env.GetOption('help'):
    # various flags depending on compiler versions and things
    scons_tools.cgal.configure_check(env)
    scons_tools.swig.configure_check(env)
    scons_tools.modeller_test.configure_check(env)
    scons_tools.endian.configure_check(env)
    scons_tools.doxygen.configure_check_doxygen(env)
    scons_tools.doxygen.configure_check_dot(env)
    scons_tools.gcc.configure_check_visibility(env)
    scons_tools.gcc.configure_check_hash(env)

    # Make these objects available to SConscript files:
    Export('env')

    scons_tools.standards.setup_standards(env)

    env.Append(BUILDERS={'IMPConfigPY':scons_tools.config_py.ConfigPY})
    config_py=env.IMPConfigPY(target=["#/config.py"],
                              source=[env.Value("#".join(env['IMP_CONFIGURATION']))])

    SConscript('kernel/SConscript')

SConscript('modules/SConscript')
if not env.GetOption('help'):
    SConscript('applications/SConscript')
    # This must be after the other SConscipt calls so that it knows about all the generated files
    if env['doxygen']:
        SConscript('doc/SConscript')
    SConscript('tools/SConscript')

    env.Alias(env.Alias('test'), [env.Alias('examples-test')])

    Clean('all', ['build'])
    Clean('all', Glob('scons_tools/*.pyc')\
              + Glob('tools/*.pyc'))

    env.Alias(env.Alias('all'), 'tools')
    env.Default(env.Alias('all'))

    scons_tools.build_summary.setup(env)


unknown = vars.UnknownVariables()
if unknown:
    print "Unknown variables: ", unknown.keys()
    print "Use 'scons -h' to get a list of the accepted variables."
    Exit(1)
Help("""
Available command-line options:
(These can also be specified in regular Python syntax by creating a file
called 'config.py' in this directory.)
""")
Help(vars.GenerateHelpText(env))

Help("""
Type: 'scons' to build the IMP kernel and all configured modules (i.e. those
              with no unmet dependencies);
      'scons test' to run unit tests for the kernel and all configured modules;
      'scons install' to install the kernel and all configured modules;
      'scons doc{-install}' to build (and optionally install) doc

Other useful targets:
      '[kernel,modulename]-test' to test all modules, the kernel, or a particular module
      '[kernel, modulename]-test-examples' to test the examples for a particular module or the kernel
      'all' to build and test everything (and clean up everything in conjunction with '-c')

Infrequently changing settings can be stored in a 'config.py' file in the build directory. An example is provided in tools/example-config.py.
""")
