# Project Overview: Alpha-Beta Pruning Parallelization for Tic-Tac-Toe Game

This project aims to improve the performance of the MinMax algorithm used in the Tic-Tac-Toe game by parallelizing the **Alpha-Beta Pruning algorithm**. The parallelization is achieved using **OpenMP**, a widely-used API for shared memory multiprocessing. The goal is to reduce the time complexity and improve the efficiency of decision-making in the game by evaluating game tree branches concurrently.

## Problem Description

The focus of this project is to create a parallel version of the Alpha-Beta Pruning algorithm that optimizes the decision-making process in Tic-Tac-Toe. The traditional Alpha-Beta pruning algorithm is applied in sequential order, which can be time-consuming when dealing with deeper decision trees. By parallelizing the algorithm, we aim to minimize the number of nodes evaluated and increase computational efficiency.

## Solution

The parallelization approach is based on dividing the game tree branches and evaluating them independently across multiple threads. Each child node in the tree is evaluated in parallel, without depending on the results of other branches. This parallelization allows the main thread to gather the final results faster and compute the best move more efficiently.

The project simulates a player (marked as "X") against an AI (marked as "O"). Both the player and AI utilize the Alpha-Beta Pruning algorithm to compute their moves, and the game is designed in such a way that the match ends in a tie when both players use the optimal strategy.

## Theoretical Background

**Alpha-Beta Pruning** is an optimization technique used in game trees to reduce the number of nodes that need to be evaluated. By maintaining two threshold values, alpha and beta, the algorithm can eliminate branches that are guaranteed not to affect the final decision, thereby reducing computational time.

**OpenMP** is an API that allows developers to write parallel code for shared memory environments. It supports efficient parallelization of tasks and is commonly used in high-performance computing and scientific applications.

## Experimental Setup

The project was tested on a system with the following specifications:
- **CPU**: Intel(R) Core(TM) i7-10510U CPU @ 1.80GHz, 4 cores and 8 threads
- **RAM**: 8 GB

The algorithm was evaluated using various levels of compiler optimizations (O0, O1, O2, O3) to analyze its performance and efficiency under different conditions.

## Performance and Optimization

The performance of the algorithm is measured using **speedup**—the ratio of sequential execution time to parallel execution time. Speedup values greater than 1 indicate performance improvement. The project explored different optimization levels, with the parallelization showing significant improvements in early thread usage but diminishing returns with more threads as the game tree became smaller.

## Key Findings

- The parallel version of the algorithm showed better performance than the sequential version, particularly with 2 to 4 threads.
- Due to the limited complexity of Tic-Tac-Toe, the benefits of parallelization were modest. More significant gains would be expected in more complex games like chess or checkers.


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
## Conclusion

This project demonstrates the potential of parallelizing the Alpha-Beta Pruning algorithm using OpenMP to optimize decision-making in games like Tic-Tac-Toe. While the improvements in Tic-Tac-Toe are moderate due to the limited game complexity, the approach has strong potential for more complex decision trees.
