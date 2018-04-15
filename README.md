[![Linux Build Status](https://travis-ci.org/fancidev/euler.svg?branch=master)](https://travis-ci.org/fancidev/euler)
[![Window Build Status](https://ci.appveyor.com/api/projects/status/q88yfg3w89bmxk54/branch/master?svg=true)](https://ci.appveyor.com/project/fancidev/euler/branch/master)

[Project Euler](https://projecteuler.net/) is a website that provides programming challenges to solve math problems. The majority of problems are related to number theory, but they also cover topics in geometry, combinatorics, game theory and even physics.

This repository contains (1) code solutions to Project Euler problems, (2) well-documented C++ library of common routines used by the solutions, and (3) detailed analysis of selected problems. 

This repository is licenced under the [MIT License](LICENSE).

## What's Inside

The [src](src) directory contains C++ solutions to the problems that I have solved. Solution to problem number `<N>` is placed in file `p<N>.cpp`.

The [src/euler](src/euler) directory contains a C++ library of common routines used by the solutions. The library covers prime generation, modular arithmetic, and more. The code is well-documented in doxygen format.

The [notes](notes) directory contains detailed analysis of selected (usually difficult) problems, in TeX format. Analysis of problem number `<N>` is placed in file `p<N>.tex`.

## How to Build

[CMake](https://cmake.org/) is used to build the project. Both Linux and Windows are supported.

Under Linux, it is recommended to perform an out-of-source build. From the root directory of the working copy, run the following to generate the necessary Makefiles:
```
mkdir build
cd build
cmake ..
```
The remaining commands should be executed from the `build` directory. To build the code that computes solutions to each problem and verify them against the known correct answers:
```
make && make test
```
To generate HTML documentation for the C++ library of math routines (available only if you have `doxygen` installed):
```
make doc
```
To generate PDF files of the notes (available only if you have `pdflatex` installed):
```
make pdf
```

Under Windows, it is recommended to use Visual Studio 2017, which has built-in support for CMake. Choose File > Open > Folder, and select the root folder of the working copy. After the project is loaded, choose CMake > Build All. After the project is built, choose CMake > Run Tests > euler to compute and verify the solutions.

## Problem Rating

For each problem that I solved, I rate its difficulty and fun level on a scale of 1 to 3. Hopefully this helps you to quickly locate problems of interest without going through the entire list.

_Difficulty_ is a subjective measure of how hard it was for me to solve the problem. I rate difficulty on three levels:

* ☆ - straightforward
* ☆☆ - fair 
* ☆☆☆ - hard 

An alternative, objective measure of a problem's difficulty is the number of users that have successfully solved it. This statistic is available in the [problem list](https://projecteuler.net/archives).

_Fun Level_ is a subjective measure of how interesting I find the problem. I rate fun level on three levels:

* ☆ - The problem is either trivial or obscure. Not worth a look.
* ☆☆ - The problem is challenging or interesting.
* ☆☆☆ - The problem is challenging and interesting, and the solution is practically useful. It's worth a look.

## Solution Complexity

For each problem that I solved, I give an estimate of the time and space complexity of my solution.

Be aware that my solution may not be optimal (in terms of complexity) for a few reasons:

* I may not have been able to find an optimal solution;
* There may be a trade-off between time and space, and I may have opted for one or the other;
* The scale of a problem may be too small to deserve the effort of devising a complex but optimal solution.

You may refer to Project Euler's online forum to look at how others tackle the problems.

## Remarks

I developed this repository during 2011 to 2012 and hosted it on [Google Code](https://code.google.com/archive/p/fun-math-problems/). I take the opportunity of migrating it to github to modernize some of the workflows.
