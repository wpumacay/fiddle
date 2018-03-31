# Optimization project - UCSP

This is a repo containing the final project of the second part of the Artifficial intelligence course @ UCSP.

## Swarm implementation

We implemented the basic variant of the Particle Swarm Optimization algorithm ( PSO ) in both python and c++ ( serial and Gpu implementations).

## To build

Just use cmake with an out of place build:

    mkdir build
    cd build
    cmake ..
    make

## To run

To run the test in c++ ( CUDA version ) use the following :

    ./testPSO_2 saveFileName

This will run the algorithm and save the iterations ( 100 by default ) in a .txt file.