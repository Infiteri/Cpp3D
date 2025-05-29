@ECHO OFF

if EXIST "Bin\" (
  pushd LibraryCode
)

g++ -g -shared Main.cpp -I. -o ../Bin/Lib.dll -L../Bin -lEngine

popd