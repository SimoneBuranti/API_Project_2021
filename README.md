# GraphRanker – Data Structures and Algorithms final Project

This project implements **GraphRanker**, a program developed as part of the Data Structures and Algorithms coursework.  
It focuses on efficient graph manipulation and ranking based on shortest paths.

## 🚀 Project Overview
- **Language:** C (C11, standard libraries only, no external dependencies)
- **Context:** Practical application of graph algorithms within the scope of efficiency-focused programming
- **Core Idea:** Maintain a ranking of the *k* "best" directed, weighted graphs according to a shortest-path metric

## 🧩 Functionality
- **AddGraph**: Insert a new graph by providing its adjacency matrix
- **TopK**: Output the indices of the *k* graphs with the smallest sum of shortest paths from node `0` to all reachable nodes

Graphs are processed in sequence, and ties are resolved by insertion order.

## 🛠️ Example
**Input:**<br>
3 2 <br>
AggiungiGrafo<br>
0,4,3<br>
0,2,0<br>
2,0,0<br>
AggiungiGrafo<br>
0,0,2<br>
7,0,4<br>
0,1,0<br>
TopK<br>

**Output:**<br>
0 1


## 📂 Repository Structure
- `Delivery/` – C source code (`API_Project_Buranti.c`)
- `inputgen.py, suitegen.py` – Python utility for generating test cases

## 🏆 Evaluation
This project was awarded the maximum grade of **30L**.  
The evaluation was based on automated testing of correctness and efficiency across multiple test batteries.


## 📜 License
Distributed under the [MIT License](LICENSE).

