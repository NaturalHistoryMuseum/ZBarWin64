# ZBarWin64
Adds Visual Studio 64-bit DLL build of the zbar Python package.
Tested with [Anaconda] distribution of Python 2.7.x and Visual Studio Community
2013.

1. Build the 64-bit Release target of zbar64

2. Install Anaconda

3. Build the zbar Python wrapper

    cd python
    pip install --upgrade pip
    pip install --upgrade setuptools
    pip install wheel
    build.bat
    pip install dist/zbar-<whatever>.whl
