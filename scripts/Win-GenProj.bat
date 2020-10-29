@echo off
pushd %cd%
cd /d %~dp0
cd ../
call vendor\bin\premake\premake5.exe vs2019
popd
PAUSE