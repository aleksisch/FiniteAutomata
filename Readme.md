# Finite Automata
## What is it
This is a program to get minimal determenistic finite automata from non-determenistic.
## Build
```
cmake
make
```
## How to use
-h - flag to get help and exit.
-a - inverse result automata
## Input
Input automata format:
```
S
F
f1 f2 f3 ...
A
s1 s2 s3 ...
N M
s t c
...

S - start vertex.
F - number of finish states
f1 f2 f3 ... - finish states
A - number of letter in alphabet
s1 s2 s3 ... - letter, separated by space
N M - number of vertex and edges
s t c - start vertex, end vertex, letter
```

## Smoke tests
To make tests you should execute python script
```
pytest test.py
```