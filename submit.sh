#!/bin/sh

#BSUB -J ThesisCodeEA

#BSUB -n 32
#BSUB -R "span[ptile=32]"
#BSUB -W 11:00
#BSUB -q hpc
#BSUB -R "rusage[mem=1GB]"
#BSUB -o build/Output_%J.out 

cd build

cmake .. -DCMAKE_BUILD_TYPE=Release

make

./ThesisCode

