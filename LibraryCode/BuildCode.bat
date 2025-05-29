@ECHO OFF

if EXIST "Bin\" (
  pushd LibraryCode
)

g++ -g -shared Main.cpp -I. -I../Engine/Source -o ../Bin/Lib.dll -L../Bin -lEngine

popd
