# toy-assembler

An attempt to write an assembler for the [X-TOY machine from Princeton](https://lift.cs.princeton.edu/xtoy/) in C++.

[grammar.md](grammar.md) is the only semi-complete portion (it defines the ARM-like language used by the assembler). Most other things work to an extent.

The TOY machine uses *.toy as its file extension. This assembler take in *.atoy (assembly for TOY) files, and outputs *.toy files.

Princeton's implementation is written in Java and doesn't work with a terminal. Try [this](https://github.com/EthanDenny/python-x-toy) instead.

Documentation for X-TOY machine code can be found [here](https://introcs.cs.princeton.edu/java/62toy/cheatsheet.txt).
