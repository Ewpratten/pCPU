#! /bin/bash

python3 asm.py $1 > bin/"${1##*/}".txt
python3 asmi2bin.py bin/"${1##*/}".txt > bin/"${1##*/}".bin
cat bin/"${1##*/}".txt