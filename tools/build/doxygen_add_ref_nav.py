#!/usr/bin/env python

"""Add navigation to the reference guide HTML output, similarly
   to what is done for the manual (see doxygen_add_nav.py).
"""

from doxygen_add_nav import Docs, Page
import tools
import sys
import os

def all_module_docs(modules):
    return [os.path.join(m, 'doc') for m in modules]

def get_module_readme(module):
    """Make a Page object for a module's top-level README file"""
    p = Page('namespace' + module)
    if module == 'kernel':
        p.out_file_name = 'namespaceIMP'
    else:
        p.out_file_name = 'namespaceIMP_1_1' + module
    p.source_file_name = 'modules/%s/README.md' % module
    return p

def main():
    source = sys.argv[1]
    modules = [m[0] for m in tools.get_modules(source)]
    docs = Docs(xml_dir='doxygen/ref/xml',
                html_dir='doc/ref',
                top_source_dir=source,
                source_subdirs=sys.argv[2:] + all_module_docs(modules))
    for p in docs.pages:
        p.map_location_to_source(source)
    for m in modules:
        docs.pages.append(get_module_readme(m))

    for p in docs.pages:
        docs.add_page_navigation(p)

if __name__ == '__main__':
    main()