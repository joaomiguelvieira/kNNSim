# KNNSimulator

This repository contains a simulator written in C language to evaluate the performance of the KNN clustering algorithm in different platforms. The content of this repository is as follows:
* `./` contains the source of the simulator. To compile:
  - **Linux:** just type `make`;
  - **macOS:** define `MACOS=1` as a bash variable and type `make`.
* `./datasets/` contains the raw data of seven datasets that were download from [UCI Machine Learning Repository](https://archive.ics.uci.edu/ml/index.php) and precompiled binaries that can be used out of the box with the simulator.
* `./sim/` contains some experimental results extracted from several runnings on different systems.

## Usage

To know how to use the simulator, just type `./knnsim -h` after compiling the software. It will produce:
