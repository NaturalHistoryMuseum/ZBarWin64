#!/usr/bin/env python
import platform
import shutil
import sys

from setuptools import setup, Extension

if 'bdist' in sys.argv:
    # Include zlib DLLS in binary distribution for Windows
    assert('64bit' == platform.architecture()[0])
    shutil.copy('../x64/Release/libzbar64-0.dll', '.')
    shutil.copy('../zbar/libiconv/dll_x64/libiconv.dll', '.')

setup(
    name = 'zbar',
    version = '0.10',
    author = 'Jeff Brown',
    author_email = 'spadix@users.sourceforge.net',
    url = 'http://zbar.sourceforge.net',
    description = 'read barcodes from images or video',
    license = 'LGPL',
    long_description = open('README').read(),
    classifiers = [
        'License :: OSI Approved :: GNU Library or Lesser General Public License (LGPL)',
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'Environment :: Console',
        'Environment :: X11 Applications',
        'Environment :: Win32 (MS Windows)',
        'Operating System :: POSIX',
        'Operating System :: Unix',
        'Operating System :: Microsoft :: Windows',
        'Topic :: Communications',
        'Topic :: Multimedia :: Graphics',
        'Topic :: Software Development :: Libraries',
    ],
    ext_modules = [
        Extension('zbar', [
                'zbarmodule.c',
                'enum.c',
                'exception.c',
                'symbol.c',
                'symbolset.c',
                'symboliter.c',
                'image.c',
                'processor.c',
                'imagescanner.c',
                'decoder.c',
                'scanner.c',
                ],
            libraries = [ 'libzbar64-0' ],
            include_dirs = ['../include/'],
            library_dirs = ['../x64/Release']
        ),
    ],
    data_files = [('', ['libzbar64-0.dll', 'libiconv.dll'])],
)
