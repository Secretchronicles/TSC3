Installation instructions for TSC
=================================

Time-stamp: <2016-08-03 10:13:02 quintus>

TSC uses [CMake][1] as the build system, so the first thing you have to
ensure is that you have CMake installed.

TSC supports the Linux and Windows platforms officially.
On Windows, testing is done on Windows 7.
**Windows XP is unsupported**.

To build TSC, do:

    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_INSTALL_PREFIX=/where/you/want/it ..
    $ make
    $ make install
