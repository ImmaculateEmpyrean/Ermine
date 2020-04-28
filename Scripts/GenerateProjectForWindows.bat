@echo off
pushd %~dp0\..\Workspace\
call %~dp0\..\Tools\premake5.exe vs2019
popd
PAUSE