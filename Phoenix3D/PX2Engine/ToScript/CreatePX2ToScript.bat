@echo off

: dir
cd /d %cd%
del "PX2ToLua.cpp"
: tolua
tolua++.exe -n PX2 -o PX2ToLua.cpp PX2ToLua.pkg

del "PX2ToAngelScript.cpp"
PkgParserD.exe PX2ToLua.pkg

pause