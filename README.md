# Utils
Some light-weighted utilities to share

## Installation
The provided packages are header-only, there is no compilation. It is using CMake for installing the files. Under Unix system, the following commands install the files in a standard directory :

`mkdir build; cd build; cmake ..; make install;`

To install in a custom directory, use the `CMAKE_INSTALL_PREFIX` option of CMake. The uninstall target is also provided if you need to remove the files from your system :

`make uninstall`

## Packages

* [Union-Find](union_find/README.md)
* [Workflow](workflow/README.md)
