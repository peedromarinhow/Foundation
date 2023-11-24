@echo off
SetLocal EnableDelayedExpansion

set Opts=-g -Wno-pointer-sign
set Srcs=../main.c -I../download/
set Defs=-DDEBUG
set Libs=-lkernel32 -luser32 -lwinmm -lgdi32
set Name=main.exe

if not exist build mkdir build
echo Building.
pushd build
  @REM clang %Opts% %Srcs% %Incs% %Libs% -o %Name%
  @REM clang %Opts% ../main_gl.c %Incs% %Libs% -o main_gl.exe
  @REM clang %Opts% ../main_d3.c %Incs% %Libs% -o main_d3.exe
  clang %Opts% %Srcs% %Defs% %Libs% -o %Name%
  if %ERRORLEVEL% equ 0 echo Built succesfully.
  if %ERRORLEVEL% neq 0 echo Build failed.
popd
