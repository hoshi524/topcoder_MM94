cd `dirname $0`

mkdir out

javac src/ConnectedComponentVis.java -d out

g++ --std=c++0x -W -Wall -Wno-sign-compare -O2 -s -pipe -mmmx -msse -msse2 -msse3 src/ConnectedComponent.cpp
mv a.out out/

java -cp out/ ConnectedComponentVis -exec out/a.out