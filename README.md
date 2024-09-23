# CommonAssignmentOpenMP

## Dependencies
* Python3
* OMP

## How to run
Assumption: The code for generating directories, tables and plots requires the python interpreter and the matplotlib library to be installed entering the command: pip3 install matplotlib

1.	Navigate to the folder containing the makefile
2.	To clear previously obtained achievements and builds, enter the command: make clean
3.	To generate the necessary directories and compile and linking the various source codes, enter the command: make all
4.	To run the algorithm for making tests, producing results, measurements, graphs and tables, enter the command: make test

The results of the algorithm can be viewed in the 'Results' folder, divided by type of optimization.
The execution times of the algorithms and their average values can be viewed respectively in the 'Informations' and 'Results' folders, divided by type of optimization.
The results in graphical and tabular format can be viewed respectively in the 'Plots' and 'Tables' folders, divided by type of optimization.