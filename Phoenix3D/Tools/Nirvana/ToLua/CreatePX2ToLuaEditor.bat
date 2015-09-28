: dir
cd /d %cd%
del "PX2ToLuaEditor.cpp"
: tolua
tolua++.exe -n PX2Editor -o PX2ToLuaEditor.cpp PX2ToLuaEditor.pkg