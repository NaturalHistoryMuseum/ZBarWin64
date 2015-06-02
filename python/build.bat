rem Builds wheels of zbar
echo Clean
del /S *pyc
del /S libzbar64-0.dll libiconv.dll
rmdir /Q /S dist build

echo Build
python setup.py build --compiler msvc bdist bdist_wheel
