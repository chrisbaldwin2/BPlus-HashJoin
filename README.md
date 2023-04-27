# B+ Tree and Hash Based Joining for CSCE 608
Christopher Baldwin

## Introduction
This project is to implement B+ Tree and Hash Based Joining for CSCE 608.

## Environment
- g++ (C++20)

## Usage
- Clean: `make clean`
- Compile: `make`
- Run: `bin/main <test> > output.txt`

## Tests
- `btree1`: Tests the order 13 dense tree
- `btree2`: Tests the order 13 sparse tree
- `btree3`: Tests the order 24 dense tree
- `btree4`: Tests the order 24 sparse tree
- `hash1`: Tests 5.1 - R w/ 1000 tuples and using S's keys
- `hash2`: Tests 5.2 - R w/ 1200 tuples and using random keys = [20000, 30000)
- `all`: Runs all tests

