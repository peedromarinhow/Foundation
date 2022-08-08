@echo off
SetLocal EnableDelayedExpansion

set Opts=-g -Wno-deprecated-declarations -Wno-pointer-sign -Wno-writable-strings -Wno-unknown-warning-option -Wno-microsoft-anon-tag -fdiagnostics-absolute-paths
set Defs=-DENABLE_ASSERT=1 -DDEBUG=1
set Srcs=../main.c -o main.exe
set Incs=-I.
set Libs=-luser32 -lwinmm -lgdi32

where curl > nul
if %ERRORLEVEL% neq 0 (
  echo You do not have curl installed. Download these files and put them in this same folder: https://registry.khronos.org/OpenGL/api/GL/glcorearb.h, https://registry.khronos.org/OpenGL/api/GL/glcorearb.h and https://registry.khronos.org/EGL/api/KHR/khrplatform.h. This last one must be put in a folder named KHR besides the other files.
) else (
  if not exist download (
    mkdir download
    echo Downloading opengl headers.
    echo Remember to replace angle brackets with double quotes in download\glcorearb.h:63:10. because for some reason clang wont allow that. Also remove
    pushd download
      if not exist KHR mkdir KHR
      pushd KHR
        if not exist khrplatform.h curl -s -o khrplatform.h https://registry.khronos.org/EGL/api/KHR/khrplatform.h
      popd
      if not exist glcorearb.h curl -s -o glcorearb.h https://registry.khronos.org/OpenGL/api/GL/glcorearb.h
      if not exist glext.h     curl -s -o glext.h     https://registry.khronos.org/OpenGL/api/GL/glcorearb.h
    popd
  )
)

call %vcvarsall% x64

if not exist build mkdir build
echo Building.
pushd build
  clang %Opts% %Defs% %Srcs% %Incs% %Libs%
  if %ERRORLEVEL% equ 0 echo Built succesfully.
  if %ERRORLEVEL% neq 0 echo Build failed.
popd
