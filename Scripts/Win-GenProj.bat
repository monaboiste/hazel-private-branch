@echo off
pushd %cd%
cd /d %~dp0
cd ../
call Vendor\premake\Bin\premake5.exe vs2019
popd
PAUSE