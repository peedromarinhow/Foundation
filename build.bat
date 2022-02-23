@echo off
cls

set opts=-DENABLE_ASSERT=1 -DDEBUG=1
set libs=Winmm.lib Userenv.lib Advapi32.lib
set cl_flags=-nologo -FC -GR- -EHa- -Zi -std:c++latest %opts%

call %vcvarsall% x64 > NUL 2> NUL

if not exist build mkdir build
pushd build
echo building
cl.exe %cl_flags% ../main.cc /link %libs%
main.exe ../input.pr
popd
