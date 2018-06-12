# Utils
Some light-weighted utilities to share

## Installation
The provided packages are header-only, there is no compilation. It is using CMake for installing the files. Under Unix system, the following commands install the files in a custom directory :

`mkdir build; cd build; cmake .. -DCMAKE_INSTALL_PREFIX=<path>; make install;`
  
The uninstall target is also provided if you need to remove the files from your system :

`make uninstall`

## Packages

* [Union-Find](union_find/README.md)
* [Workflow](workflow/README.md)
