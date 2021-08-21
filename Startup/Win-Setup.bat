@echo off
pushd ..\
call premake\premake5.exe vs2019
popd
PAUSE