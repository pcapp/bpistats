# Bitcoin Price Index (BPI) Stats

## ABOUT
This application reads in JSON files based on the format described in the following Coindesk API [document](https://www.coindesk.com/api/). Please refer to `src/example.json` for a sample document.


## REQUIREMENTS
* CMAKE
* A C++ compiler capable of using the C++17 standard.
* A copy of Google Test installed on the system.

## BUILDING

First, please ensure your system has a copy of [Google Test](https://github.com/google/googletest) installed. CMAKE requires this to be installed. Unix-like systems will install this in a location that CMAKE's `find_package` command can find; Windows may require extra help, which this document will describe shortly.

1. Clone the project from GitHub.
2. With a terminal, `cd ${PROJECT_ROOT}` where `${PROJECT_ROOT}` is the top-level folder where you installed the project.
3. Enter these commands: 
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

On Windows, the sequence of commands may look something like this:
1. Open a Visual Studio 15 2017 Developer Developer Command prompt.
2. `cd` to the `${PROJECT_ROOT}`.
3. Enter these commands:
```
$ mkdir build
$ cd build
$ cmake .. -DGTEST_ROOT=C:\src\googletest-distribution -G "NMake Makefiles"
$ nmake
```

This currently builds two artifacts: `test_all.exe` and `stats.exe`.

## TESTING
After building the project, enter these commands (this will use Windows/NMake as an example) from the `${PROJECT_ROOT}`:

```
$ cd build\test
$ test_all.exe
```

This will run Google Test's driver, which runs the test suite.

## RUNNING
From the project root:
```
$ cd build\src
$ stats.exe example.json
```

## USAGE
`$ stats.exe <filename>`