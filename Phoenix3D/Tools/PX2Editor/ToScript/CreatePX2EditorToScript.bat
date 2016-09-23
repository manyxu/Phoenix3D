@echo off

: dir
cd /d %cd%
del "PX2EditorToLua.cpp"
: tolua
..\..\..\..\Bin\tolua++.exe -n PX2Editor -o PX2EditorToLua.cpp PX2EditorToScript.pkg

pause