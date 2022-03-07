#!/bin/sh

#BSUB -J ThesisCodeEA

#BSUB -n 24
#BSUB -R "span[ptile=24]"
#BSUB -W 00:59
#BSUB -q hpc
#BSUB -R "rusage[mem=1GB]"
#BSUB -o build/Output_%J.out 

cd build

cmake .. -DCMAKE_BUILD_TYPE=Release

make

./ThesisCode

