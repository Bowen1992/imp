
import imp_module
from SCons.Script import Glob, Dir, File, Builder, Action, Exit
import os
import sys
import re

# 1. Workaround for SWIG bug #1863647: Ensure that the PySwigIterator class
#    (SwigPyIterator in 1.3.38 or later) is renamed with a module-specific
#    prefix, to avoid collisions when using multiple modules
# 2. If module names contain '.' characters, SWIG emits these into the CPP
#    macros used in the director header. Work around this by replacing them
#    with '_'. A longer term fix is not to call our modules "IMP.foo" but
#    to say %module(package=IMP) foo but this doesn't work in SWIG stable
#    as of 1.3.36 (Python imports incorrectly come out as 'import foo'
#    rather than 'import IMP.foo'). See also IMP bug #41 at
#    https://salilab.org/imp/bugs/show_bug.cgi?id=41
def _action_patch_swig_wrap(target, source, env):
    lines = file(source[0].path, 'r').readlines()
    repl1 = '"swig::%s_PySwigIterator *"' % env['IMP_MODULE_PREPROC']
    repl2 = '"swig::%s_SwigPyIterator *"' % env['IMP_MODULE_PREPROC']
    orig = 'SWIG_IMP.%s_WRAP_H_' % env['IMP_MODULE']
    repl = 'SWIG_IMP_%s_WRAP_H_' % env['IMP_MODULE']
    fh= file(target[0].path, 'w')
    for line in lines:
        line = line.replace('"swig::PySwigIterator *"', repl1)
        line = line.replace('"swig::SwigPyIterator *"', repl2)
        line = line.replace(orig, repl)
        line = line.replace("wrap.h", "patched_wrap.h")
        fh.write(line.replace('"swig::SwigPyIterator *"', repl2))
    fh.close()

def _print_patch_swig_wrap(target, source, env):
    print "Patching swig file"

PatchSwig = Builder(action=Action(_action_patch_swig_wrap,
                                _print_patch_swig_wrap))


def _action_swig_file(target, source, env):
    vars= imp_module.make_vars(env)
    alldeps=imp_module.expand_dependencies(env, env['IMP_REQUIRED_MODULES'])
    preface=["""// Generated file, do not edit, edit the .in instead
%%module(directors="1") "%s"

%%{
#include "IMP.h"
"""%vars['module_include_path'].replace("/", ".")]
    for d in alldeps:
        if d != "kernel":
            preface.append("#include \"IMP/%s.h\""% d)
    if vars['module'] != 'kernel':
        preface.append("#include \"%(module_include_path)s.h\""%vars)
    preface.append("""%%}
%%include "%(module_include_path)s/config.h"
%%include "std_vector.i"
%%include "std_string.i"
%%include "std_pair.i"

%%include "IMP_macros.i"
%%include "IMP_exceptions.i"
%%include "IMP_directors.i"
%%include "IMP_keys.i"
%%include "IMP_refcount.i"
%%include "IMP_streams_kernel.i"
%%include "IMP_streams.i"
%%include "IMP_decorators.i"
%%include "IMP_typemaps.i"

%%include "typemaps.i"

/* Don't wrap classes that provide no methods usable in Python */
%%ignore IMP::ValidDefault;
%%ignore IMP::NullDefault;
%%ignore IMP::UninitializedDefault;
%%ignore IMP::Comparable;
"""%vars)
    preface.append("""%{
#ifdef NDEBUG
#error "The python wrappers must not be built with NDEBUG"
#endif
%}
""")

    for d in alldeps:
        preface.append("%%import %s.i"% d)
    preface.append(open(source[0].abspath, "r").read())
    open(target[0].abspath, "w").write("\n".join(preface))

def _print_swig_file(target, source, env):
    print "Generating swig preface"

SwigPreface = Builder(action=Action(_action_swig_file,
                                    _print_swig_file))


def _action_simple_swig(target, source, env):
    vars= imp_module.make_vars(env)
    cppflags= ""
    for x in env.get('CPPFLAGS', []):
        if x.startswith("-I") or x.startswith("-D"):
            cppflags= cppflags+" " + x

    base="swig -interface _IMP%(module_suffix)s -DPySwigIterator=%(PREPROC)s_PySwigIterator -DSwigPyIterator=%(PREPROC)s_SwigPyIterator -python -c++ -naturalvar "%vars
    #print base
    out= "-o "+ target[0].abspath
    doti= source[0].abspath
    includes= " -I"+Dir("#/build/swig").abspath+" "+" ".join(["-I"+str(x) for x in env.get('CPPPATH', []) if not x.startswith("#")]) #+ " -I"+Dir("#/build/include").abspath
    # scons puts cppflags before includes, so we should too
    command=base + " " +out + " "\
         + " " +cppflags+ " -Ibuild/include "+ includes + " -DIMP_SWIG " + doti
    env.Execute(command)

def _print_simple_swig(target, source, env):
    print "Generating swig file"

SwigIt = Builder(action=Action(_action_simple_swig,
                                _print_simple_swig))


def _get_swig_version(env):
    """Run the SWIG command line tool to get and return the version number"""
    #print "swig version"
    out = os.popen('swig' + ' -version').read()
    match = re.search(r'SWIG Version\s+(\S+)$', out, re.MULTILINE)
    print match
    if match:
        #print "Found " + match.group(1)
        return match.group(1)
    else:
        return ""


def _check(context):
    context.Message('Checking for swig version ')
    try:
        out = os.popen('swig' + ' -version').read()
        match = re.search(r'SWIG Version\s+(\S+)$', out, re.MULTILINE)
        version= match.group(1)
    except:
        print sys.exc_info()
        context.Result("failed")
        return False
    sv= version.split(".")
    v= [int(sv[0]), int(sv[1]), int(sv[2])]
    success = v> [1,3,24]
    if success:
        context.Result(version)
        return True
    else:
        return False

def configure_check(env):
    custom_tests = {'CheckSWIG':_check}
    conf = env.Configure(custom_tests=custom_tests)
    if not env.GetOption('clean') and not env.GetOption('help') \
       and conf.CheckSWIG() is False:
        print """SWIG with a version > 1.3.34 must be installed to support python. Please make sure 'swig' is found in the path passed to scons."""
        env['python']=False
    conf.Finish()
