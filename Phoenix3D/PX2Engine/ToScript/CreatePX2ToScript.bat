@echo off

: dir
cd /d %cd%
del "PX2ToLua.cpp"
: tolua
..\\..\\..\\Bin\\tolua++.exe -n PX2 -o PX2ToLua.cpp PX2ToScript.pkg

del "PX2ToAngelScript.cpp"
..\\..\\..\\Bin\\PkgParser.exe PX2ToScript.pkg PX2ToAngelScript.cpp

pause