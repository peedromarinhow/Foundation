@echo off
SetLocal EnableDelayedExpansion

set Opts=-g -fuse-ld=lld-link -Wno-deprecated-declarations -Wno-pointer-sign -Wno-writable-strings -Wno-unknown-warning-option -Wno-microsoft-anon-tag -fdiagnostics-absolute-paths
set Srcs=../fontstash.c -o main.exe
set Incs=-I.
set Libs=-lkernel32 -luser32 -lwinmm -lgdi32

if not exist build mkdir build
echo Building.
pushd build
  clang %Opts% %Srcs% %Incs% %Libs%
  if %ERRORLEVEL% equ 0 echo Built succesfully.
  if %ERRORLEVEL% neq 0 echo Build failed.
popd
