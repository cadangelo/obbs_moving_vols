#/bin/bash

#MOAB_PATH=$HOME/opt/moab/
MOAB_PATH=$HOME/research/moab/
MOAB_LIBRARY=$MOAB_PATH"lib"
MOAB_INCLUDE=$MOAB_PATH"include"

echo $MOAB_LIBRARY

#g++  -std=c++11  obb.cpp -g -I/home/chelsea/opt/moab/include -L/home/chelsea/opt/moab/lib -lMOAB -Wl,-rpath,"$MOAB_LIBRARY" -o obb
#g++  -std=c++11 obb.cpp -g -I$MOAB_INCLUDE -L$MOAB_LIBRARY -lMOAB -o obb
g++  -std=c++11  obb.cpp -g -I/home/chelsea/research/moab/include -L/home/chelsea/research/moab/lib -lMOAB -Wl,-rpath,"$MOAB_LIBRARY" -o obb

