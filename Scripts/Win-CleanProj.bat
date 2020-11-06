@echo off
pushd %cd%
cd /d %~dp0
cd ../

echo Clean Hazel.sln
del /q /f "*.sln" >nul 2>&1

echo Cleaning Hazel/Hazel.vcxproj ...
echo Cleaning Hazel/Hazel.vcxproj.filters ...
del /q /f "Hazel\*.vcxproj*" >nul 2>&1

echo Cleaninig Hazel/Hazelnut.vcxproj...
echo Cleaninig Hazel/Hazelnut.vcxproj.filters ...
del /q /f "Hazel\*.vcxproj*" >nul 2>&1

echo Clean Hazel/Sandbox.vcxproj ...
del /q /f "Hazel\*.vcxproj*" >nul 2>&1

popd
PAUSE