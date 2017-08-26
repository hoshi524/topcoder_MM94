set -u

cd `dirname $0`

mkdir out

javac src/ConnectedComponentVis.java -d out

g++ --std=c++0x -W -Wall -Wno-sign-compare -O2 -s -pipe -mmmx -msse -msse2 -msse3 -o out/a.out $1

java -cp out/ ConnectedComponentVis out/a.out