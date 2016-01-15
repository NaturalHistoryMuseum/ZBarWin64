# ZBarWin64

A Windows 64-bit wheel build of the zbar Python package.

## Installing on Windows 64-bit
Install a release from this repo:

```
https://github.com/NaturalHistoryMuseum/ZBarWin64/releases/download/v0.10/zbar-0.10-cp27-none-win_amd64.whl
```

The wheel was built using Visual C++ 2013. If Python reports an `ImportError`
when `import pydmtx` is run then you will need to install the
[Visual C++ Redistributable Packages for Visual Studio 2013](https://www.microsoft.com/en-US/download/details.aspx?id=40784).

## Bulding

You should only need to build if you want to release a new version.

* Install Visual C++ 2013 Community Edition

* Build the 64-bit Release target of zbar64

* Install the latest Python 2.7 release of [Anaconda](https://store.continuum.io/cshop/anaconda/)

* Build the zbar Python extension module and a wheel:

    ```
        cd <where you cloned this repo>\python
        pip install --upgrade pip
        pip install --upgrade setuptools
        pip install wheel
        build.bat
    ```
