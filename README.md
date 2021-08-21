# BioInf-Contest-2021

This repository contains my solutions for Bioinformatics Contest 2021 Final Round.
Link to contest: stepik.org/course/91751/info
Link to the scoreboard: http://mon.stepik.org/2021/?_ga=2.113996608.653995406.1629575396-843884434.1623649917

There are 5 folders, 1 for each problem.
Each folder contains:
- short computer-science-like problem statements (details are missed, read original statements for accurate description);
- short description of idea to solve the problem;
- *.cpp file (may be 2 files) with implemented solution;
- smallest input data example (given in contest);

Visual Studio 2019 (and respective compiler) was used during the contest, but any С++ compiler should be ok.
OpenMP was used in some problems.
To enable OpenMP in VS2019 go Project Properties -> C/C++ -> Language -> Open MP Support set to Yes.
If you are not using OpenMP, clear the "#pragma omp parallel for num_threads(8)" line in the code. 
Input data path, algorithms parameters, number of threads are hardcoded in code (they are not parameters of the program).

All input data can be downloaded via the original contest link.

Code is modified after the contest for purposes of optimization, minor comments, readability, but last is not guaranteed.

Some of the code may run up to several hours on the biggest input files.