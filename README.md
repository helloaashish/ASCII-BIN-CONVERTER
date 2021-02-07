# ASCII-BIN-CONVERTER



Get matis from here:
http://glaros.dtc.umn.edu/gkhome/metis/metis/download

locate metis.h -> metis-5.1.0/include/metis.h

# Compile:

## Convert to Binary:

g++ -std=c++11 converter.cpp graphReader.cpp -o converter

## Read Binary :
g++ -std=c++11 readBin.cpp -o reader

# Run:

./converter 

./reader
